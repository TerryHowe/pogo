package main

import "fmt"

func main() {
	var tree [][]int = [][]int{{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}}
	traverse(tree, 0, 2)
}

func traverse(tree [][]int, depth, swap int) {
	max := 1 << depth
	fmt.Printf("Level %d max=%d\n", depth, max)
	if len(tree) < max {
		if len(tree) == 0 {
			fmt.Println("success")
			return
		}
		fmt.Printf("ERROR short len(tree)=%d  max=%d\n", len(tree), max)
		return
	}
	//fmt.Printf("depth=%d swap=%d\n", depth, swap)
	if depth == swap {
		for left := 0; left < ((max + 1) / 2); left++ {
			right := max - left - 1
			//fmt.Printf("swap left=%d right=%d\n", left, right)
			save0 := tree[right][0]
			save1 := tree[right][1]
			//fmt.Printf("swap save0=%d save0=%d\n", save0, save1)
			tree[right][0] = tree[left][0]
			tree[right][1] = tree[left][1]
			//fmt.Printf("new right zero=%d one=%d\n", tree[right][0], tree[right][1])
			tree[left][0] = save0
			tree[left][1] = save1
		}
	}
	for j := 0; j < max; j++ {
		fmt.Printf("%d %d\n", tree[j][0], tree[j][1])
	}
	// fmt.Println(tree[max:])
	traverse(tree[max:], depth+1, swap)
}
