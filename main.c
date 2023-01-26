#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "searching.h"
#include "accounts.h"
#include "posts.h"

#define MAX_COMMAND_SIZE 10

int main()
{
    char command[MAX_COMMAND_SIZE];
    user *userHeadNode, *userLastNode, *curUser = NULL;     /*curUser: the current user that logged in*/
    user *userNode;
    post *postNode;
    FILE *accountFile, *postFile;
    int postCount;

    userHeadNode = (user*) malloc(sizeof(user));    /*create the dummy node as head of the linked list of users*/
    if( userHeadNode == NULL)
    {
        printf("out of memory!\n");
        return 1;
    }
    userHeadNode->next = NULL;
    userLastNode = userHeadNode;

    printf("<<Guide>>\n");
    printf("signup <user name> <password>\n");
    printf("login <user name> <password>\n");
    printf("post <text>\nlike <user name> <post ID>\n");
    printf("logout\ndelete <post ID>\n");
    printf("info\nfind_user <username>\n");
    printf("-------------------------------------------\n");

    while(1)
    {
        printf("\nplease enter your desired command:\n");
        
        for(int i=0; i<10; i++)     /*free command*/
            command[i] = '\0';

        scanf("%9s", command);  

        if(compareString(command, "signup", MAX_COMMAND_SIZE))
        {   
            if(curUser != NULL)
                printf("Please logout first.\n");

            else    
                userLastNode = signup(userHeadNode->next, userLastNode);
        }

        else if(compareString(command, "login", MAX_COMMAND_SIZE))
        {   
            if(curUser != NULL)
                printf("Please log out of the current account, then enter the login command.\n");

            else
                curUser = login(userHeadNode->next);
        }

        else if(compareString(command, "logout", MAX_COMMAND_SIZE))
        {   
            if(curUser == NULL)
                printf("You are not logged in.\n");

            else
            {
                curUser = NULL;
                printf("log out successfully.\n");
            }
        }

        else if(compareString(command, "post", MAX_COMMAND_SIZE))
        {   
            if(curUser == NULL)
                printf("Please login first.\n");
                
            else
                newPost(curUser);
        }

        else if(compareString(command, "like", MAX_COMMAND_SIZE))
        {   
            if(curUser == NULL)
                printf("Please login first.\n");
                
            else
                like(userHeadNode->next , curUser);
        }

        else if(compareString(command, "delete", MAX_COMMAND_SIZE))
        {   
            if(curUser == NULL)
                printf("Please login first.\n");
                
            else
                deletePost(curUser);
        }

        else if(compareString(command, "info", MAX_COMMAND_SIZE))
        {   
            if(curUser == NULL)
                printf("Please login first.\n");
                
            else
                info(curUser);
        }

        else if(compareString(command, "find_user", MAX_COMMAND_SIZE))
        {   
            if(curUser == NULL)
                printf("Please login first.\n");
                
            else
                findUser(userHeadNode->next);
        }

        else
        {
            printf("wrong command.\n");
        }

        accountFile = fopen("accounts.txt", "w");
        postFile = fopen("posts.txt", "w");
        if((accountFile == NULL) || (postFile == NULL))
        {
            printf("can't open files.\n");
            return 1;
        }

        userNode = userHeadNode->next;
        while(userNode != NULL)     /*write the accounts information in the file*/
        {
            postCount = 0;
            postNode = (userNode->headPost)->next;
            while(postNode != NULL)     /*write the posts information in the file*/
            {
                fprintf(postFile, "%s  %s  %d\n", postNode->text, userNode->username, postNode->likeCount);
                postCount++;
                postNode = postNode->next;
            }
            fprintf(accountFile, "%s  %s  %d\n", userNode->username, userNode->password, postCount);
            userNode = userNode->next;
        }
        fclose(accountFile);
        fclose(postFile);
        
        fflush(stdin);
    }
}
    

