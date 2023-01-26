#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "accounts.h"

/*
inputs: the input username, length of the input username, the first node of the linked list of users
outputs: 1 if the input username is invalid, 0 if the input username is valid 
side effects: -

This function takes the entered username and if the username is the same as the keywords of the program or if it has already been selected by another user, it declares it as invalid username.
*/
int isInvalidName(char *username, int usernameLen, user *userNode)
{
    if( compareString(username, "signup", usernameLen) ||
        compareString(username, "login", usernameLen)  ||
        compareString(username, "post", usernameLen)   ||
        compareString(username, "logout", usernameLen) ||
        compareString(username, "delete", usernameLen) ||
        compareString(username, "like", usernameLen)   ||
        compareString(username, "info", usernameLen))       /*A username that is the same as program keywords is invalid*/
        return 1;

    while( userNode != NULL)    /*Checking that the username is not duplicated*/
    {
        if(compareString(username, userNode->username, usernameLen))
            return 1;
        userNode = userNode->next;    
    }        
    return 0;
}


/*
inputs: the input username, length of the input username, the input password, length of the input password, the last node of the linked list of users
outputs: the last node of the linked list of users(if the insertion is successful), NULL(if the insertion is not successful)
side effects: inserting the new node at the end of the linked list of users

This function creates a new node with the given username and password and then inserts this new node at the end of the linked list of users.
*/
user* userInsert(char *username, int usernameLen, char *password, int passwordLen, user *lastNode)
{
    lastNode->next = (user*) malloc(sizeof(user));   /*create a new node*/
    if(lastNode->next == NULL)
    {
        printf("out of memory!\n");
        return NULL;
    }

    lastNode = lastNode->next;      /*add the new node at the end of the linked list of users*/

    lastNode->username = (char*) malloc(sizeof(char) * (usernameLen + 1));
    lastNode->password = (char*) malloc(sizeof(char) * (passwordLen + 1));
    lastNode->headPost = (post*) malloc(sizeof(post));
    if((lastNode->username == NULL) || (lastNode->password == NULL) || (lastNode->headPost == NULL))
    {
        printf("out of memory!\n");
        return NULL;
    }

    memcpy(lastNode->username, username, sizeof(char) * (usernameLen + 1));     /*Initialize the new node elements*/
    memcpy(lastNode->password, password, sizeof(char) * (passwordLen + 1));
    lastNode->headPost->next = NULL;
    lastNode->headPost->postId = 0;
    lastNode->lastPost = lastNode->headPost;
    lastNode->next = NULL;

    return lastNode;
}


/*
inputs: the first node of the linked list of users, the last node of the linked list of users
outputs: the last node of the linked list of users
side effects: inserting the new node at the end of the linked list of users (by calls the userInsert function)

This function gets the <signup> command and if the format of the input command is correct, it sign ups the new user.
*/
user* signup(user *firstNode, user *lastNode)
{
    char *username, *password, inputChar;
    int usernameLen = 0, passwordLen = 0;

    if(getchar() != 32)
    {
        printf("please enter the correct format.\n");
        return lastNode;
    }

    username = (char*) malloc(sizeof(char));
    password = (char*) malloc(sizeof(char));
    if((username == NULL) || (password == NULL))
    {
        printf("out of memory!\n");
        return lastNode;
    }

    while ((inputChar = getchar()) > 32)    /*get the username character by character*/
    {
        *(username + usernameLen) = inputChar;
        usernameLen++;

        username = (char*) realloc(username, sizeof(char) * (usernameLen + 1));

        if(username == NULL)
        {
            printf("out of memory!\n");
            return lastNode;
        }
    }

    *(username + usernameLen) = '\0';

    if(inputChar != 32) 
    {
        printf("please enter the correct format.\n");
        free(username);
        free(password);
        return lastNode;
    }

    if(isInvalidName(username, usernameLen, firstNode))     /*Resist invalid usernames*/
    {
        printf("please enter another username.\n");
        free(username);
        free(password);
        return lastNode;
    }

    while ((inputChar = getchar()) > 32)    /*get the password character by character*/
    {
        *(password + passwordLen) = inputChar;
        passwordLen++;

        password = (char*) realloc(password, sizeof(char) * (passwordLen + 1));

        if(password == NULL)
        {
            printf("out of memory!\n");
            return lastNode;
        }
    }

    *(password + passwordLen) = '\0';

    if((inputChar != 10) || (passwordLen == 0))
    {
        printf("please enter the correct format.\n");
        free(username);
        free(password);
        return lastNode;
    }

    lastNode->next = userInsert(username, usernameLen, password, passwordLen, lastNode);    /*sign up the new user*/
    free(username);
    free(password);

    if(lastNode->next == NULL)  /*If the insertion was not successful, return the current last node*/
        return lastNode;

    printf("signup successfully.\n");    
    return lastNode->next;      /*If the insertion was successful, return the new last node*/
}


/*
inputs: the first node of the linked list of users
outputs: the pointer to the logged in user(if login is successful), NULL(if login is not successful)
side effects: -

This function gets the <login> command and if the format of the input command is correct, the user is logged in.
*/
user* login(user *firstNode)
{
    char *username, *password, inputChar;
    int usernameLen = 0, passwordLen = 0;
    user *foundUser;

    if(getchar() != 32)
    {
        printf("please enter the correct format.\n");
        return NULL;
    }

    username = (char*) malloc(sizeof(char));
    password = (char*) malloc(sizeof(char));
    if((username == NULL) || (password == NULL))
    {
        printf("out of memory!\n");
        return NULL;
    }

    while ((inputChar = getchar()) > 32)    /*get the username character by character*/
    {
        *(username + usernameLen) = inputChar;
        usernameLen++;

        username = (char*) realloc(username, sizeof(char) * (usernameLen + 1));

        if(username == NULL)
        {
            printf("out of memory!\n");
            return NULL;
        }
    }

    *(username + usernameLen) = '\0';

    if(inputChar != 32)
    {
        printf("please enter the correct format.\n");
        return NULL;
    }

    while ((inputChar = getchar()) > 32)    /*get the password character by character*/
    {
        *(password + passwordLen) = inputChar;
        passwordLen++;

        password = (char*) realloc(password, sizeof(char) * (passwordLen + 1));

        if(password == NULL)
        {
            printf("out of memory!\n");
            return NULL;
        }
    }

    *(password + passwordLen) = '\0';

    if(inputChar != 10)
    {
        printf("please enter the correct format.\n");
        return NULL;
    }

    foundUser = searchUser(username, usernameLen, firstNode);   /*Check the existence of the user*/
    if(foundUser == NULL)
    {
        printf("the username is wrong!\n");
        return NULL;
    }

    if(compareString(foundUser->password, password, passwordLen) == 0)  /*Check the validity of the password*/
    {
        printf("the password is wrong!\n");
        return NULL;
    }

    printf("login successfully.\n");
    return foundUser;
}