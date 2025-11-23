cplargs=-Wunused-result -O2

VPATH = tcpcli:tcplib:tcpsrv:test
vpath %.o

%.o: %.c
	$(CC) -c $(cplargs) -o $@ $<

%.d: %.c
	$(CC) $(cplargs) -o $@ $^

all: tcp_srv.d tcp_cli.d

tcp_srv.d: tcp_srv.o tcp_lib.o skt_select.o skt_fdset.o skt_epoll.o
	$(CC) $(cplargs) -o $@ $^

tcp_cli.d: tcp_cli.o
	$(CC) $(cplargs) -o $@ $^

tcp_srv.o: tcpsrv/tcp_srv.c
tcp_cli.o: tcpcli/tcp_cli.c
skt_select.o: tcplib/skt_select.c
skt_epoll.o: tcplib/skt_epoll.c
skt_fdset.o: tcplib/skt_fdset.c
tcp_lib.o: tcplib/tcp_lib.c
net_util.o: tcplib/net_util.c

test: 

fdset_test.d: fdset_test.o skt_fdset.o
poll_test.d: test/poll_test.c
args_test.d: test/args_test.c
str_test.d: test/str_test.c
klist_test.d: test/klist_test.c
select_test.d: test/select_test.c

fdset_test.o: test/fdset_test.c

clean:
	rm -rf *.o *.a *.so *.out
	rm -rf *.d