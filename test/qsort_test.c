#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <iso646.h>

typedef struct vtyp {
    int f0,f1;
    char *f2,*f3;
    double f4;
} vtyp;

int vtyp_cmp(const void *v0, const void *v1) {
    vtyp *vv0=(vtyp*)v0, *vv1=(vtyp*)v1;
    int f0=vv0->f0-vv1->f0, f1=vv0->f1-vv1->f1;
    int f2=strncmp(vv0->f2,vv1->f2,3),f3=strncmp(vv0->f3,vv1->f3,3);
    double f4=vv0->f4-vv1->f4;
    return f0!=0?f0:(
        f1!=0?f1:(
            f2!=0)?f2:(f3!=0?f3:(f4>0?+1:-1)
        )
    );
}

char *tmpfname = "/tmp/cur0";
void cleanup(){
    if(remove(tmpfname)){
        fprintf(stderr, "remove file: %s error\n", tmpfname);
    }
}

int main() {

    atexit(cleanup);
    const int n = 6;
    vtyp vs[6] = {
        {13,14,"p","&",2.34},
        {13,12,"r","@",4.33},
        {13,14,"r","x",4.31},
        {13,14,"r","z",3.55},
        {13,14,"r","z",3.11},
        {12,17,"c","A",6.66},
        
    };

    qsort(vs, n, sizeof(vtyp), vtyp_cmp);

    // for(int i=0;i<n;i++){
    //     printf("<%"PRId32",%"PRId32",%s,%s,%.3f>\n",
    //         vs[i].f0,vs[i].f1,vs[i].f2,vs[i].f3,vs[i].f4
    //     );
    // }

    printf("%ld %ld %ld %ld %ld",
        offsetof(vtyp, f0), 
        offsetof(vtyp, f1),
        offsetof(vtyp, f2),
        offsetof(vtyp, f3),
        offsetof(vtyp, f4));

    FILE *ffd=fopen(tmpfname,"w");
    fwrite("11\n\t\r", sizeof(char), 5, ffd);
    fclose(ffd);

    fprintf(stdout, "\n%d", not 1);

    // sleep(5);
    fprintf(stderr, "%s\n", strerror(errno));
    perror("PERROR");
    
    

    return 0;
}