all: tcp_srvd tcp_clid

cplargs=-Wunused-result -O2

VPATH = tcpcli:tcplib:tcpsrv:test
vpath %.o

%.o: %.c
	$(CC) -c $(cplargs) -o $@ $<

tcp_srvd: tcp_srv.o tcp_lib.o skt_select.o skt_fdset.o
	$(CC) $(cplargs) -o $@ $^

tcp_clid: tcp_cli.o
	$(CC) $(cplargs) -o $@ $^

tcp_srv.o: tcpsrv/tcp_srv.c
tcp_cli.o: tcpcli/tcp_cli.c
net_util.o: tcplib/net_util.c
skt_fdset.o: tcplib/skt_fdset.c
skt_select.o: tcplib/skt_select.c
tcp_lib.o: tcplib/tcp_lib.c

test: fdset_testd

fdset_testd: fdset_test.o skt_fdset.o
	$(CC) $(cplargs) -o $@ $^

fdset_test.o: test/fdset_test.c

clean:
	rm -rf *.o *.a *.so *.out
	rm -rf tcp_srvd tcp_clid *_test