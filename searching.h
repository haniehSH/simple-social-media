#ifndef STRUCTS
#define STRUCTS
    typedef struct  Liker
    {
        char *username;
        struct Liker *next;
    }liker;

    typedef struct Post
    {
        int postId;
        int likeCount;
        liker *headLiker;
        char *text;
        struct Post *next;
    }post;

    typedef struct User
    {
        char *username;
        char *password;
        post *headPost;
        post *lastPost;
        struct User *next;
    }user;
#endif    

int compareString(char*, char*, int);
user* searchUser(char *, int, user *);
post* searchPost(user *, int);
void info(user *);
void findUser(user *);