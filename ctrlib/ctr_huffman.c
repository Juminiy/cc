#include "ctr_huffman.h"
#include "ctr_bheap.h"

#include <stdlib.h>
#include <string.h>

huffman_node* makeHuffmanNode(char ch, int ts, bool leaf, huffman_node *left, huffman_node *right){
	huffman_node *he = (huffman_node*)malloc(sizeof(huffman_node));
	he->ch=ch;
	he->ts=ts;
	he->leaf=leaf;
	he->left=left;
	he->right=right;
	he->code = NULL;
	return he;
}

void freeHuffmanNode(huffman_node *he) {
	if(he->left)
		freeHuffmanNode(he->left);
	if(he->right)
		freeHuffmanNode(he->right);
	free(he->code);
	free(he);
}

int elem_cmp_huffman(elem_t e0, elem_t e1) {
	huffman_node *he0 = cast_elem_typ(e0, huffman_node*);
	huffman_node *he1 = cast_elem_typ(e1, huffman_node*);
	int cmp_res = -(he0->ts - he1->ts);
	if(cmp_res == 0) {
		return -(he0->ch-he1->ch);
	}
	return cmp_res;
}

void huffmanNodeVist(huffman_node *nd, strstrmap code, int *wpl) {
	if(!nd)
		return;
	if(nd->leaf) {
		char chstk[2]={nd->ch,'\0'};
		strstrmapPut(code, chstk, nd->code);
		*wpl += nd->ts * __strlen(nd->code);
	}
	if(nd->left) {
		nd->left->code = __stradd(nd->code, "0");
		huffmanNodeVist(nd->left, code, wpl);
	}
	if(nd->right) {
		nd->right->code = __stradd(nd->code, "1");
		huffmanNodeVist(nd->right, code, wpl);
	}
}

huffman_res huffmanEncode(const char *_src) {
	huffman_res res;
	res.src = __strdup(_src);
	res.dest = NULL;
	res.code = makestrstrmap();
	res.wpl = 0;

	int freqs[128];
	memset(freqs, 0, 128*sizeof(int));
	for(size_t i=0;_src&&_src[i]!='\0';i++){
		freqs[_src[i]]++;	
	}

	bheap bh = makeBHeap(elem_cmp_huffman,NULL);
	for(int ch=0;ch<128;ch++){
		if(freqs[ch]>0){
			bh = bHeapPush(bh, varElemPtr(makeHuffmanNode(ch, freqs[ch],true,NULL,NULL)));
		}
	}

	while(bHeapLen(bh)>=2) {
		huffman_node *he0 = cast_elem_typ(bHeapTop(bh), huffman_node*); bh = bHeapPop(bh);
		huffman_node *he1 = cast_elem_typ(bHeapTop(bh), huffman_node*); bh = bHeapPop(bh);
		bh = bHeapPush(bh, varElemPtr(makeHuffmanNode(0,he0->ts+he1->ts,false,he0,he1)));
	}
	
	res.root = cast_elem_typ(bHeapTop(bh), huffman_node*);
	
	huffmanNodeVist(res.root, res.code, &res.wpl);

	freeBHeap(bh);

	for(size_t i=0;_src&&_src[i]!='\0';i++){
		char chstk[2]={_src[i],'\0'};
		res.dest = __strcat(res.dest, strstrmapGet(res.code, chstk));
	}

	return res;
}

char* huffmanDecode(huffman_res _res) {
	huffman_node *nd = _res.root;
	char *src=NULL;
	for(size_t i=0;_res.dest&&_res.dest[i]!='\0';i++){
		if(_res.dest[i]=='0') {
			nd = nd->left;
		} else if(_res.dest[i]=='1'){
			nd = nd->right;
		}

		if(nd->leaf){
			char chstk[2]={nd->ch,'\0'};
			src = __strcat(src, chstk);
			nd = _res.root;
		}
	}
	return src;
}

void freeHuffmanRes(huffman_res _res) {
	free(_res.src);
	free(_res.dest);
	freeHuffmanNode(_res.root);
	freestrstrmap(_res.code);
}