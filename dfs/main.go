package main

import "fmt"

func main() {
	var tree [][]int = [][]int{{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}}
	dfs(tree, 0, 0)
}

func dfs(tree [][]int, x, y int) {
	idx := ((1 << y) - 1) + ((1 << x) - 1)
	if len(tree) > idx {
		fmt.Printf("x=%d y=%d idx=%d\n", x, y, idx)
		dfs(tree, x, y+1)
		fmt.Printf("%d %d\n", tree[idx][0], tree[idx][1])
		dfs(tree, x+1, y+1)
	}
}
