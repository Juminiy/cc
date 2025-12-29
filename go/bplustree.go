package main

import "fmt"

type BPlusTree[T comparable] struct {
	size   int
	height int
	degree int
	root   *bNode[T]
	cmpF   func(T, T) int

	opt int
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
		size:   0,
		height: 0,
		degree: degree,
		root:   nil,
		cmpF:   cmpF,
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

	if len(nd.elem) == t.degree {
		return nd.split()
	}

	return nil
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
	for idx := len(nd.elem) - 1; idx > i; idx-- {
		nd.elem[idx] = nd.elem[idx-1]
	}
	nd.elem[i] = e
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

func (t *BPlusTree[T]) Delete(e T) {
	t.opt = 0
	t.delete(nil, t.root, e)
	t.size += t.opt
}

func (t *BPlusTree[T]) delete(parent, nd *bNode[T], e T) {
	if nd == nil {
		return
	}

	geIndex, eq := t.getGEIndex(nd, e)
	if len(nd.child) == 0 { // leafNode
		if eq {
			t.opt = -1
			nd.deleteElem(geIndex)
		} else {
			return
		}
	} else { // internalNode
		if eq {
			geIndex += 1
		}
		t.delete(nd, nd.child[geIndex], e)
	}

	if len(nd.elem) == halfUpper(t.degree)-1 {

	}

}

func (nd *bNode[T]) deleteElem(i int) {
	nd.elem = append(nd.elem[:i], nd.elem[i+1:]...)
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

func (t *BPlusTree[T]) minNode() *bNode[T] {
	for nd := t.root; nd != nil; {
		if len(nd.child) == 0 {
			return nd
		}
		nd = nd.child[0]
	}
	return nil
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

func (t *BPlusTree[T]) levelNode(level int) *bNode[T] {
	for l, cur := 1, t.root; cur != nil && l <= min(t.height, level); l++ {
		if l == level || len(cur.child) == 0 {
			return cur
		}
		cur = cur.child[0]
	}
	return nil
}

func (t *BPlusTree[T]) IterateLevel(level int, vis func(T) bool) {
	for cur := t.levelNode(level); cur != nil; cur = cur.next {
		fmt.Printf("[ ")
		for _, e := range cur.elem {
			if !vis(e) {
				return
			}
		}
		fmt.Printf(" ]")
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
