#ifndef CTR_HUFFMAN_H
#define CTR_HUFFMAN_H

#include "ctr_map.h"

typedef struct huffman_node {
	char ch; bool leaf;
	int ts;
	struct huffman_node *left;
	struct huffman_node *right;
	char *code;
} huffman_node;

typedef struct huffman_res {
	char *src;
	char *dest;
	strstrmap code;
	huffman_node *root;
	int wpl;
} huffman_res;

huffman_res huffmanEncode(const char *_src);
char* huffmanDecode(huffman_res _res);
void freeHuffmanRes(huffman_res _res);

#endif