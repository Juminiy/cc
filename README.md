# cc 

## Network TCP C/S
1. TCP Server
   1. Blocking: 1Server-1Client
   2. NonBlocking: by linux-select
   3. NonBlocking: by linux-epoll
   4. IPv4
   5. MaxClientSize
   6. Read from clients, Write to clients

2. TCP Client
   1. Write to server, Read from server
   2. IPv4
   3. Client connect to clients by server dispatching

3. Features Append:
   1. server read/write, client read/write both in both sides.
   2. client-client connects in chat-room mode dispatched by server.
   3. IPv6 Nerwork.
   4. server give the avaliable ipv4 or ipv6 network addresses.

## Debug
1. `gdb ./rbtree_test.d -x rbtree_test.gdbinit`