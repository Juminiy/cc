#include "../ctrlib/ctr_map.h"
#include "strstrpair.h"

#include <map>
#include <string>
#include <cstdlib>
#include <ctime>

int main(int argc, char **argv) {
    srand(time(NULL));
    int opt_cnt = 0;
    if(argc>=2){
        opt_cnt = std::strtol(argv[1], NULL, 10);
    }

    std::map<std::string, std::string> _stdssmap;
    strstrmap _avlmap = makestrstrmap();

    kv *kvs = makekvs(opt_cnt);
    for(int i=0;i<opt_cnt;i++){ 
        _stdssmap[std::string(kvs[i].k)] = std::string(kvs[i].v);
        strstrmapPut(_avlmap, kvs[i].k, kvs[i].v);
    }

    printf("stdmap_size:%zu, avlmap_size:%zu\n", _stdssmap.size(), strstrmapSize(_avlmap));

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

    freekvs(kvs, opt_cnt);
    freestrstrmap(_avlmap);
    return 0;
}