package main

import (
	"encoding/json"
	"fmt"
	"os"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("argv[1] not found")
		return
	}
	bs, err := os.ReadFile(os.Args[1])
	if err != nil {
		fmt.Print("n")
	} else {
		var val any
		if err := json.Unmarshal(bs, &val); err != nil {
			fmt.Print("n")
		} else {
			fmt.Print("y")
		}
	}

}
