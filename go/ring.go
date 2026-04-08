package main

import (
	"container/ring"
	"fmt"
)

func main() {
	rng := ring.New(5)
	rng.Value = 1
	fmt.Println(rng.Len())
	rng.Link((&ring.Ring{Value: 10}).Next())
	rng.Move(3)
	rng.Do(func(v any) {
		fmt.Println(v)
	})
}
