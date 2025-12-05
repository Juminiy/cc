optargs=-Wunused-result -O2
debugargs=-g -O0 -lm
memsant=-fsanitize=address
cplargs=$(debugargs)

VPATH = tcpcli:tcplib:tcpsrv:ctrlib:test
vpath %.o

%.o: %.c
	$(CC) -std=c99 -c -o $@ $< $(cplargs)

%.d: %.c
	$(CC) -std=c99 -o $@ $^ $(cplargs)

%.d: %.cpp
	g++ -std=c++11 -o $@ $^

%.d: %.o
	$(CC) -std=c99 -o $@ $^ $(cplargs)

all: tcp_srv.d tcp_cli.d

tcp_srv.d: tcpsrv/tcp_srv.c tcp_lib.o skt_select.o skt_fdset.o skt_epoll.o net_util.o ctr_blist.o
tcp_cli.d: tcpcli/tcp_cli.c util.o net_util.o ctr_blist.o

skt_select.o: tcplib/skt_select.c
skt_epoll.o: tcplib/skt_epoll.c
skt_fdset.o: tcplib/skt_fdset.c
tcp_lib.o: tcplib/tcp_lib.c
net_util.o: tcplib/net_util.c
util.o: tcplib/util.c
ctr_blist.o: ctrlib/ctr_blist.c
ctr_bstack.o: ctrlib/ctr_bstack.c
ctr_bqueue.o: ctrlib/ctr_bqueue.c
ctr_tree_bs.o: ctrlib/ctr_tree_bs.c
ctr_tree_rb.o: ctrlib/ctr_tree_rb.c
ctr_tree_avl.o: ctrlib/ctr_tree_avl.c
ctr_tree_mavl.o: ctrlib/ctr_tree_mavl.c
ctr_map.o: ctrlib/ctr_map.c
strstrpair.o: test/strstrpair.c
ctr_seq.a: ctr_blist.o ctr_bstack.o ctr_bqueue.o
	ar rcs $@ $^
ctr_tree.a: ctr_tree_bs.o ctr_tree_rb.o ctr_tree_avl.o ctr_tree_mavl.o
	ar rcs $@ $^

test: 

fdset_test.d: test/fdset_test.c skt_fdset.o
poll_test.d: test/poll_test.c
args_test.d: test/args_test.c
str_test.d: test/str_test.c
klist_test.d: test/klist_test.c
select_test.d: test/select_test.c
logf_test.d: test/logf_test.c
mt_test.d: test/mt_test.c
os_test.d: test/os_test.c
net_test.d: test/net_test.c net_util.o ctr_blist.o
blist_test.d: test/blist_test.c ctr_blist.o
bdeque_test.d: test/bdeque_test.c ctr_seq.a
bstree_test.d: test/bstree_test.c ctr_tree.a ctr_seq.a 
rbtree_test.d: test/rbtree_test.c ctr_tree.a ctr_seq.a
avltree_test.d: test/avltree_test.c ctr_tree.a ctr_seq.a 
map_avl_test.d: test/map_avl_test.c   strstrpair.o ctr_map.o ctr_tree.a ctr_seq.a
map_std_test.d: test/map_std_test.cpp strstrpair.o
map_zoo_test.d: test/map_zoo_test.cpp strstrpair.o ctr_map.o ctr_tree.a ctr_seq.a
luogu_p3369.d: test/luogu_p3369.c ctr_tree.a ctr_seq.a

clean:
	rm -rf *.o *.a *.so *.out *.dSYM
	rm -rf *.d