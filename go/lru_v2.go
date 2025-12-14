package main

import (
	"container/list"
	"fmt"
)

type LRUCache struct {
	kvs  map[int]*list.Element
	ls   *list.List
	mcap int
}

// func printlist(ca LRUCache) {
// 	fmt.Printf("[ ")
// 	for e := ca.ls.Front(); e != nil; e = e.Next() {
// 		fmt.Printf("%v ", e.Value)
// 	}
// 	fmt.Printf("] %v \n", ca.kvs)

// }

type kv struct {
	k, v int
}

func Constructor(capacity int) LRUCache {
	return LRUCache{
		kvs:  make(map[int]*list.Element, capacity),
		ls:   list.New(),
		mcap: capacity,
	}
}

func (this *LRUCache) Get(key int) int {
	if nd, ok := this.kvs[key]; ok {
		this.ls.MoveToFront(nd)
		return nd.Value.(kv).v
	}
	return -1
}

func (this *LRUCache) Put(key int, value int) {
	if nd, ok := this.kvs[key]; ok {
		nd.Value = kv{k: key, v: value}
		this.ls.MoveToFront(nd)
	} else {
		if this.ls.Len() == this.mcap {
			bckVal := this.ls.Remove(this.ls.Back())
			delete(this.kvs, bckVal.(kv).k)
		}
		nd = this.ls.PushFront(kv{k: key, v: value})
		this.kvs[key] = nd
	}
}

func test_1() {
	ca := Constructor(2)
	ca.Put(1, 1)           // [(1,1)]
	ca.Put(2, 2)           // [(2,2),(1,1)]
	fmt.Println(ca.Get(1)) // 1, [(1,1),(2,2)]
	ca.Put(3, 3)           // [(3,3),(1,1)]
	fmt.Println(ca.Get(2)) // -1
	ca.Put(4, 4)           // [(4,4),(3,3)]
	fmt.Println(ca.Get(1)) // -1
	fmt.Println(ca.Get(3)) // 3 [(3,3),(4,4)]
	fmt.Println(ca.Get(4)) // 4 [(4,4),(3,3)]
}

func test_2() {
	ca := Constructor(2)
	ca.Put(1, 0)           // [(1,0)]
	ca.Put(2, 2)           // [(2,2),(1,0)]
	fmt.Println(ca.Get(1)) // 1, [(1,1),(2,2)]
	ca.Put(3, 3)           // [(3,3),(1,1)]
	fmt.Println(ca.Get(2)) // -1
	ca.Put(4, 4)           // [(4,4),(3,3)]
	fmt.Println(ca.Get(1)) // -1
	fmt.Println(ca.Get(3)) // 3 [(3,3),(4,4)]
	fmt.Println(ca.Get(4)) // 4 [(4,4),(3,3)]
}

func main() {
	test_1()
	test_2()
}
