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

    kv *kvs = makekvs(opt_cnt);
    for(int i=0;i<opt_cnt;i++){ 
        // printf("<%s : %s>\n", kvs[i].k, kvs[i].v);
        _stdssmap[std::string(kvs[i].k)] = std::string(kvs[i].v);
    }
    
    int getops=0,delops=0,putops=0;
    for(int i=0;i<opt_cnt;i++){
        int opt_code = rand() % 10;
        // printf("opt_code=%d\n", opt_code);
        if(opt_code>=0&&opt_code<5){ //put
            putops++;
            int idx = rand()%opt_cnt;
            _stdssmap[std::string(kvs[idx].k)] = std::string(kvs[idx].v);
        } else if (opt_code>=5&&opt_code<=7){ //del
            delops++;
            if(rand()%2==0){
                int idx = rand()%opt_cnt;
                _stdssmap.erase(std::string(kvs[idx].k));
            } else {
                char *_tmps=rand_str(rand()%15);
                _stdssmap.erase(std::string(_tmps));
                free(_tmps);
            }
        } else {
            getops++;
            if(rand()%2==0){
                int idx = rand()%opt_cnt;
								_stdssmap[kvs[idx].k];
            } else {
                char *_tmps=rand_str(rand()%15);
								_stdssmap[_tmps];
                free(_tmps);
            }
        }
    }
    
    printf(
        "tot_siz=%d, ops_cnt=%d, get_ops=%d, del_ops=%d, put_ops=%d\n", 
        opt_cnt, opt_cnt, getops, delops, putops 
    );

    return 0;
}