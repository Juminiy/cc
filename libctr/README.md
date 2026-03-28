# Test
## AVLTree VS SkipList
```bash
u@me:~/cc/cc$ time ./skiplist_test.d avl 10000000
test AVLTree
len=9976754, level=28

real    0m20.797s
user    0m20.317s
sys     0m0.480s
u@me:~/cc/cc$ time ./skiplist_test.d skiplist 10000000
test SkipList
len=9976776, level=12

real    0m27.256s
user    0m26.703s
sys     0m0.552s
```