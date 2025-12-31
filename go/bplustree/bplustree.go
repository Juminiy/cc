package bplustree

type BPlusTree[T comparable] struct {
	size   int
	height int
	degree int
	root   *bNode[T]
	cmpF   func(T, T) int

	keyMinCnt int
	keyMaxCnt int
	opt       int
}

type bNode[T comparable] struct {
	elem       []T
	child      []*bNode[T]
	prev, next *bNode[T]
}

type bNodeSplit[T comparable] struct {
	elem     T
	lnd, rnd *bNode[T]
}

func NewBPlusTree[T comparable](degree int, cmpF func(T, T) int) *BPlusTree[T] {
	return &BPlusTree[T]{
		size:      0,
		height:    0,
		degree:    degree,
		root:      nil,
		cmpF:      cmpF,
		keyMinCnt: halfUpper(degree) - 1,
		keyMaxCnt: degree - 1,
	}
}

func (t *BPlusTree[T]) Size() int {
	return t.size
}

func (t *BPlusTree[T]) Height() int {
	return t.height
}

func (t *BPlusTree[T]) Insert(e T) {
	t.opt = 1
	if split := t.insert(t.root, e); split != nil {
		newRoot := newBNode([]T{split.elem}, nil)
		if split.lnd != nil && split.rnd != nil {
			newRoot.child = append(newRoot.child, split.lnd, split.rnd)
		}
		t.root = newRoot
		t.height++
	}
	t.size += t.opt
}

func (t *BPlusTree[T]) Delete(e T) {
	t.opt = 0
	t.delete(t.root, e)
	if len(t.root.elem) == 0 {
		if len(t.root.child) > 0 {
			t.root = t.root.child[0]
		} else {
			t.root = nil
		}
		t.height -= 1
	}
	t.size += t.opt
}

func (t *BPlusTree[T]) Get(e T) (T, bool) {
	for nd := t.root; nd != nil; {
		if geIndex, eq := t.getGEIndex(nd, e); eq {
			if len(nd.child) == 0 {
				return nd.elem[geIndex], true
			} else {
				nd = nd.child[geIndex+1]
			}
		} else {
			nd = nd.child[geIndex]
		}
	}
	return e, false
}

func (t *BPlusTree[T]) Iterate(vis func(T) bool) {
	for cur := t.minNode(); cur != nil; cur = cur.next {
		for _, e := range cur.elem {
			if !vis(e) {
				return
			}
		}
	}
}

func (t *BPlusTree[T]) IterateLevel(level int, vis func(T) bool, nodeBrace ...func()) {
	b0fn, b1fn := func() {}, func() {}
	if len(nodeBrace) > 0 {
		b0fn, b1fn = nodeBrace[0], nodeBrace[0]
	}
	if len(nodeBrace) > 1 {
		b1fn = nodeBrace[1]
	}
	for cur := t.levelNode(level); cur != nil; cur = cur.next {
		b0fn()
		for _, e := range cur.elem {
			if !vis(e) {
				return
			}
		}
		b1fn()
	}
}

func (t *BPlusTree[T]) insert(nd *bNode[T], e T) *bNodeSplit[T] {
	if nd == nil {
		return &bNodeSplit[T]{elem: e}
	}

	geIndex, eq := t.getGEIndex(nd, e)
	if len(nd.child) == 0 { // leafNode
		if eq {
			nd.elem[geIndex] = e
			t.opt = 0
			return nil
		} else {
			nd.insertElem(geIndex, e)
		}
	} else { // internalNode
		if eq {
			geIndex += 1
		}
		if split := t.insert(nd.child[geIndex], e); split != nil {
			nd.insertElem(geIndex, split.elem)
			nd.insertChild(geIndex, split.lnd, split.rnd)
		}
	}

	if len(nd.elem) > t.keyMaxCnt {
		return nd.split()
	}

	return nil
}

