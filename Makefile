optargs=-Wunused-result -O2
debugargs=-g
cplargs=$(debugargs)

VPATH = tcpcli:tcplib:tcpsrv:ctrlib:test
vpath %.o

%.o: %.c
	$(CC) -c $(cplargs) -o $@ $<

%.d: %.c
	$(CC) $(cplargs) -o $@ $^

%.d: %.o
	$(CC) $(cplargs) -o $@ $^

all: tcp_srv.d tcp_cli.d

tcp_srv.d: tcp_srv.o tcp_lib.o skt_select.o skt_fdset.o skt_epoll.o net_util.o ctr_blist.o
	$(CC) $(cplargs) -o $@ $^

tcp_cli.d: tcp_cli.o util.o net_util.o ctr_blist.o
	$(CC) $(cplargs) -o $@ $^

tcp_srv.o: tcpsrv/tcp_srv.c
tcp_cli.o: tcpcli/tcp_cli.c
skt_select.o: tcplib/skt_select.c
skt_epoll.o: tcplib/skt_epoll.c
skt_fdset.o: tcplib/skt_fdset.c
tcp_lib.o: tcplib/tcp_lib.c
net_util.o: tcplib/net_util.c
util.o: tcplib/util.c
ctr_blist.o: ctrlib/ctr_blist.c
ctr_bstack.o: ctrlib/ctr_bstack.c
ctr_bqueue.o: ctrlib/ctr_bqueue.c
ctr_rbtree.o: ctrlib/ctr_rbtree.c
ctr_bstree.o: ctrlib/ctr_bstree.c
ctr_map.o: ctrlib/ctr_map.c

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
bdeque_test.d: test/bdeque_test.c ctr_bstack.o ctr_bqueue.o ctr_blist.o
bstree_test.d: test/bstree_test.c ctr_bstree.o ctr_bstack.o ctr_bqueue.o ctr_blist.o 
rbtree_test.d: test/rbtree_test.c ctr_bstree.o ctr_bstack.o ctr_bqueue.o ctr_blist.o ctr_rbtree.o
map_test.d: test/map_test.c ctr_map.o ctr_bstree.o ctr_bstack.o ctr_bqueue.o ctr_blist.o 

clean:
	rm -rf *.o *.a *.so *.out
	rm -rf *.d