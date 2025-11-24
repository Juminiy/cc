#include "../thirdparty/uthash/uthash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct t_user {
    int user_id;
    char *name;
    UT_hash_handle hh;
} t_user;

void add_user(t_user *_users, t_user *_user) {
    HASH_ADD_INT(_users, user_id, _user);
}

t_user* get_user(t_user *_users, int user_id) {
    t_user *_user;
    HASH_FIND_INT(_users, &user_id, _user);
    printf("%p\n", _user);
    return _user;
}

void del_user(t_user *_users, t_user *_user) {
    HASH_DEL(_users, _user);
}

int main() {

    t_user *users=(t_user*)malloc(sizeof(t_user)*4);


    printf("not in get\n");

    t_user *user = get_user(users, 10);
    if(user==NULL) {
        printf("id:%d NOT FOUND\n",10);
    } else {
        printf("id:%d FOUND\n", user->user_id);
    }

    user = (t_user*)malloc(sizeof(t_user));
    strcpy(user->name, "Chisato");
    user->user_id = 10;
    add_user(users, user);

    printf("add, in get\n");

    user = get_user(users, 10);
    if(user==NULL) {
        printf("id:%d NOT FOUND\n",10);
    } else {
        printf("id:%d FOUND\n", user->user_id);
    }

    del_user(users, user);

    printf("after del, not in get\n");

    user = get_user(users, 10);
    if(user==NULL) {
        printf("id:%d NOT FOUND\n",10);
    } else {
        printf("id:%d FOUND\n", user->user_id);
    }

    free(users);


    return 0;
}