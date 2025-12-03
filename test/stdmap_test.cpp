#include <cstdio>
#include <ctime>
#include <map>
#include <cstdlib>

int main(int argc, char **argv){
	srand(time(NULL));
	int t_sz = 100;
  if(argc>=2){
      t_sz = strtol(argv[1], NULL, 10);
	}	

	std::map<int, int> _mp;
	for(int i=0;i<t_sz;i++){
		int rkey=rand();
		_mp[rkey]=i;
	}

	return 0;
}