#ifndef CTR_FILE_H
#define CTR_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "ctr_string.h"

// safe file/dir function
static inline char* __readstream(FILE *_pf) {
	fseek(_pf, 0, SEEK_END);
	long fsiz = ftell(_pf);
	rewind(_pf);

	char *buf = (char*)malloc(sizeof(char)*(fsiz+1));
	fread(buf, 1, fsiz, _pf);
	buf[fsiz]='\0';
	return buf;
}

static inline char* __readfile(const char *__path) {
	FILE *_pf = fopen(__path,"r");
	if(_pf==NULL){
		return NULL;
	}

	char *buf = __readstream(_pf);
	fclose(_pf);
	return buf;
}

static inline void __writetruncfile(const char *__path, const char *__s) {
	int _fd = open(__path, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	write(_fd, __s, __strlen(__s));
	close(_fd);
}

static inline void __writeappendfile(const char *__path, const char *__s) {
	int _fd = open(__path, O_WRONLY|O_CREAT|O_APPEND, 0666);
	write(_fd, __s, __strlen(__s));
	close(_fd);
}

static inline int __remove(const char *__filename) {
    struct stat _fstat;
    if(stat(__filename, &_fstat)==0 && S_ISREG(_fstat.st_mode)){
        return remove(__filename);
    }
    return 0;
}

static inline int __mkdir(const char *__path, mode_t __mode) {
    struct stat _fstat;
    if(stat(__path, &_fstat)){
        return mkdir(__path, __mode);
    }
    return 0;   
}

static inline int __rmdir(const char *__path) {
    struct stat _fstat;
    if(stat(__path, &_fstat)==0 && S_ISDIR(_fstat.st_mode)){
        return rmdir(__path);
    }
    return 0;
}

#endif // CTR_FILE_H