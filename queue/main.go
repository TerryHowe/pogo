package main

import "fmt"

type Queue struct {
	queue []int
}

func (q *Queue) pop() (error, int) {
	if len(q.queue) < 1 {
		return fmt.Errorf("queue is empty"), -1
	}
	result := q.queue[0]
	q.queue = q.queue[1:]
	return nil, result
}

func (q *Queue) push(val int) {
	q.queue = append(q.queue, val)
}

func main() {
	var tree [][]int = [][]int{{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}}
	var q Queue
	action := func(val int) {
		q.push(val)
	}
	traverse(tree, 0, action)
	for {
		err, val := q.pop()
		if err != nil {
			break
		}
		fmt.Printf("pop=%d\n", val)
	}
}

func traverse(tree [][]int, depth int, action func(int)) {
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
	for j := 0; j < max; j++ {
		action(tree[j][0])
		action(tree[j][1])
		fmt.Printf("%d %d\n", tree[j][0], tree[j][1])
	}
	traverse(tree[max:], depth+1, action)
}
