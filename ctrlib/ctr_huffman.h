#ifndef CTR_HUFFMAN_H
#define CTR_HUFFMAN_H

#include "ctr_map.h"

typedef struct huffman_res {
	char *src;
	char *dest;
	strstrmap code;
} huffman_res;

huffman_res huffmanEncode(const char *_src);
char* huffmanDecode(const char *_dest, strstrmap _code);

typedef struct huffman_node {
	char ch; bool leaf;
	int ts;
	huffman_node *left;
	huffman_node *right;
	char *code;
} huffman_node;

#endif