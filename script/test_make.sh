#!/bin/bash

ls="fdset_test.d
poll_test.d
args_test.d
str_test.d
klist_test.d
select_test.d
logf_test.d
mt_test.d
os_test.d
net_test.d
blist_test.d
bdeque_test.d
bstree_test.d
rbtree_test.d
avltree_test.d
map_avl_test.d
map_std_test.d
map_zoo_test.d
luogu_p3369.d
mem_test.d
barray_test.d
bheap_test.d
skiplist_test.d
lru_test.d
lfu_test.d
size_test.d 
gcc_test.d
hash_test.d 
bitmap_test.d
bloom_test.d
pair_test.d 
btree_test.d
huffman_test.d
json_test.d
json_valid.d
"

for dd in $ls;
do
	make $dd
done