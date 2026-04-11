#include "ctr_sbuf.h"
#include "ctr_util.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void sBufWriteN(sbuf *_s, const char *_src, size_t _n) {
    size_t newsiz = _s->_siz+_n;
    sBufAlloc(_s, _n);
    strncpy(_s->_buf+_s->_siz, _src, _n);
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

roSBuf readFileAll(const char *_fpath) {
    roSBuf _buf = {._p=NULL,._siz=0,._init_type=ROSBUF_INIT_NONE};

    FILE *_pf = fopen(_fpath,"r");
	if(_pf==NULL){
		return _buf;
    }

    _buf = readStreamAll(_pf);
    fclose(_pf);

    return _buf;
}

roSBuf readStreamAll(FILE *_pf) {
    roSBuf _buf = {._p=NULL,._siz=0,._init_type=ROSBUF_INIT_NONE};

	fseek(_pf, 0, SEEK_END);
	long fsiz = ftell(_pf);
	rewind(_pf);

    char *_content = (char*)malloc(sizeof(char)*(fsiz+1));
    _content[fsiz] = '\0';
	fread(_content, 1, fsiz, _pf);

    _buf._p = _content;
    _buf._siz = fsiz;
    _buf._init_type = ROSBUF_INIT_ALLOC;

	return _buf;
}

roSBuf readFdAll(const int _fd) {
    roSBuf _buf = {._p=NULL,._siz=0,._init_type=ROSBUF_INIT_NONE};
    sbuf *_sbuf = makeSBuf(_KiB);

    char _stkbuf[_KiB*4];
    ssize_t rd_sz = 0;

    while(1){
        rd_sz=read(_fd, _stkbuf, _KiB*4);
        if(rd_sz==-1){
            freeSBuf(_sbuf);
            return _buf;
        } else if(rd_sz==0){
            break;
        } else {
            sBufWriteN(_sbuf, _stkbuf, rd_sz);
        }
    }
    _buf._init_type = ROSBUF_INIT_ALLOC;
    _buf._p = sBufStr(_sbuf);
    _buf._siz = sBufSize(_sbuf);
    free(_sbuf);

    return _buf;
}

void freeROSBuf(roSBuf _buf) {
    switch (_buf._init_type) {
        case ROSBUF_INIT_ALLOC:
            free(_buf._p);
        break;
    }
}