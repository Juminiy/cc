optargs=-Wunused-result -O2 -std=c99
debugargs=-g -O0 -std=c99 -DIDEBUG
cplargs=$(debugargs)

VPATH = libctr:libjson:libsocket/tcp:test
vpath %.o

%.o: %.c
	$(CC) -c -o $@ $< $(cplargs)

%.d: %.c
	$(CC) -o $@ $^ $(cplargs)

%.d: %.cpp
	g++ -std=c++11 -o $@ $^

all:

# libsocket/tcp
tcp_srv.d: libsocket/tcp/tcp_srv.c tcp_lib.o skt_select.o skt_fdset.o skt_epoll.o net_util.o ctr_blist.o
tcp_cli.d: libsocket/tcp/tcp_cli.c util.o net_util.o ctr_blist.o
skt_select.o: libsocket/tcp/skt_select.c
skt_epoll.o: libsocket/tcp/skt_epoll.c
skt_fdset.o: libsocket/tcp/skt_fdset.c
tcp_lib.o: libsocket/tcp/tcp_lib.c
net_util.o: libsocket/tcp/net_util.c
util.o: libsocket/tcp/util.c

# libctr
ctr_blist.o: libctr/ctr_blist.c
ctr_bstack.o: libctr/ctr_bstack.c
ctr_bqueue.o: libctr/ctr_bqueue.c
ctr_tree_bs.o: libctr/ctr_tree_bs.c
ctr_tree_rb.o: libctr/ctr_tree_rb.c
ctr_tree_avl.o: libctr/ctr_tree_avl.c
ctr_tree_mavl.o: libctr/ctr_tree_mavl.c
ctr_map.o: libctr/ctr_map.c
ctr_barray.o: libctr/ctr_barray.c
ctr_bheap.o: libctr/ctr_bheap.c
ctr_skiplist.o: libctr/ctr_skiplist.c
ctr_lru.o: libctr/ctr_lru.c
ctr_lfu.o: libctr/ctr_lfu.c
ctr_hash.o: libctr/ctr_hash.c
ctr_bitmap.o: libctr/ctr_bitmap.c
ctr_bloom.o: libctr/ctr_bloom.c
ctr_btree.o: libctr/ctr_btree.c
ctr_huffman.o: libctr/ctr_huffman.c
ctr_seq.a: ctr_blist.o ctr_bstack.o ctr_bqueue.o ctr_barray.o ctr_bheap.o
	ar rcs $@ $^
ctr_tree.a: ctr_tree_bs.o ctr_tree_rb.o ctr_tree_avl.o ctr_tree_mavl.o
	ar rcs $@ $^
ctr_sbuf.o: libctr/ctr_sbuf.c

# libjson
json.o: libjson/json.c
	$(CC) -c -o $@ $< $(cplargs) -DJSON_OBJECT_RBTREE
json_encode.o: libjson/json_encode.c
json_decode.o: libjson/json_decode.c
json_parse.o: libjson/json_parse.c
json_path.o: libjson/json_path.c
json.a: json.o json_encode.o json_decode.o json_parse.o json_path.o
	ar rcs $@ $^

test: 

strstrpair.o: test/strstrpair.c
fdset_test.d: test/fdset_test.c skt_fdset.o
poll_test.d: test/poll_test.c
args_test.d: test/args_test.c
str_test.d: test/str_test.c ctr_sbuf.o
klist_test.d: test/klist_test.c
select_test.d: test/select_test.c
logf_test.d: test/logf_test.c
mt_test.d: test/mt_test.c
os_test.d: test/os_test.c
net_test.d: test/net_test.c net_util.o ctr_blist.o
blist_test.d: test/blist_test.c ctr_blist.o
bdeque_test.d: test/bdeque_test.c ctr_seq.a
bstree_test.d: test/bstree_test.c ctr_tree.a ctr_seq.a 
	$(CC) -o $@ $^ $(cplargs) -lm
rbtree_test.d: test/rbtree_test.c ctr_tree.a ctr_seq.a
	$(CC) -o $@ $^ $(cplargs) -lm
avltree_test.d: test/avltree_test.c ctr_tree.a ctr_seq.a 
	$(CC) -o $@ $^ $(cplargs) -lm
map_avl_test.d: test/map_avl_test.c   strstrpair.o ctr_map.o ctr_tree.a ctr_seq.a
map_std_test.d: test/map_std_test.cpp strstrpair.o
map_zoo_test.d: test/map_zoo_test.cpp strstrpair.o ctr_map.o ctr_tree.a ctr_seq.a
luogu_p3369.d: test/luogu_p3369.c ctr_tree.a ctr_seq.a
mem_test.d: test/mem_test.c
barray_test.d: test/barray_test.c ctr_seq.a
bheap_test.d: test/bheap_test.c ctr_seq.a
skiplist_test.d: test/skiplist_test.c ctr_skiplist.o ctr_tree.a ctr_seq.a
lru_test.d: test/lru_test.c ctr_lru.o ctr_tree.a ctr_seq.a
lfu_test.d: test/lfu_test.c ctr_lfu.o ctr_tree.a ctr_seq.a
size_test.d: test/size_test.c
gcc_test.d: test/gcc_test.c
hash_test.d: test/hash_test.c ctr_hash.o ctr_tree.a ctr_seq.a strstrpair.o
bitmap_test.d: test/bitmap_test.c ctr_bitmap.o
bloom_test.d: test/bloom_test.c ctr_bloom.o ctr_bitmap.o strstrpair.o
pair_test.d: test/pair_test.c
btree_test.d: test/btree_test.c ctr_btree.o ctr_seq.a strstrpair.o
huffman_test.d: test/huffman_test.c ctr_huffman.o ctr_map.o ctr_tree.a ctr_seq.a 
json_test.d: test/json_test.c json.a ctr_tree.a ctr_seq.a ctr_sbuf.o
json_valid.d: test/json_valid.c json.a ctr_tree.a ctr_seq.a ctr_sbuf.o
file_test.d: test/file_test.c

clean:
	rm -rf *.o *.a *.so *.out *.dSYM
	rm -rf *.d