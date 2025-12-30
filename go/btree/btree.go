package main

type BTree[T any] struct {
	Degree int
	Height int
	Size   int
	Root   *BNode[T]
	Cmp    func(T, T) int

	insert int
	delete int
}

type BNode[T any] struct {
	Child []*BNode[T]
	Elem  []T
}

type splitNode[T any] struct {
	lnd, rnd *BNode[T]
	elem     T
}

func NewBTree[T any](degree int, cmp func(T, T) int) *BTree[T] {
	return &BTree[T]{
		Degree: degree,
		Height: 0,
		Size:   0,
		Root:   nil,
		Cmp:    cmp,
	}
}

func newBNode[T any](e []T, ch []*BNode[T]) *BNode[T] {
	ecpy, chcpy := make([]T, len(e)), make([]*BNode[T], len(ch))
	copy(ecpy, e)
	copy(chcpy, ch)
	return &BNode[T]{
		Child: chcpy,
		Elem:  ecpy,
	}
}

func (t *BTree[T]) Insert(e T) {
	t.insert = 1
	if split := t.insertNode(t.Root, e); split != nil {
		t.Root = newBNode([]T{split.elem}, nil)
		if split.lnd != nil && split.rnd != nil {
			t.Root.Child = append(t.Root.Child, split.lnd, split.rnd)
		}
		t.Height++
	}
	t.Size += t.insert
}

func (t *BTree[T]) Delete(e T) {
	t.delete = 0

	t.Size += t.delete
}

func (t *BTree[T]) Get(e T) (T, bool) {
	for cur := t.Root; cur != nil; {
		gtIndex, eq := t.getGEIndex(cur, e)
		if eq {
			return cur.Elem[gtIndex], true
		} else if len(cur.Child) > 0 {
			cur = cur.Child[gtIndex]
		} else {
			return e, false
		}
	}
	return e, false
}

func (t *BTree[T]) Iterate(f func(T) bool) {
	t.iterateNode(t.Root, f)
}

func (t *BTree[T]) iterateNode(nd *BNode[T], f func(T) bool) {
	if nd == nil {
		return
	}
	if len(nd.Child) > 0 {
		for idx := 0; idx < len(nd.Elem); idx++ {
			t.iterateNode(nd.Child[idx], f)
			if !f(nd.Elem[idx]) {
				return
			}
		}
		t.iterateNode(nd.Child[len(nd.Elem)], f)
	} else {
		for idx := 0; idx < len(nd.Elem); idx++ {
			if !f(nd.Elem[idx]) {
				return
			}
		}
	}
}

func (t *BTree[T]) insertNode(nd *BNode[T], e T) *splitNode[T] {
	if nd == nil {
		return &splitNode[T]{elem: e}
	}

	gtIndex, eq := t.getGEIndex(nd, e)
	if eq {
		nd.Elem[gtIndex] = e
		t.insert = 0
		return nil
	}

	if len(nd.Child) == 0 {
		nd.insertElem(gtIndex, e)
	} else {
		if split := t.insertNode(nd.Child[gtIndex], e); split != nil {
			nd.insertChild(gtIndex, split.lnd, split.rnd)
			nd.insertElem(gtIndex, split.elem)
		}
	}

	if len(nd.Elem) == 2*t.Degree {
		var lch, rch []*BNode[T]
		if len(nd.Child) > 0 {
			lch, rch = nd.Child[:t.Degree+1], nd.Child[t.Degree+1:]
		}
		return &splitNode[T]{
			lnd:  newBNode(nd.Elem[:t.Degree], lch),
			rnd:  newBNode(nd.Elem[t.Degree+1:], rch),
			elem: nd.Elem[t.Degree],
		}
	}

	return nil
}

func (t *BTree[T]) deleteNode(nd *BNode[T], e T) {
	t.delete = 0

	t.Size += t.delete
}

func (t *BTree[T]) getGEIndex(nd *BNode[T], e T) (idx int, eq bool) {
	for i, ele := range nd.Elem {
		if cmpRes := t.Cmp(ele, e); cmpRes == 0 {
			return i, true
		} else if cmpRes > 0 {
			return i, false
		}
	}
	return len(nd.Elem), false
}

func (nd *BNode[T]) insertElem(i int, e T) {
	nd.Elem = append(nd.Elem, e)
	for idx := len(nd.Elem) - 1; idx > i; idx-- {
		nd.Elem[idx] = nd.Elem[idx-1]
	}
	nd.Elem[i] = e
}

func (nd *BNode[T]) insertChild(i int, lnd, rnd *BNode[T]) {
	nd.Child = append(nd.Child, rnd)
	for idx := len(nd.Child) - 1; idx > i; idx-- {
		nd.Child[idx] = nd.Child[idx-1]
	}
	nd.Child[i] = lnd
	nd.Child[i+1] = rnd
}
