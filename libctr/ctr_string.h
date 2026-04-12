#ifndef CTR_STRING_H
#define CTR_STRING_H

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

// safe string function

// safe strlen
#define __strlen(__s) (__s ? strlen(__s): 0)

// safe strdup
static inline char* __strdup(const char *__s) {
	if(!__s) return NULL;
	size_t __ssz = __strlen(__s);
	char *__sdup=(char*)malloc(sizeof(char)*(__ssz+1)); // `__s`+'\0'
	strncpy(__sdup, __s, __ssz);
	__sdup[__ssz] = '\0';
	return __sdup;
}

static inline char* __strndup(const char *__s, size_t __n) {
	if(!__s) return NULL;
	char *__sdup=(char*)malloc(sizeof(char)*(__n+1)); // `__s`+'\0'
	strncpy(__sdup, __s, __n);
	__sdup[__n] = '\0';
	return __sdup;
}

// safe strcmp
static inline int __strcmp(const char *__s1, const char *__s2) {
	if(__s1 && __s2){
		return strcmp(__s1, __s2);
	} else {
		return __strlen(__s1) - __strlen(__s2);
	}
}

static inline char* __char2str(char __ch) {
	char* __s = (char*)malloc(sizeof(char)*2);
	__s[0] = __ch;
	__s[1] = '\0';
	return __s;
}

// @return dst = dst+src, self
// @param dst must not `const char*`, must in heap
static inline char* __strcat(char *dst, const char *src) {
	size_t dstsz=__strlen(dst), srcsz=__strlen(src);
	dst = (char*)realloc(dst, sizeof(char)*(dstsz+srcsz+1));
	strncpy(dst+dstsz,src,srcsz);
	dst[dstsz+srcsz]='\0';
	return dst;
}

static inline char* __strncat(char *dst, const char *src, size_t __n) {
	size_t dstsz=__strlen(dst);
	dst = (char*)realloc(dst, sizeof(char)*(dstsz+__n+1));
	strncpy(dst+dstsz,src,__n);
	dst[dstsz+__n]='\0';
	return dst;
}

// @return dst = dst+src, deepcopy
static inline char* __stradd(char *dst, const char *src) {
	return __strcat(__strdup(dst), src);
}

static inline char* __substr(const char *__s, size_t __pos, size_t __len) {
	size_t __sz = __strlen(__s);
	if(__pos>=__sz||__pos+__len>__sz){
		return NULL;
	}
	char *__dst = (char*)malloc(sizeof(char)*(__len+1));
	strncpy(__dst, __s+__pos, __len);
	__dst[__len] = '\0';
	return __dst;
}

static inline char* __substr2(char * __dst, const char *__s, size_t __pos, size_t __len) {
	size_t __sz = __strlen(__s);
	if(__pos>=__sz||__pos+__len>__sz){
		return NULL;
	}
	strncpy(__dst, __s+__pos, __len);
	__dst[__len] = '\0';
	return __dst;
}

static inline char* __strjoin(char * const *__ss, const char *__sep) {
	if(!__ss||!__ss[0]){
		return NULL;
	}
	char *dst = __strdup(*__ss);
	for(char * const *_s=__ss+1;_s&&*_s;_s++){
		dst = __strcat(dst, __sep);
		dst = __strcat(dst, *_s);
	}
	return dst;
}

static inline size_t __strcount(char *__s, const char *__subs) {
	size_t n=0, subn=__strlen(__subs);
	if(subn==0||__strlen(__s)==0){
		return 0;
	}
	while(__s=strstr(__s, __subs)){
		__s += subn;
		n++;
	}
	return n;
}

static inline bool __strcontain(char *__s, const char *__subs) {
	return strstr(__s, __subs)!=NULL;
}

static inline char* __strrepeat(char *__s, const size_t __n) {
	char *dst=NULL;
	for(size_t i=0;i<__n;i++){
		dst = __strcat(dst,__s);
	}
	return dst;
}

static inline bool __strhasprefix(char *__s, const char *__pref) {
	return strncmp(__s, __pref, __strlen(__pref)) == 0;
}

static inline bool __strhassuffix(char *__s, const char *__suff) {
	size_t sn=__strlen(__s),suffn=__strlen(__suff);
	return sn>=suffn && strncmp(__s+sn-suffn,__suff,suffn);
}

static inline char* __strreplace_n(char *__s, const char *__old, char *__new, size_t __n) {
	size_t oldn=__strlen(__old),newn=__strlen(__new);
	if(__n==0||newn==0||__strcmp(__old,__new)==0){
		return __strdup(__s);
	} else if(oldn==0){
		size_t sn = __strlen(__s);
		size_t repn = __n<(sn+1)?__n:(sn+1), repi = 0;
		size_t dn = sn+repn*newn, cur = 0;
		char *dst = (char*)malloc(sizeof(char)*(dn+1));
		strncpy(dst+cur,__new,newn); cur+=newn;
		for(repi=0;repi<repn-1;repi++){
			dst[cur]=__s[repi]; cur++;
			strncpy(dst+cur,__new,newn); cur+=newn;
		}
		strncpy(dst+cur,__s+repi,sn-repi);
		dst[dn] = '\0';
		return dst;
	}

	char *dst = NULL;
	while(__s&&*__s!='\0'){
		char *old_start = strstr(__s,__old);
		if(__n==0||old_start==NULL){
			dst = __strcat(dst, __s);
			break;
		}
		if(old_start){
			if(old_start-__s>0){
				dst = __strncat(dst, __s, old_start-__s);
			}
			dst = __strcat(dst, __new);
			__s += old_start-__s+oldn;
			__n--;
		}
	}
	return dst;
}

// todo: string split

#endif // CTR_STRING_H