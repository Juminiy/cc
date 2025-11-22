all: tcpsrvd tcpclid

cplargs=-Wunused-result -O2

tcpsrvd: tcpsrv/tcp_srv.c
	gcc $(cplargs) -o $@ $<

tcpclid: tcpcli/tcp_cli.c
	gcc $(cplargs) -o $@ $<

clean:
	rm -rf *.out tcpsrvd tcpclid