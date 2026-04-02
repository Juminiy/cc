package main

import (
	"encoding/json"
	"fmt"
	"os"
	"time"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("argv[1] not found")
		return
	}
	bs, err := os.ReadFile(os.Args[1])
	if err != nil {
		panic(err)
	}

	fmt.Printf("bssize = %dB\n", len(bs))

	t0 := time.Now()
	var val any
	if err := json.Unmarshal(bs, &val); err != nil {
		panic(err)
	}
	fmt.Printf("json decode time: %dms\n", time.Since(t0).Milliseconds())
	// fmt.Printf("%s, %v\n", bs, val)
	t0 = time.Now()
	if _, err := json.Marshal(val); err != nil {
		panic(err)
	}
	fmt.Printf("json encode time: %dms\n", time.Since(t0).Milliseconds())

}
