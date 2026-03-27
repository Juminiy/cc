#ifndef CTR_SBUF_H
#define CTR_SBUF_H

#include <stddef.h>
#include <stdint.h>

#define _KiB (size_t)(1<<10)
#define _MiB (size_t)(1<<20)
#define _GiB (size_t)(1<<30)

#define SBUF_BLOCK_S0 256
#define SBUF_BLOCK_S1 1*_KiB
#define SBUF_BLOCK_S2 4*_KiB
#define SBUF_BLOCK_S3 8*_KiB
#define SBUF_BLOCK_S4 16*_KiB
#define SBUF_BLOCK_M0 32*_KiB
#define SBUF_BLOCK_M1 64*_KiB
#define SBUF_BLOCK_M2 256*_KiB
#define SBUF_BLOCK_H0 1*_MiB
#define SBUF_BLOCK_H1 4*_MiB
#define SBUF_BLOCK_H2 16*_MiB

// safe string buffer
typedef struct sbuf {
    size_t _siz;
    size_t _cap;
    size_t _alloc_ts;
    size_t _alloc_bs;

    char  *_buf;
} sbuf;

#define sBufSize(_s) (_s->_siz)
#define sBufStr(_s) (_s->_buf)
#define sBufCap(_s) (_s->_cap)

sbuf* makeSBuf(size_t _cap);
void freeSBuf(sbuf* _s);
void sBufAlloc(sbuf* _s, size_t _siz);
void sBufWrite(sbuf* _s, const char *_src);
void sBufWriteFree(sbuf* _s, char *_src);
void sBufWriteChar(sbuf* _s, char _c);
void sBufWriteI64(sbuf* _s, int64_t _i);
void sBufWriteF64(sbuf* _s, double _f);
void sBufMerge(sbuf* _dst, sbuf* _src);
void sBufMergeFree(sbuf* _dst, sbuf* _src);
#define sBufWriteFmt(_s, __format, ...) \
    do { \
        sBufAlloc(_s, SBUF_BLOCK_S0); \
        size_t sz = sprintf(_s->_buf+_s->_siz, __format, __VA_ARGS__); \
        _s->_siz += sz; \
    } while(0)


#endif