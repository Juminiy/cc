package main

func main() {
	arr := make([]int, 0, 0)
	for i := 0; i < 1000; i++ {
		arr = append(arr, i)
		println(len(arr), cap(arr))
	}
}
