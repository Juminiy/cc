package main

import (
	"container/heap"
	"fmt"
)

type LFUCache struct {
	KN  map[int]*node
	H   hp
	Cap int
	Ts  int
}

func Constructor(capacity int) LFUCache {
	return LFUCache{
		KN:  make(map[int]*node, capacity),
		H:   make(hp, 0, capacity),
		Cap: capacity,
		Ts:  0,
	}
}

//func (this *LFUCache) pushIndex(nd *node) {
//	nd.I = min(len(this.KN), this.Cap-1)
//
//}

func (this *LFUCache) Get(key int) int {
	this.Ts++

	if nd, ok := this.KN[key]; ok {
		heap.Remove(&this.H, nd.I)
		nd.Refs++
		nd.Ts = this.Ts
		heap.Push(&this.H, nd)
		return nd.V
	}
	return -1
}

func (this *LFUCache) Put(key int, value int) {
	this.Ts++

	var cur *node
	if nd, ok := this.KN[key]; ok {
		nd.Refs++
		nd.Ts = this.Ts
		nd.V = value
		heap.Remove(&this.H, nd.I)
		cur = nd
	} else {
		if len(this.KN) == this.Cap {
			rmnd := heap.Pop(&this.H).(*node)
			delete(this.KN, rmnd.K)
		}
		cur = &node{Ts: this.Ts, Refs: 1, K: key, V: value, I: min(len(this.KN), this.Cap-1)}
	}
	this.KN[key] = cur
	heap.Push(&this.H, cur)

}

type hp []*node

func (h hp) Len() int { return len(h) }
func (h hp) Less(i, j int) bool {
	if h[i].Refs == h[j].Refs {
		return h[i].Ts < h[j].Ts
	}
	return h[i].Refs < h[j].Refs
}
func (h hp) Swap(i, j int) {
	h[i].I, h[j].I = h[j].I, h[i].I
	h[i], h[j] = h[j], h[i]
}

func (h *hp) Push(x any) {
	*h = append(*h, x.(*node))
}

func (h *hp) Pop() any {
	old := *h
	n := len(old)
	x := old[n-1]
	*h = old[0 : n-1]
	return x
}

type node struct {
	Ts      int // low is upper
	Refs    int // low is upper
	I, K, V int
}

func test1() {
	ca := Constructor(2)
	ca.Put(1, 1)
	ca.Put(2, 2)
	fmt.Println(ca.Get(1)) // 1
	ca.Put(3, 3)
	fmt.Println(ca.Get(2)) // -1
	fmt.Println(ca.Get(3)) // 3
	ca.Put(4, 4)
	fmt.Println(ca.Get(1)) // -1
	fmt.Println(ca.Get(3)) // 3
	fmt.Println(ca.Get(4)) // 4
}

func test2() {
	ca := Constructor(2)
	ca.Put(2, 1)
	ca.Put(2, 2)
	fmt.Println(ca.Get(2)) // 2
	ca.Put(1, 1)
	ca.Put(4, 1)
	fmt.Println(ca.Get(2)) // 2
}

func test3() {
	ca := Constructor(3)
	ca.Put(1, 1)
	ca.Put(2, 2)
	ca.Put(3, 3)
	ca.Put(4, 4)
	fmt.Println(ca.Get(4)) // 4
	fmt.Println(ca.Get(3)) // 3
	fmt.Println(ca.Get(2)) // 2
	fmt.Println(ca.Get(1)) // -1
	ca.Put(5, 5)
	fmt.Println(ca.Get(1)) // -1
	fmt.Println(ca.Get(2)) // 2
	fmt.Println(ca.Get(3)) // 3
	fmt.Println(ca.Get(4)) // -1
	fmt.Println(ca.Get(5)) // 5
}

func main() {
	test1()
	test2()
	test3()
}