func (t *BPlusTree[T]) delete(nd *bNode[T], e T) { // *bNode[T]
	if nd == nil {
		return
	}

	geIndex, eq := t.getGEIndex(nd, e)
	if len(nd.child) == 0 { // leafNode
		if eq {
			t.opt = -1
			nd.deleteElem(geIndex)
		}
	} else { // internalNode
		chIndex, chNode := geIndex, nd.child[geIndex]
		if eq { // eq: index replacement, both in internalNode and leafNode
			chIndex, chNode = geIndex+1, nd.child[geIndex+1]
			nd.updateIndex(geIndex, chNode)
		}
		t.delete(chNode, e)
		if len(chNode.elem) < t.keyMinCnt {
			t.borrowOrMerge(nd, chIndex, chNode)
		}
	}
}

// parent.child[curIndex] = nd
func (t *BPlusTree[T]) borrowOrMerge(parent *bNode[T], curIndex int, nd *bNode[T]) {
	hasLeftSib, hasRightSib := curIndex-1 >= 0, curIndex+1 < len(parent.child)

	if hasLeftSib {
		if nd.borrowLeft(parent, curIndex, t.keyMinCnt) { // borrow ok
			return
		}
	}
	if hasRightSib {
		if nd.borrowRight(parent, curIndex, t.keyMinCnt) { // borrow ok
			return
		}
	}
	if hasLeftSib {
		leftSibling := parent.child[curIndex-1]
		leftSibling.merge(parent.elem[curIndex-1], nd)
		linkBNode(leftSibling, nd.next)
		parent.deleteChild(curIndex)
		parent.deleteElem(curIndex - 1)
		return
	}
	if hasRightSib {
		rightSibling := parent.child[curIndex+1]
		nd.merge(parent.elem[curIndex], rightSibling)
		linkBNode(nd, rightSibling.next)
		parent.deleteChild(curIndex + 1)
		parent.deleteElem(curIndex)
		return
	}
}

func (t *BPlusTree[T]) getGEIndex(nd *bNode[T], e T) (ge int, eq bool) {
	geIndex := len(nd.elem)
	for i := range nd.elem {
		if cmpRes := t.cmpF(nd.elem[i], e); cmpRes == 0 {
			return i, true
		} else if cmpRes > 0 {
			return i, false
		}
	}
	return geIndex, false
}

func (t *BPlusTree[T]) minNode() *bNode[T] {
	return t.root.minLeafNode()
}

func (t *BPlusTree[T]) levelNode(level int) *bNode[T] {
	for l, cur := 1, t.root; cur != nil && l <= min(t.height, level); l++ {
		if l == level || len(cur.child) == 0 {
			return cur
		}
		cur = cur.child[0]
	}
	return nil
}

func newBNode[T comparable](elem []T, child []*bNode[T]) *bNode[T] {
	emcpy := make([]T, len(elem))
	chcpy := make([]*bNode[T], len(child))
	copy(emcpy, elem)
	copy(chcpy, child)
	return &bNode[T]{
		elem:  emcpy,
		child: chcpy,
	}
}

func (nd *bNode[T]) split() *bNodeSplit[T] {
	d := len(nd.elem)
	var lnd, rnd *bNode[T]
	if len(nd.child) > 0 { // internalNode
		lnd = newBNode(nd.elem[:d/2], nd.child[:d/2+1])
		rnd = newBNode(nd.elem[d/2+1:], nd.child[d/2+1:])
	} else { // leafNode
		lnd = newBNode(nd.elem[:d/2], nil)
		rnd = newBNode(nd.elem[d/2:], nil)
	}
	linkBNode(nd.prev, lnd)
	linkBNode(lnd, rnd)
	linkBNode(rnd, nd.next)

	return &bNodeSplit[T]{
		elem: nd.elem[d/2],
		lnd:  lnd,
		rnd:  rnd,
	}
}

func (nd *bNode[T]) insertElem(i int, e T) {
	nd.elem = append(nd.elem, e)
	if i != -1 {
		for idx := len(nd.elem) - 1; idx > i; idx-- {
			nd.elem[idx] = nd.elem[idx-1]
		}
		nd.elem[i] = e
	}
}

