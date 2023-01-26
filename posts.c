#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "posts.h"

/*
inputs: the text of the new post, the lenght of the text, the last node of the linked list of the current user's posts
outputs: the last node of the linked list of posts(if the insertion is successful), NULL(if the insertion is not successful)
side effects: inserting the new node at the end of the linked list of the current user's posts

This function creates a new node with the given text and then inserts this new node at the end of the linked list of the current user's posts.
*/
post* postInsert(char *text, int textLen, post *lastNode)
{
    int prePostID = lastNode->postId;   /*get the ID of last node of the linked list of the current user's posts*/

    lastNode->next = (post*) malloc(sizeof(post));      /*create a new node*/
    if(lastNode->next == NULL)
    {
        printf("out of memory!\n");
        return NULL;
    }

    lastNode = lastNode->next;      /*add the new node at the end of the linked list of users*/

    lastNode->text = (char*) malloc(sizeof(char) * (textLen + 1));
    lastNode->headLiker = (liker*) malloc(sizeof(liker));
    if((lastNode->text == NULL) || (lastNode->headLiker == NULL)) 
    {
        printf("out of memory!\n");
        return NULL;
    }

    memcpy(lastNode->text, text, sizeof(char) * (textLen + 1));      /*Initialize the new node elements*/
    lastNode->likeCount = 0;
    (lastNode->headLiker)->next = NULL;
    lastNode->postId = prePostID + 1;
    lastNode->next = NULL;
    
    return lastNode;
}


/*
inputs: the current user
outputs: -
side effects: inserting the new node at the end of the linked list of the current user's posts (by calls the userInsert function)

This function gets the <post> command and if the format of the input command is correct, it post the new text.
*/
void newPost(user *curUser)
{
   char *text, inputChar;
   int textLen = 0;
   post *lastPost;

   if(getchar() != 32)
    {
        printf("please enter the correct format.\n");
        return;
    }

    text = (char*) malloc(sizeof(char));
    if(text == NULL)
    {
        printf("out of memory!\n");
        return;
    }

    while ((inputChar = getchar()) != 10)   /*get the text character by character*/
    {
        *(text + textLen) = inputChar;
        textLen++;

        text = (char*) realloc(text, sizeof(char) * (textLen + 1));

        if(text == NULL)
        {
            printf("out of memory!\n");
            return;
        } 
    }

    *(text + textLen) = '\0';

    lastPost = postInsert(text, textLen, curUser->lastPost);    /*post the new text*/
    if(lastPost != NULL)
    {
        printf("the new post is added succesfully.\n");
        curUser->lastPost = lastPost;
    }

    free(text);
    return;    
}


/*
inputs: the first node of the linked list of the current user's posts, the current user
outputs: -
side effects: inserting the current user to the list of post likers. 

This function gets the <like> command and if the format of the input command is correct,it adds the current user to the list of post likers.
*/
void like(user *firstNode, user *curUser)
{
    char *username, inputChar;
    int usernameLen = 0, postID;
    user *foundUser;
    post *foundPost;
    liker *preLiker, *curLiker;

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

    if(inputChar != 32)
    {
        printf("please enter the correct format.\n");
        return;
    }

    inputChar = getchar();      /*Check the post id format (post id should only contain numbers)*/
    if((inputChar < 48) || (inputChar > 57))
    {
        printf("please enter the correct postID.\n");
        return;
    }
    ungetc(inputChar, stdin);

    scanf("%d", &postID);   /*get the post id*/

    inputChar = getchar();

    if(inputChar != 10)
    {
        printf("please enter the correct format.\n");
        return;
    }
    
    foundUser = searchUser(username, usernameLen, firstNode);
    if(foundUser == NULL)
    {
        printf("this user does not exist.\n");
        return;
    }
    else if(foundUser == curUser)
    {
        printf("you can't like your own post.\n");
        return;
    }

    foundPost = searchPost(foundUser, postID);
    if(foundPost == NULL)
    {
        printf("this user doesn't have this postID.\n");
        return;
    }
    foundPost = foundPost->next;

    preLiker = foundPost->headLiker;  
    curLiker = preLiker->next;
    while(curLiker != NULL)     /*Checking that the user has not liked the post before*/
    {
        if(compareString(curLiker->username , curUser->username, usernameLen))
        {
            printf("you have already liked this post.\n");
            return;
        }
        preLiker = curLiker;
        curLiker = curLiker->next;   
    }

    preLiker->next = (liker*) malloc(sizeof(liker));    /*create a new node*/
    curLiker = preLiker->next;     
    if(curLiker == NULL)
    {
        printf("out of memory!\n");
        return;
    }

    curLiker->username = (char*) malloc(sizeof(char) * (usernameLen+1));    
    if(curLiker->username == NULL)
    {
        printf("out of memory!\n");
        return;
    }

    strcpy(curLiker->username, curUser->username);      /*add the current user to the list of post likers*/
    curLiker->next = NULL;
    (foundPost->likeCount)++;

    printf("the post is liked succesfully.\n");
    free(username);
    return;
}


/*
inputs: the current user
outputs: -
side effects: removing the node of desired post from linked list of the current user's posts

This function gets the <delete> command and if the format of the input command is correct, it deletes the desired post.
*/
void deletePost(user *curUser)
{
    char inputChar;
    int postID;
    post *foundPost, *deletedPost;
    liker *postLiker, *deletedLiker;


    if(getchar() != 32)
    {
        printf("please enter the correct format.\n");
        return;
    }

    inputChar = getchar();
    if((inputChar < 48) || (inputChar > 57))    /*Check the post id format (post id should only contain numbers)*/
    {
        printf("please enter the correct postID.\n");
        return;
    }
    ungetc(inputChar, stdin);   

    scanf("%d", &postID);   /*get the desired post id */

    inputChar = getchar();

    if(inputChar != 10)
    {
        printf("please enter the correct format.\n");
        return;
    }

    foundPost = searchPost(curUser, postID);
    if(foundPost == NULL)
    {
        printf("this post does not exist.\n");
        return;
    }

    deletedPost = foundPost->next;   
    foundPost->next = deletedPost->next;    /*remove the node of desired post from linked list of the current user's posts*/
    if(foundPost->next == NULL)     /*moving the end of the post list to the previous node if the post we deleted was the last node*/
        curUser->lastPost = foundPost;

    postLiker = deletedPost->headLiker;
    while (postLiker != NULL)   /*free the list of the post likers*/
    {
        deletedLiker = postLiker;
        postLiker = postLiker->next;
        free(deletedLiker);       
    }
    
    free(deletedPost);
    printf("post is deleted successfully.\n");
    return;
}