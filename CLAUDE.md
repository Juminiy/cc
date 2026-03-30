# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a C codebase implementing multiple libraries:
- **libctr**: Container/data structures library (sequential and associative containers)
- **libjson**: Zero-dependency JSON parsing library using libctr
- **libsocket**: TCP socket library (server/client with select/epoll support)
- **test**: Comprehensive unit tests for all components

## Build System

The project uses a Makefile with different compilation modes:

```bash
# Build all tests
make

# Build specific test (e.g., json_test.d)
make json_test.d

# Run built test
./json_test.d

# Clean all build artifacts
make clean
```

### Compilation Flags

- **Debug mode**: `make cplargs="-g -O0 -std=c99 -DIDEBUG"` (for `make json_test.d`)
- **Optimized mode**: Uses `-O2 -std=c99` by default

## Architecture

### libjson (Zero-dependency JSON Library)

The JSON implementation has these key characteristics:
- Uses rbtree from libctr for JSON objects
- Uses barray from libctr for JSON arrays
- Safe string buffer (sbuf) for JSON operations
- Provides JavaScript-like (`json_parse/json_stringify`), Python-like (`json_loads/json_dumps`), and Go-like (`json_unmarshal/json_marshal`) APIs
- Performance optimization notes: Uses perf profiling (`perf record -g ./a.out`, `perf report`)

### libctr (Container Library)

Core data structures include:
- **Sequential**: blist (blocked list), bqueue (blocked queue), bstack (blocked stack), barray (blocked array)
- **Associative**: bstree (binary search tree), avltree, rbtree (red-black), bheap (binary heap)
- **Multiple-associative**: m-bst, m-avl, m-rb
- Other: skiplist, lru/lfu caches, bloom filters, huffman encoding
- Uses `elem_uni` union type for flexible value storage

### libsocket (TCP Socket Library)

Network implementations:
- **TCP Server**: Blocking, non-blocking (select), non-blocking (epoll) modes
- **TCP Client**: Basic client with server-dispatched client-client connections
- **Chat room** support via server dispatching
- IPv4 and IPv6 support

## Testing

Run tests with:
```bash
# Build and run all tests
./script/test_make.sh

# Run JSON-specific tests
./script/test_json.sh

# Performance test example
./skiplist_test.d avl 10000000  # Test AVL tree with 10M elements
./skiplist_test.d skiplist 10000000  # Test skiplist with 10M elements
```

## Debugging

- **GDB**: `gdb ./<test_name>.d -x x.gdbinit`
- **Valgrind**: `valgrind --tool=memcheck --leak-check=full ./<test_name>.d`
- **Performance**: `perf record -g ./a.out` then `perf report`

## Key Implementation Notes

- JSON parser handles edge cases like null characters (`\0`), form feed (`\f`), Unicode characters (`\u2060`)
- Container library uses blocked allocations for better memory efficiency
- Socket library supports both blocking and non-blocking I/O
- All memory allocations are tracked and can be debugged with valgrind