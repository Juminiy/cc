package main

import (
	"os"
	"strconv"
)

func main() {
	sz, _ := strconv.Atoi(os.Args[1])
	arr := make([]int, 0, 0)
	cx := cap(arr)
	for i := 0; i < sz; i++ {
		arr = append(arr, i)
		cpx := cap(arr)
		if cpx > cx {
			println("size=", len(arr), ", cap=", cap(arr))
		}
		cx = cpx
	}
}
