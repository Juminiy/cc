package main

import (
	"container/heap"
	_ "container/heap"
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

func (this *LFUCache) Get(key int) int {
	if nd, ok := this.KN[key]; ok {
		
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
		cur = nd
	} else {
		cur = &node{Ts: this.Ts, Refs: 1, K: key, V: value}
	}

	heap.Push(&this.H, cur)
	this.KN[key] = cur

	if len(this.KN) > this.Cap {
		cur = heap.Pop(&this.H).(*node)
		delete(this.KN, cur.K)
	}
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

func (h *hp) Top() any {
	return (*h)[len(*h)-1]
}

type node struct {
	Ts   int // low is upper
	Refs int // low is upper
	K, V int
}

func main() {
	ca := Constructor(2)
	ca.Put(1, 1)           //
	ca.Put(2, 2)           //
	fmt.Println(ca.Get(1)) // 1
	ca.Put(3, 3)
	fmt.Println(ca.Get(2))
	fmt.Println(ca.Get(3))
	ca.Put(4, 4)
	fmt.Println(ca.Get(1))
	fmt.Println(ca.Get(3))
	fmt.Println(ca.Get(4))
}