func (nd *bNode[T]) insertChild(i int, lnd, rnd *bNode[T]) {
	nd.child = append(nd.child, lnd)
	for idx := len(nd.child) - 1; idx > i; idx-- {
		nd.child[idx] = nd.child[idx-1]
	}
	nd.child[i] = lnd
	nd.child[i+1] = rnd
	//if i > 0 {
	//	linkBNode(nd.child[i-1], nd.child[i])
	//}
	//if i+2 < len(nd.child) {
	//	linkBNode(nd.child[i+1], nd.child[i+2])
	//}
}

func (nd *bNode[T]) merge(parentElem T, right *bNode[T]) {
	if len(nd.child) == 0 { // leafNode
		nd.elem = append(nd.elem, right.elem...)
	} else { // internalNode
		nd.elem = append(append(nd.elem, parentElem), right.elem...)
		nd.child = append(nd.child, right.child...)
	}
}

func (nd *bNode[T]) borrowLeft(parent *bNode[T], curIndex int, keyMinCnt int) bool {
	if leftSibling := parent.child[curIndex-1]; len(leftSibling.elem) > keyMinCnt { // left enough
		lastElem := leftSibling.deleteElem(-1)
		if len(nd.child) == 0 { // leafNode
			nd.insertElem(0, lastElem)
		} else { // internalNode
			nd.insertElem(0, parent.elem[curIndex-1])
		}
		parent.elem[curIndex-1] = lastElem
		return true
	}
	return false
}

func (nd *bNode[T]) borrowRight(parent *bNode[T], curIndex int, keyMinCnt int) bool {
	if rightSibling := parent.child[curIndex+1]; len(rightSibling.elem) > keyMinCnt { // right enough
		firstElem := rightSibling.deleteElem(0)
		if len(nd.child) == 0 {
			nd.insertElem(-1, firstElem)
			parent.elem[curIndex] = rightSibling.elem[0]
		} else {
			nd.insertElem(-1, parent.elem[curIndex])
			parent.elem[curIndex] = firstElem
		}
		return true
	}
	return false
}

func (nd *bNode[T]) deleteElem(i int) T {
	var e T
	if i == -1 { // last elem
		e = nd.elem[len(nd.elem)-1]
		nd.elem = nd.elem[:len(nd.elem)-1]
	} else {
		e = nd.elem[i]
		nd.elem = append(nd.elem[:i], nd.elem[i+1:]...)
	}
	return e
}

func (nd *bNode[T]) deleteChild(i int) {
	nd.child = append(nd.child[:i], nd.child[i+1:]...)
}

func (nd *bNode[T]) minElem() (e T, ok bool) {
	if cur := nd.minLeafNode(); cur != nil && len(cur.elem) > 0 {
		return cur.elem[0], true
	}
	return e, false
}

func (nd *bNode[T]) minLeafNode() *bNode[T] {
	for cur := nd; cur != nil; {
		if len(cur.child) == 0 {
			return cur
		}
		cur = cur.child[0]
	}
	return nil
}

// |nd|
//
//	  \
//	rchNode
func (nd *bNode[T]) updateIndex(geIndex int, rChNode *bNode[T]) {
	if leafNode := rChNode.minLeafNode(); leafNode != nil {
		if leafNode.elem[0] == nd.elem[geIndex] {
			if len(leafNode.elem) >= 2 {
				nd.elem[geIndex] = leafNode.elem[1]
			} else { // must be merged
			}
		} else {
			nd.elem[geIndex] = leafNode.elem[0]
		}
	}
}

func linkBNode[T comparable](lnd, rnd *bNode[T]) {
	if lnd != nil {
		lnd.next = rnd
	}
	if rnd != nil {
		rnd.prev = lnd
	}
}

// internalNode: keyNum: [ halfUpper(d)-1, d-1 ], childNum: keyNum+1
// leafNode: keyNum: [ halfUpper(d), d-1 ]
// so Split: keyNum == d
// so Merge: keyNum == halfUpper(d)-1
func halfUpper(x int) int {
	halfx := x >> 1
	if x%2 == 1 {
		halfx += 1
	}
	return halfx
}
