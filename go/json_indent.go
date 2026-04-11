package main

import (
	"encoding/json"
	"fmt"
)

func main() {
	bs, err := json.MarshalIndent(map[string]any{
		"k1": map[string]any{
			"k2": []any{
				map[string]any{
					"k3": []any{
						"v3",
						true, false, nil,
						map[string]any{"k4": 1, "k5": 2},
						[]int{1, 4, 5},
						[]float32{1.22},
					},
				},
			},
		},
	}, "", "  ")
	if err != nil {
		panic(err)
	}
	fmt.Printf("%s", bs)
}
