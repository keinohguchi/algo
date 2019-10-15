# Mastering Algorithms with C

[![CircleCI]](https://circleci.com/gh/keithnoguchi/workflows/algo)

[CircleCI]: https://circleci.com/gh/keithnoguchi/workflows/algo

## Examples

- [factorial.c]: Factorial example
  - [factorial_test.c]: [factorial.c] unit test
- [issort.c]: Insertion sort example
  - [issort_test.c]: [issort.c] unit test
- [list.c], [list.go], [list.asm]: Linked list example
  - [list_test.c], [list_test.go]: [list.c], [list.go] unit test
- [stack.c]: Stack example
  - [stack_test.c]: [stack.c] unit test
- [queue.c]: Queue example
  - [queue_test.c]: [queue.c] unit test
- [set.c]: Set example
  - [set_test.c]: [set.c] unit test
- [hash.c]: Hash table example
  - [hash_test.c]: [hash.c] unit test
- [tree.c]: Binary tree example
  - [tree_test.c]: [tree.c] unit test
- [avl.c]: AVL tree example
  - [avl_test.c]: [avl.c] unit test
- [heap.c]: Heap/Priority queue example
  - [heap_test.c]: [heap.c] unit test

## Tests

```sh
$ make test | tail
    --- PASS: TestRemoveHeadString/4_out_of_5_removal (0.00s)
    --- PASS: TestRemoveHeadString/5_out_of_5_removal (0.00s)
PASS
ok      command-line-arguments  1.011s
ok      command-line-arguments  0.002s  coverage: 90.3% of statements
C tests
=======
factorial_test: PASS
issort_test:    PASS
list_test:      PASS
stack_test:     PASS
queue_test:     PASS
set_test:       PASS
hash_test:      PASS
tree_test:      PASS
avl_test:       PASS
heap_test:      PASS
```

Happy hacking!

[factorial.c]: factorial.c
[factorial_test.c]: factorial_test.c
[issort.c]: issort.c
[issort_test.c]: issort_test.c
[list.c]: list.c
[list.go]: list.go
[list.asm]: list.asm
[list_test.c]: list_test.c
[list_test.go]: list_test.go
[stack.c]: stack.c
[stack_test.c]: stack_test.c
[queue.c]: queue.c
[queue_test.c]: queue_test.c
[set.c]: set.c
[set_test.c]: set_test.c
[hash.c]: hash.c
[hash_test.c]: hash_test.c
[tree.c]: tree.c
[tree_test.c]: tree_test.c
[avl.c]: avl.c
[avl_test.c]: avl_test.c
[heap.c]: heap.c
[heap_test.c]: heap_test.c
