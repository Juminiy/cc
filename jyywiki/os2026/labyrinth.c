#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>

#include "../../libctr/ctr_util.h"
#include "../../libctr/ctr_union.h"

#define DIR_ERROR -1
#define DIR_NONE 0
#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHT 4

typedef struct lab_map {
    char  *buf;     // raw buf
    size_t buf_siz; // raw buf size
    char  *ch[100]; // map grid
    size_t sz,m,n;  // map_size, map_m, map_n
    int pos[10];    // player position
    char *_err_msg; // debug error message
} lab_map;

int read_map(const char * map_filename, lab_map *_lmap);
bool lab_map_valid(lab_map *_lmap);

int read_map(const char * map_filename, lab_map *_lmap) {
    FILE *ffd = fopen(map_filename, "r");
    if (ffd==NULL){
        _lmap->_err_msg = (char*)malloc(sizeof(char)*100);
        sprintf(_lmap->_err_msg, "open file: %s, error: %s", map_filename, strerror(errno));
        return EXIT_FAILURE;
    }
    char * buf = __readstream(ffd);
    size_t siz = __strlen(buf);
    _lmap->buf = buf;
    _lmap->buf_siz = siz;

    size_t off = 0, curn = 0, curm = 0;
    // read lines
    while(off<siz){
        char *bufptr = strchr(buf,'\n');
        if(bufptr==NULL){
            _lmap->ch[curm] = buf;
            break;
        }
        size_t szn = bufptr-buf;
        if(curn==0){
            curn=szn;
        } else if (curn!=szn){
            _lmap->_err_msg = "map column length invalid";
            return EXIT_FAILURE;
        }
        _lmap->ch[curm] = buf;
        curm++;
        off += (curn+1);
        buf += (curn+1);
    }

    _lmap->m = curm+1;
    _lmap->n = curn;
    _lmap->sz = siz;
    for(int i=0;i<10;i++){
        _lmap->pos[i] = -1;
    }

    fclose(ffd);

    if(!lab_map_valid(_lmap)){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool lab_map_valid(lab_map *_lmap) {
    if(_lmap->m>100||_lmap->n>100||
        _lmap->m==0||_lmap->n==0){
        _lmap->_err_msg = "map too huge";
        return false;
    }

    bool _valid = true;
    union_set *_st = makeUnionSet(_lmap->sz);

    int uset_idx[_lmap->m*_lmap->n], idxi = 0;
    for(size_t i=0;i<_lmap->m;i++){
        for(size_t j=0;j<_lmap->n;j++){
            int idx = i*_lmap->n+j;
            if(_lmap->ch[i][j]=='#') { // Wall
            } else if(isdigit(_lmap->ch[i][j])){ // player
                _lmap->pos[_lmap->ch[i][j]-'0'] = idx;
                uset_idx[idxi++] = idx;
                unionSetPut(_st, idx);
            } else if(_lmap->ch[i][j]=='.'){ // Land
                uset_idx[idxi++] = idx;
                unionSetPut(_st, idx);
            } else {
                // DEBUGF("map invalid pos: (%ld,%ld), char: [%c]",i,j,_lmap->ch[i][j]);
                _lmap->_err_msg = "map invalid character";
                _valid = false;
                goto fin_tag;
            }
        }
    }

    #define uset_merge(_px,_py) \
        do { \
            int _pi = _py/_lmap->n, _pj = _py%_lmap->n; \
            if(_pi>=0&&_pi<_lmap->m \
                &&_pj>=0&&_pj<_lmap->n&& \
                (isdigit(_lmap->ch[_pi][_pj])||_lmap->ch[_pi][_pj]=='.')){ \
                unionSetMerge(_st, _px, _py); \
            } \
        } while(0)
        // DEBUGF("connect(%d %d)",_px,_py);
    for(int i=0;i<idxi;i++){
        int pi=uset_idx[i]/_lmap->n;
        int pj=uset_idx[i]%_lmap->n;
        int lpos = uset_idx[i]-1;
        int rpos = uset_idx[i]+1;
        int upos = (pi-1)*_lmap->n+pj;
        int dpos = (pi+1)*_lmap->n+pj;
        if(pj>0) {
            uset_merge(uset_idx[i],lpos);
        }
        if(pj<_lmap->n-1){
            uset_merge(uset_idx[i],rpos);
        }
        if(pi>0){
            uset_merge(uset_idx[i],upos);
        }
        if(pi<_lmap->m-1){
            uset_merge(uset_idx[i],dpos);
        }
    }

    if(unionSetCnt(_st)!=1){
        _lmap->_err_msg = "map not connected";
        _valid = false;
    }

fin_tag:
    freeUnionSet(_st);
    return _valid;
}

void free_lab_map(lab_map _lmap){
    free(_lmap.buf);
}

bool move_lab_map(lab_map *_lmap, int player_id, char _dir) {
    bool optok = false;
    if(_lmap->pos[player_id]!=-1){ // found
        int pos_i = _lmap->pos[player_id]/_lmap->n;
        int pos_j = _lmap->pos[player_id]%_lmap->n;
        switch (_dir)
        {
        case DIR_LEFT:
            if(pos_j>0&&_lmap->ch[pos_i][pos_j-1]=='.'){
                _lmap->ch[pos_i][pos_j-1]=player_id+'0';
                _lmap->pos[player_id]-=1;
                optok = true;
            }
            break;
        
        case DIR_RIGHT:
            if(pos_j<_lmap->n-1&&_lmap->ch[pos_i][pos_j+1]=='.'){
                _lmap->ch[pos_i][pos_j+1]=player_id+'0';
                _lmap->pos[player_id]+=1;
                optok = true;
            }
            break;
        
        case DIR_UP:
            if(pos_i>0&&_lmap->ch[pos_i-1][pos_j]=='.'){
                _lmap->ch[pos_i-1][pos_j]=player_id+'0';
                _lmap->pos[player_id]-=_lmap->n;
                optok = true;
            }
            break;
        
        case DIR_DOWN:
            if(pos_i<_lmap->m-1&&_lmap->ch[pos_i+1][pos_j]=='.'){
                _lmap->ch[pos_i+1][pos_j]=player_id+'0';
                _lmap->pos[player_id]+=_lmap->n;
                optok = true;
            }
            break;
        }
        if(optok){
            _lmap->ch[pos_i][pos_j] = '.';
        }
    } else { // not found
        // 1. lack pos
        int pos_i=0,pos_j=0;
        for(;pos_i<_lmap->m;pos_i++){
            for(;pos_j<_lmap->n;pos_j++){
                if(_lmap->ch[pos_i][pos_j]=='.'){
                    optok=true;
                    goto posok_tag;
                }
            }
        }
        posok_tag:
        // 2. pos valid
        if(optok){
            _lmap->pos[player_id] = pos_i*_lmap->n+pos_j;
            _lmap->ch[pos_i][pos_j] = player_id+'0';
        }
    }

    return optok;
}

void write_map(const char * map_filename, lab_map *_lmap) {
    FILE *ffd = fopen(map_filename, "w");
    if (ffd==NULL){
        _lmap->_err_msg = (char*)malloc(sizeof(char)*100);
        sprintf(_lmap->_err_msg, "open file: %s, error: %s", map_filename, strerror(errno));
        return EXIT_FAILURE;
    }

    fwrite(_lmap->buf, sizeof(char), _lmap->buf_siz, ffd);
    fclose(ffd);
}

int main(int argc, char *argv[]){
    bool get_version = false;
    char * map_filename = NULL;
    int player_id = -1;
    int direction = DIR_NONE;

    char const * const short_opts = "m:p:M:vh";
    const struct option long_opts[] = {
        {"map", required_argument, NULL, 'm'},
        {"player", required_argument, NULL, 'p'},
        {"move", required_argument, NULL, 'M'},
        {"version", no_argument, NULL, 'v'},
        {"help", no_argument, NULL, 'h'}
    };

    #define HELP() \
        do { \
            fprintf(stdout, "labyrinth [-m|--map FILE] [-p|--player ID] [--move DIRECTION] [--version]\n"); \
        } while(0)

    int opt_case;
    while((opt_case = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1)
    {
        switch (opt_case)
        {
        case 'm':
            map_filename = __strdup(optarg);
        break;

        case 'p':
            player_id = strtol(optarg, NULL, 10);
        break;

        case 'M':
            if(__strcmp(optarg,"up")){
                direction = DIR_UP;
            } else if(__strcmp(optarg,"down")){
                direction = DIR_DOWN;
            } else if(__strcmp(optarg,"left")){
                direction = DIR_LEFT;
            } else if(__strcmp(optarg,"right")){
                direction = DIR_RIGHT;
            } else {
                direction = DIR_ERROR;
            }
        break;

        case 'v':
            get_version = true;
        break;

        case 'h':
            HELP();
            return 0;
        default:
            return 1;
        }
    }
    if(get_version){
        if(map_filename||player_id!=-1||direction!=DIR_NONE){
            return 1;
        }
        fprintf(stdout, "Labyrinth Game v0.0.1\n");
        return 0;
    }

    if(!map_filename){
        DEBUGF("map FILE is null");
        return EXIT_FAILURE;
    }
    if(player_id<0||player_id>9){
        DEBUGF("player_id: %d is invalid", player_id);
        return EXIT_FAILURE;
    }
    if(direction==DIR_ERROR){
        DEBUGF("move direction is invalid");
        return EXIT_FAILURE;
    }

    lab_map _lmap={};
    int res = read_map(map_filename, &_lmap);
    if(res==EXIT_SUCCESS){ // print it
        puts(_lmap.buf);
    } else {
        DEBUGF("%s",_lmap._err_msg);
        return EXIT_FAILURE;
    }

    if(direction!=DIR_NONE){ // move it
        if(move_lab_map(&_lmap, player_id, direction)){
            write_map(map_filename, &_lmap);
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}