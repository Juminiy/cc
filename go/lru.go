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
		C: capacity,
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
		this.delNd(nd)
		cur = nd
	} else {
		cur = &node{K: key, V: value}
	}
	this.KN[key] = cur
	this.addHead(cur)

	if this.sz > this.C {
		this.sz--
	}
}

func (this *LRUCache) delHead() *node {
	if this.h == nil {
		return nil
	}
	hd := this.h
	this.h = this.h.n
	this.sz--
	if this.h == nil {
		this.t = nil
	}

	hd.n = nil
	return hd
}

func (this *LRUCache) delTail() *node {
	if this.t == nil {
		return nil
	}
	tl := this.t
	this.t = this.t.p
	this.sz--
	if this.t == nil {
		this.h = nil
	}

	tl.p = nil
	return tl
}

func (this *LRUCache) delNd(nd *node) *node {
	if this.h == nd {
		this.delHead()
	} else if this.t == nd {
		this.delTail()
	}

	ndp, ndn := nd.p, nd.n
	ndp.n, ndn.p = ndn, ndp
	this.sz--
	return nd
}

func (this *LRUCache) addHead(nd *node) {
	nd.p = nil
	nd.n = this.h
	this.h = nd
	if this.t == nil {
		this.t = nd
	}
	this.sz++
}
func (this *LRUCache) addTail(nd *node) {
	nd.n = nil
	nd.p = this.h
	this.h = nd
	if this.h == nil {
		this.h = nd
	}
	this.sz++
}

type node struct {
	p, n *node
	K, V int
}

func main() {
	ca := Constructor(2)
	ca.Put(1, 1)
	ca.Put(2, 2)
	fmt.Println(ca.Get(1))
	ca.Put(3, 3)
	fmt.Println(ca.Get(2))
	ca.Put(4, 4)
	fmt.Println(ca.Get(1))
	fmt.Println(ca.Get(3))
	fmt.Println(ca.Get(4))
}
