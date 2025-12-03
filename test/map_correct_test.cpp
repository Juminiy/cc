#include "../ctrlib/ctr_map.h"
#include <map>
#include <string>
#include <cstdlib>
#include <ctime>

typedef struct kv {
    char *k, *v;
} kv;

void freekv(kv kvp){
    free(kvp.k), free(kvp.v);
}

char* rand_str(size_t _sz) {
    srand(time(NULL));
    char *_ss = (char*)malloc(sizeof(char)*(_sz+1));
    for(size_t _i=0;_i<_sz;_i++)
        _ss[_i] = '0' + rand()%72;
    _ss[_sz]='\0';
    return _ss;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    int opt_cnt = 0;
    if(argc>=2){
        opt_cnt = std::strtol(argv[1], NULL, 10);
    }

    
    std::map<std::string, std::string> _stdssmap;
    strstrmap _avlmap = makestrstrmap();

    kv kvs[opt_cnt+1];
    for(int i=0;i<opt_cnt;i++){
        kvs[i].k = rand_str(rand()%15);   
        kvs[i].v = rand_str(rand()%15);   
        printf("<%s : %s>\n", kvs[i].k, kvs[i].v);
        _stdssmap[std::string(kvs[i].k)] = std::string(kvs[i].v);
        strstrmapPut(_avlmap, kvs[i].k, kvs[i].v);
    }


    auto get_key = [&_stdssmap, &_avlmap](char *_skey) -> bool{
        size_t _cnt = _stdssmap.count(std::string(_skey));
        char * _val = strstrmapGet(_avlmap, _skey);
        return (_cnt==1&&_val) || (_cnt==0&&!_val);
    };

    #define nvalid_exit(__skey) \
        do { if(!get_key(__skey)) {fprintf(stderr, "ERROR\n"); exit(1);} } while(0)

    
    for(int i=0;i<opt_cnt;i++){
        int opt_code = rand() % 10;
        // printf("opt_code=%d\n", opt_code);
        if(opt_code>=0&&opt_code<5){ //put
            int idx = rand()%opt_cnt;
            _stdssmap[std::string(kvs[idx].k)] = std::string(kvs[idx].v);
            strstrmapPut(_avlmap, kvs[idx].k, kvs[idx].v);
            nvalid_exit(kvs[idx].k);
        } else if (opt_code>=5&&opt_code<=7){
            if(rand()%2==0){
                int idx = rand()%opt_cnt;
                _stdssmap.erase(std::string(kvs[idx].k));
                strstrmapDel(_avlmap, kvs[idx].k);
                nvalid_exit(kvs[idx].k);
            } else {
                char *_tmps=rand_str(rand()%15);
                _stdssmap.erase(std::string(_tmps));
                strstrmapDel(_avlmap, _tmps);
                nvalid_exit(_tmps);
                free(_tmps);
            }
        } else {
            if(rand()%2==0){
                int idx = rand()%opt_cnt;
                nvalid_exit(kvs[idx].k);
            } else {
                char *_tmps=rand_str(rand()%15);
                nvalid_exit(_tmps);
                free(_tmps);
            }
        }

    }

    printf("stdmap_size:%zu, avlmap_size:%zu\n", _stdssmap.size(), strstrmapSize(_avlmap));


    for(int i=0;i<opt_cnt;i++){
        freekv(kvs[i]);
    }

    freestrstrmap(_avlmap);
    return 0;
}