#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

typedef struct {
    int room_id;
    char *room_name;
    char *room_pass;

    int max_fd_sz;
    int max_buf_sz;
    int max_buf_send;
    char **buf;
} chat_room;

typedef struct {
    int user_fd;
    char *user_name;
    
    int max_buf_send;
    char *buf;
} chat_user;

int max_room_sz = 0;

// 
int create_chat_room(
    int max_fd_sz,
    int max_buf_sz,
    int max_buf_send
);



#endif