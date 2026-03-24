#include "ctr_sbuf.h"
#include "ctr_util.h"

#include <stdlib.h>
#include <string.h>

sbuf* makeSBuf(size_t _cap) {
    if(_cap < 0){
        _cap = 0;
    }

    MALLOC_TYPE(sbuf, buf);
    buf->_siz=0;
    buf->_cap=_cap;
    buf->_buf=(char*)malloc(sizeof(char)*(_cap+1));
    buf->_buf[_cap]='\0';
    buf->_alloc_ts=1;
    buf->_alloc_bs=sizeof(char)*(_cap+1);
    return buf;
}

void freeSBuf(sbuf* _s) {
    free(_s->_buf);
    free(_s);
}

#define __sbuf_ava(_s) (_s->_cap-_s->_siz)

#define __chose_sz(_sz) \
    do { \
        if(_sz<=SBUF_BLOCK_S0){_sz=SBUF_BLOCK_S0;}\
        else if(_sz<=SBUF_BLOCK_S1){_sz=SBUF_BLOCK_S1;}\
        else if(_sz<=SBUF_BLOCK_S2){_sz=SBUF_BLOCK_S2;}\
        else if(_sz<=SBUF_BLOCK_S3){_sz=SBUF_BLOCK_S3;}\
        else if(_sz<=SBUF_BLOCK_S4){_sz=SBUF_BLOCK_S4;}\
        else if(_sz<=SBUF_BLOCK_M0){_sz=SBUF_BLOCK_M0;}\
        else if(_sz<=SBUF_BLOCK_M1){_sz=SBUF_BLOCK_M1;}\
        else if(_sz<=SBUF_BLOCK_M2){_sz=SBUF_BLOCK_M2;}\
        else if(_sz<=SBUF_BLOCK_H0){_sz=SBUF_BLOCK_H0;}\
        else if(_sz<=SBUF_BLOCK_H1){_sz=SBUF_BLOCK_H1;}\
        else if(_sz<=SBUF_BLOCK_H2){_sz=SBUF_BLOCK_H2;}\
        else {}\
    }while(0)

void sBufAlloc(sbuf* _s, size_t _siz) {
    size_t oldsiz = _s->_siz, oldcap = _s->_cap;
    size_t newcap = oldcap;
    size_t ava = __sbuf_ava(_s);
    if (ava < _siz) {
        __chose_sz(_siz);
        newcap += _siz;
        _s->_buf = realloc(_s->_buf, sizeof(char)*(newcap+1));
        _s->_buf[oldsiz] = '\0';
        _s->_buf[newcap] = '\0';
        _s->_alloc_ts+=1;
        _s->_alloc_bs=sizeof(char)*(newcap+1);
    }
    _s->_cap = newcap;
}

void sBufWrite(sbuf* _s, const char *_src) {
    size_t ssz = __strlen(_src);
    size_t newsiz = _s->_siz+ssz;
    sBufAlloc(_s, ssz);
    strncpy(_s->_buf+_s->_siz, _src, ssz);
    _s->_buf[newsiz]='\0';
    _s->_siz = newsiz;
}

void sBufWriteFree(sbuf* _s, char *_src) {
    sBufWrite(_s, _src);
    free(_src);
}

void sBufWriteChar(sbuf* _s, char _c) {
    sBufAlloc(_s, 1);
    _s->_buf[_s->_siz] = _c;
    _s->_buf[_s->_siz+1] = '\0';
    _s->_siz++;
}

void sBufWriteI64(sbuf* _s, int64_t _i) {
    sBufWriteFmt(_s, "%ld", _i);
}

void sBufWriteF64(sbuf* _s, double _f) {
    sBufWriteFmt(_s, "%.6lf", _f);
}

void sBufMerge(sbuf* _dst, sbuf* _src) {
    sBufWrite(_dst, _src->_buf);
}

void sBufMergeFree(sbuf* _dst, sbuf* _src) {
    sBufMerge(_dst, _src);
    freeSBuf(_src);
}