#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "searching.h"

/*
inputs: first string, second string, length of string(first or second)
outputs: 1 if two strings are equal, 0 if two strngs are not equal
side effects: -

This function compares two strings to determine whether they are equal or not.
*/
int compareString(char* string1, char* string2, int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        if((string1[i] == '\0') || (string2[i] == '\0'))
            break;

        if(string1[i] != string2[i])
            return 0;
    }

    if(string1[i] != string2[i])
        return 0;
    else
        return 1;
} 


/*
inputs: the input username, length of the input username, the first node of the linked list of users
outputs: the found user node(if the desired user exists), NULL(if the desired user does not exist) 
side effects: -

This function takes the entered username and searches the linked list of users to find the desired user.
*/
user* searchUser(char *username, int usernameLen, user *node)
{
    while( node != NULL)
    {
        if(compareString(username, node->username, usernameLen))   /*if the username is found*/
            return node;
        node = node->next;    
    }
    return NULL;
}


/*
inputs: the current user, the desired post ID 
outputs: the previous node of the found post(if the desired post exists), NULL(if the desired post does not exist) 
side effects: -

This function looks for the desired post ID in the linked list of the current user's posts.
*/
post* searchPost(user *curUser, int postID)
{
    post *postNode = curUser->headPost;

    while(postNode->next != NULL)
    {
        if((postNode->next)->postId == postID)
            return postNode;
        postNode = postNode->next;
    }
    return NULL;
}


/*
inputs: the current user
outputs: -
side effects: -

This function gets the <info> command and if the format of the input command is correct, it prints the current user information.
*/
void info(user *curUser)
{
    post *userPost;
    if(getchar() != 10)
    {
        printf("please enter the correct format.\n");
        return;
    }

    printf("----------------------------------------------------\n");
    printf("username: %s\n", curUser->username);
    printf("password: %s\n", curUser->password);
    printf("<post id>\t\t<like count>\t\t<post text>\n");
    userPost = curUser->headPost->next;
    while(userPost != NULL)
    {
        printf("%d\t\t\t%d\t\t\t%s\n", userPost->postId, userPost->likeCount, userPost->text);
        userPost = userPost->next;
    }
    printf("----------------------------------------------------\n");
}


/*
inputs: the first node of the linked list of users
outputs: -
side effects: -

This function gets the <find_user> command and if the format of the input command is correct, it prints the desired user information.
*/
void findUser(user *firstUser)
{
    char *username, inputChar;
    int usernameLen = 0;
    user *foundUser;
    post *userPost;

    if(getchar() != 32)
    {
        printf("please enter the correct format.\n");
        return;
    }

    username = (char*) malloc(sizeof(char));
    if(username == NULL)
    {
        printf("out of memory!\n");
        return;
    }

    while ((inputChar = getchar()) > 32)    /*get the username character by character*/
    {
        *(username + usernameLen) = inputChar;
        usernameLen++;

        username = (char*) realloc(username, sizeof(char) * (usernameLen + 1));

        if(username == NULL)
        {
            printf("out of memory!\n");
            return;
        }
    }

    *(username + usernameLen) = '\0';

    if(inputChar != 10)
    {
        printf("please enter the correct format.\n");
        return;
    }

    foundUser = searchUser(username, usernameLen, firstUser);
    if(foundUser == NULL)
    {
        printf("this user does not exist.\n");
        return;
    }

    printf("----------------------------------------------------\n");
    printf("username: %s\n", foundUser->username);
    printf("<post id>\t\t<like count>\t\t<post text>\n");
    userPost = foundUser->headPost->next;
    while(userPost != NULL)
    {
        printf("%d\t\t\t%d\t\t\t%s\n", userPost->postId, userPost->likeCount, userPost->text);
        userPost = userPost->next;
    }
    printf("----------------------------------------------------\n");
    return;
}