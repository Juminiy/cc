package main

import "fmt"

type LRUCache struct {
	KN   map[int]*node
	h, t *node
	sz   int
	C    int
}

func Constructor(capacity int) LRUCache {
	return LRUCache{
		KN: make(map[int]*node, capacity),
		h:  nil, t: nil,
		sz: 0,
		C:  capacity,
	}
}

func (this *LRUCache) Get(key int) int {
	if nd, ok := this.KN[key]; ok {
		nd = this.delNd(nd)
		this.addHead(nd)
		return nd.V
	}
	return -1
}

func (this *LRUCache) Put(key int, value int) {
	var cur *node
	if nd, ok := this.KN[key]; ok {
		nd.V = value
		nd = this.delNd(nd)
		cur = nd
	} else {
		cur = &node{K: key, V: value}
	}
	this.KN[key] = cur
	this.addHead(cur)

	if this.sz > this.C {
		cur = this.delTail()
		delete(this.KN, cur.K)
	}
}

func (this *LRUCache) delHead() *node {
	hd := this.h
	if hd != nil {
		this.h = hd.n
		unlinknext(hd)
		this.sz--
	}
	if this.sz == 0 {
		this.t = nil
	}
	return hd
}

func (this *LRUCache) delTail() *node {
	tl := this.t
	if tl != nil {
		this.t = tl.p
		unlinkprev(tl)
		this.sz--
	}
	if this.sz == 0 {
		this.h = nil
	}
	return tl
}

func (this *LRUCache) delNd(nd *node) *node {
	if this.h == nd {
		return this.delHead()
	} else if this.t == nd {
		return this.delTail()
	}

	ndp, ndn := nd.p, nd.n
	linknode(ndp, ndn)
	this.sz--
	return nd
}

func (this *LRUCache) addHead(nd *node) {
	linknode(nd, this.h)
	this.h = nd
	if this.sz == 0 {
		this.t = nd
	}
	this.sz++
}

func (this *LRUCache) addTail(nd *node) {
	linknode(this.t, nd)
	this.t = nd
	if this.sz == 0 {
		this.h = nd
	}
	this.sz++
}

func linknode(n1, n2 *node) {
	if n1 != nil {
		n1.n = n2
	}
	if n2 != nil {
		n2.p = n1
	}
}

func unlinkprev(n *node) {
	if n != nil && n.p != nil {
		n.p.n = nil
		n.p = nil
	}
}

func unlinknext(n *node) {
	if n != nil && n.n != nil {
		n.n.p = nil
		n.n = nil
	}
}

type node struct {
	p, n *node
	K, V int
}

func printlist(n *node) {
	fmt.Printf("[ ")
	for ; n != nil; n = n.n {
		fmt.Printf("%d ", n.K)
	}
	fmt.Println("]")
}

func main() {
	ca := Constructor(2)
	ca.Put(1, 1) // [(1,1)]
	printlist(ca.h)
	ca.Put(2, 2) // [(2,2),(1,1)]
	printlist(ca.h)
	fmt.Println(ca.Get(1)) // 1, [(1,1),(2,2)]
	printlist(ca.h)
	ca.Put(3, 3) // [(3,3),(1,1)]
	printlist(ca.h)
	fmt.Println(ca.Get(2)) // -1
	printlist(ca.h)
	ca.Put(4, 4) // [(4,4),(3,3)]
	printlist(ca.h)
	fmt.Println(ca.Get(1)) // -1
	fmt.Println(ca.Get(3)) // 3 [(3,3),(4,4)]
	fmt.Println(ca.Get(4)) // 4 [(4,4),(3,3)]
}
