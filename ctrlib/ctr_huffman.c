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
	return he;
}

void freeHuffmanNode(huffman_node *he) {
	free(he);
}

int elem_cmp_huffman(elem_t e0, elem_t e1) {
	huffman_node *he0 = cast_elem_typ(e0, huffman_node*);
	huffman_node *he1 = cast_elem_typ(e1, huffman_node*);
	return -(he0->ts - he1->ts);
}

void huffmanTreeVisit(huffman_node *nd, strstrmap code) {
	if(!nd)
		return;
	if(nd->leaf) {
		strstrmapPut(code, __char2str(nd->ch), nd->code);
	}
	if(nd->left) {
		nd->left->code = __strcat(nd->left->code, nd->code);
	}
}

huffman_res huffmanEncode(const char *_src) {
	huffman_res res;
	res.src = _src;
	res.dest = NULL;
	res.code = makestrstrmap();

	int freqs[128];
	memset(freqs, 0, 128*sizeof(int));
	for(size_t i=0;_src&&_src[i]!='\0';i++){
		freqs[_src[i]]++;	
	}

	bheap bh = makeBHeap(elem_cmp_huffman,NULL);
	for(char ch=0;ch<128;ch++){
		if(freqs[ch]>0){
			bh = bHeapPush(bh, varElemPtr(makeHuffmanNode(ch, freqs[ch],true,NULL,NULL)));
		}
	}

	while(bHeapLen(bh)>=2) {
		huffman_node *he0 = cast_elem_typ(bHeapTop(bh), huffman_node*); bh = bHeapPop(bh);
		huffman_node *he1 = cast_elem_typ(bHeapTop(bh), huffman_node*); bh = bHeapPop(bh);
		bh = bHeapPush(bh, varElemPtr(makeHuffmanNode(0,he0->ts+he1->ts,false,he0,he1)));
	}
	
	huffmanTreeVisit(cast_elem_typ(bHeapTop(bh), huffman_node*), res.code);

	return res;
}

char* huffmanDecode(const char *_dest, strstrmap _code) {

}