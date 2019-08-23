# Mastering Algorithms with C and Go!

[![CircleCI]](https://circleci.com/gh/keinohguchi/workflows/algo)

[CircleCI]: https://circleci.com/gh/keinohguchi/workflows/algo

## Programs

- [factorial.c]: Factorial example
  - [factorial_test.c]: [factorial.c] unit test
- [issort.c]: Insertion sort example
  - [issort_test.c]: [issort.c] unit test
- [list.c], [list.go], [list.asm]: Singly linked list example
  - [list_test.c], [list_test.go]: [list.c], [list.go] unit test

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
```

Happy Hacking!

[factorial.c]: factorial.c
[factorial_test.c]: factorial_test.c
[issort.c]: issort.c
[issort_test.c]: issort_test.c
[list.c]: list.c
[list.go]: list.go
[list.asm]: list.asm
[list_test.c]: list_test.c
[list_test.go]: list_test.go
