package main

import "fmt"

func main() {
	var tree [][]int = [][]int{{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}}
	fmt.Println("----Preorder----")
	preorder(tree, 0, 0)
	fmt.Println("----Inorder----")
	inorder(tree, 0, 0)
	fmt.Println("----Postorder----")
	postorder(tree, 0, 0)
}

// Preorder traversal
func preorder(tree [][]int, x, y int) {
	idx := ((1 << y) - 1) + x
	if len(tree) > idx {
		x = x << 1
		// fmt.Printf("x=%d y=%d idx=%d\n", x, y, idx)
		fmt.Printf("%d %d\n", tree[idx][0], tree[idx][1])
		preorder(tree, x, y+1)
		preorder(tree, x+1, y+1)
	}
}

// Inorder traversal
func inorder(tree [][]int, x, y int) {
	idx := ((1 << y) - 1) + x
	if len(tree) > idx {
		x = x << 1
		// fmt.Printf("x=%d y=%d idx=%d\n", x, y, idx)
		inorder(tree, x, y+1)
		fmt.Printf("%d %d\n", tree[idx][0], tree[idx][1])
		inorder(tree, x+1, y+1)
	}
}

// Inorder traversal
func postorder(tree [][]int, x, y int) {
	idx := ((1 << y) - 1) + x
	if len(tree) > idx {
		x = x << 1
		// fmt.Printf("x=%d y=%d idx=%d\n", x, y, idx)
		postorder(tree, x, y+1)
		postorder(tree, x+1, y+1)
		fmt.Printf("%d %d\n", tree[idx][0], tree[idx][1])
	}
}
