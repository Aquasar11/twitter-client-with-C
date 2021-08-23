/*      Twitter
 *     (Client)
 *  By Ali Razghandi
 *
 *                    */

/*importing libraries*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.h"


//Constant Macro Values

#define SIZE 100000
#define SMALLER_SIZE 1000

/* Main Functions Prototypes */
void start_menu();
void sign_up();
void log_in();
void main_menu();
void time_line();
void search_function();
void tweet_profile();
void personal_area();
void logout();
void send_tweet();
void refresh();
void set_bio();
void change_pass();

void delete_tweet();

/* Global Values */

char token[33];


// Server Parts

struct sockaddr_in server;

int initialize (){
    WSADATA wsadata;
    int wsastartup = WSAStartup(MAKEWORD(2,2),&wsadata);
    if (wsastartup != 0){
        printf("Initialize failed");
        return -1;
    }
    int port = 12345;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    return 0;
}

char *send_data(char *data){

    char *server_response = malloc(SIZE);
    memset(server_response,0,SIZE);


    int client_socket = socket(AF_INET,SOCK_STREAM,0);

    if (client_socket == INVALID_SOCKET){
        printf("Invalid socket");
        return server_response;
    }

    int can_connect = connect(client_socket,(struct  sockaddr *) &server, sizeof(server));

    if (can_connect != 0){
        printf("Can't connect to the server");
        return server_response;
    }

    send(client_socket, data , strlen(data),0);
    recv(client_socket,server_response,SIZE,0);
    closesocket(client_socket);

    return server_response;

}


//main Function

int main()
{
    int initial = initialize();
    if (initial == -1){
        return 0;
    }

    printf("Welcome! \nPlease enter the number of each option to choose it \n\n");

    start_menu();

    return 0;
}

// main menu page

void start_menu()
{
    int chosen_menu = 0;
    printf("Main Menu:\n1.Sign Up\n2.Log In\n3.Exit\n");
    printf("Please enter your choice: ");

    scanf("%d", &chosen_menu);

    switch (chosen_menu)
    {
    case 1:
        fflush(stdin);
        system("cls");
        sign_up();
        break;
    case 2:
        fflush(stdin);
        system("cls");
        log_in();
        break;
        case 3:
            break;
    default:
        fflush(stdin);
        system("cls");
        printf("Invalid choice\nPlease try again\n\n");
        start_menu();
        break;
    }
}

/* sign up menu functions  */

// sign up in the server

int sign_up_server(char username[],char password[]){
    char *server_ans;
    char signup_message[SIZE];

    sprintf(signup_message,"signup %s, %s\n",username,password);

    server_ans = send_data(signup_message);

    cJSON * ans = cJSON_Parse(server_ans);

    cJSON * message = cJSON_GetObjectItemCaseSensitive(ans,"type");
    cJSON * message_value = cJSON_GetObjectItemCaseSensitive(ans,"message");

    char *result = cJSON_Print(message);



   if (strcmp(result,"\"Successful\"")==0){
       free(server_ans);
       cJSON_Delete(ans);
        return 0;
    }
   else if (strcmp(result,"\"Error\"")==0){
       free(server_ans);
       fflush(stdin);
       system("cls");
       printf("%s \nPlease try again \n",message_value->valuestring);
       cJSON_Delete(ans);
       return -1;
   }
    free(server_ans);
    cJSON_Delete(ans);
    return -2;
}

// sign up view function

void sign_up()
{
    char username[SMALLER_SIZE];
    char password[SMALLER_SIZE];
    char back[]="back";

    memset(username,0,SMALLER_SIZE);
    memset(password,0,SMALLER_SIZE);

    printf("Sign Up Menu\n\n");
    printf("Please enter a username and a password for your account \n");
    printf("To go back to the previous menu type \"back\" in both inputs \n");
    printf("Username:\n");
    scanf("%s",username);
    printf("Password:\n");
    scanf("%s",password);

    int result = sign_up_server(username,password);

    if (strcmp(username,back) == 0 && strcmp(password,back) == 0)
    {
        fflush(stdin);
        system("cls");
        start_menu();
    }
    else
        {
    switch (result) {
        case -1:
            sign_up();
            break;
        case 0:
            fflush(stdin);
            system("cls");
            printf("Signed up successfully\n");
            log_in();
            break;
        default:
            fflush(stdin);
            system("cls");
            printf("An error occurred while trying to signing up\nPlease try again");
            start_menu();
            break;

    }
    }

}


/* Log in menu functions */

int log_in_server(char username[],char password[]){
    char *server_ans;
    char log_in_message[SIZE];

    sprintf(log_in_message,"login %s, %s\n",username,password);

    server_ans = send_data(log_in_message);
    cJSON * ans = cJSON_Parse(server_ans);
    cJSON * type = cJSON_GetObjectItemCaseSensitive(ans,"type");
    cJSON * message = cJSON_GetObjectItemCaseSensitive(ans,"message");

    char *result = cJSON_Print(type);


    if (strcmp(result,"\"Token\"")==0) {
        char *value = cJSON_Print(message);
        cJSON_Delete(ans);

        for (size_t i = 1; i < 33; i++)
        {
            token[i-1]=value[i];
        }
        token[33] = '\0';
        free(server_ans);
        free(result);
        free(value);
        return 0;
    }
    else
        {
        cJSON_Delete(ans);
        free(server_ans);
        free(result);
        return -1;
    }

}

// log in menu view

void log_in()
{
    char username[SMALLER_SIZE];
    char password[SMALLER_SIZE];
    char back[]="back";

    memset(username,0,SMALLER_SIZE);
    memset(password,0,SMALLER_SIZE);

    printf("Log In Menu\n\n");
    printf("Please enter your username and password to log in\n");
    printf("Username:\n");
    scanf("%s",username);
    printf("Password:\n");
    scanf("%s",password);

    int result = log_in_server(username,password);

    if (strcmp(username,back) == 0 && strcmp(password,back) == 0)
    {
        fflush(stdin);
        system("cls");
        start_menu();
    }
    else {
        switch (result) {
            case -1:
                fflush(stdin);
                system("cls");
                printf("Wrong username or password\nPlease try again\n\n");
                log_in();
                break;
            case 0:
                fflush(stdin);
                system("cls");
                printf("Log in successfully\n\n");
                main_menu();
                break;
            default:
                fflush(stdin);
                system("cls");
                printf("An error occurred while trying to logging in\nPlease try again\n");
                start_menu();
                break;

        }
    }
}

// main menu view
void main_menu(){
    int chosen_menu = 0;

    printf("Main Menu \n\n1.Timeline\n2.Search\n3.Tweet Profile\n4.Personal Area\n5.Log Out\n");
    printf("Please enter your chosen number: ");

    scanf("%d", &chosen_menu);

    switch (chosen_menu)
    {
        case 1:
            fflush(stdin);
            system("cls");
            time_line();
            break;
        case 2:
            fflush(stdin);
            system("cls");
            search_function();
            break;
        case 3:
            fflush(stdin);
            system("cls");
            tweet_profile();
            break;
        case 4:
            fflush(stdin);
            system("cls");
            personal_area();
            break;
        case 5:
            fflush(stdin);
            system("cls");
            logout();
            break;
        default:
            fflush(stdin);
            system("cls");
            printf("Invalid choice\nPlease try again\n\n");
            main_menu();
            break;
    }
}

// time line view
void time_line(){
    int chosen_menu = 0;

    printf("Time Line \n\n1.Send tweet\n2.Refresh\n3.back\n");
    printf("Please enter your chosen number: ");

    scanf("%d", &chosen_menu);

    switch (chosen_menu) {

        case 1:
            fflush(stdin);
            system("cls");
            send_tweet();
            break;
        case 2:
            fflush(stdin);
            system("cls");
            refresh();
            break;
        case 3:
            fflush(stdin);
            system("cls");
            main_menu();
            break;
        default:
            fflush(stdin);
            system("cls");
            printf("Invalid choice\nPlease try again\n\n");
            time_line();
            break;
    }
}

// send tweet function
void send_tweet(){
    char st[SIZE];
    char tweet[SIZE];
    int i = 0;
    char c;

    printf("Please write your tweet below and press enter to send it\n");

    while ((c = getchar()) != '\n')
    {
        tweet[i] = c;
        i += 1;
    }

    sprintf(st,"send tweet %s, %s\n",token,tweet);

    send_data(st);

    system("cls");
    printf("Successfully sent to the server.\n\n");
    time_line();
}

// refresh

void refresh(){

    char refresh_command[SIZE];

    sprintf(refresh_command,"refresh %s\n",token);

    char *result =  send_data(refresh_command);

    int i;
    cJSON *elem;
    cJSON *id;
    cJSON *author;
    cJSON *content;
    cJSON *comments;
    cJSON *likes;

    cJSON *root = cJSON_Parse(result);

    cJSON * message = cJSON_GetObjectItemCaseSensitive(root,"message");

    int n = cJSON_GetArraySize(message);

    for (i = 0; i < n; i++) {
        elem = cJSON_GetArrayItem(message, i);
        id = cJSON_GetObjectItem(elem, "id");
        author = cJSON_GetObjectItem(elem, "author");
        content = cJSON_GetObjectItem(elem, "content");
        comments = cJSON_GetObjectItem(elem, "comments");
        int numberofcomments = cJSON_GetArraySize(comments);
        likes = cJSON_GetObjectItem(elem, "likes");

        printf("%s   %d\n",author->valuestring,id->valueint);
        printf("%s\n",content->valuestring);
        printf("likes:%d   comments:%d\n",likes->valueint,numberofcomments);
        printf("\n");
    }

    printf("1.Like or comment \n2.back\n");
    printf("Please enter your chosen number: ");

    int chosen_menu = 0;
    fflush(stdin);
    scanf("%d", &chosen_menu);

    free(result);

    switch (chosen_menu)
    {
        case 1:
            fflush(stdin);
            printf("\n\n1.Like \n2.Comment \n3.See comments \n4.back\n");
            printf("Please enter your chosen number: ");

            int likeorcomment = 0;

            scanf("%d", &likeorcomment);

            switch (likeorcomment)
            {
                case 1:
                    fflush(stdin);
                    char like_command[SIZE];
                    printf("\nPlease enter the id of the tweet that you want to like: ");

                    int likedtweet ;

                    scanf("%d",&likedtweet);

                    sprintf(like_command,"like %s, %d\n",token,likedtweet);

                    char * like_result =  send_data(like_command);

                    cJSON * liked_root = cJSON_Parse(like_result);

                    cJSON * liked_message =  cJSON_GetObjectItemCaseSensitive(liked_root,"message");
                    cJSON * liked_type =  cJSON_GetObjectItemCaseSensitive(liked_root,"type");

                    char  *liked_error_cjson = liked_type->valuestring;

                    if (strcmp(liked_error_cjson,"Error")==0) {
                        printf("%s\n",liked_message -> valuestring);
                        system("pause");
                    } else{
                        printf("\nYou liked tweet number %d\n", likedtweet);
                        system("pause");
                    }
                    free(like_result);
                    cJSON_Delete(root);
                    free(liked_error_cjson);
                    refresh();
                    break;
                case 2:
                    fflush(stdin);
                    char comment_command[SIZE];
                    printf("\nPlease first enter the id then write your comment \nid: ");

                    int commenttweet;
                    char c;
                    int k = 0;

                    scanf("%d",&commenttweet);

                    char comment_content[SIZE];

                    printf("\nwrite your comment below then press enter to send it\n");
                    fflush(stdin);
                    while ((c = getchar()) != '\n')
                    {
                        comment_content[k] = c;
                        k += 1;
                    }

                    sprintf(comment_command,"comment %s, %d, %s\n",token,commenttweet,comment_content);

                    char * comment_result =  send_data(comment_command);

                    cJSON * comment_root = cJSON_Parse(comment_result);

                    cJSON * comment_message =  cJSON_GetObjectItemCaseSensitive(comment_root,"message");
                    cJSON * comment_type =  cJSON_GetObjectItemCaseSensitive(comment_root,"type");

                    char  *comment_error_cjson = comment_type->valuestring;

                    if (strcmp(comment_error_cjson,"Error")==0) {
                        printf("%s",comment_message -> valuestring);
                        system("pause");
                    } else{
                        printf("\nYour comment has successfully sent to the server\n");
                        system("pause");
                    }
                    free(comment_result);
                    cJSON_Delete(root);
                    free(comment_error_cjson);
                    refresh();
                    break;
                case 3:
                    fflush(stdin);

                    int see;

                    printf("\nPlease enter the id of the tweet that you want to see its comments: ");
                    scanf("%d",&see);

                    int m;
                    cJSON *see_elem;
                    cJSON *see_id;
                    cJSON *see_comments;
                    cJSON *show_comments;

                    int nm = cJSON_GetArraySize(message);
                    int shown =0 ;

                    for (m = 0; m < nm; m++) {
                        see_elem = cJSON_GetArrayItem(message, m);
                        see_id = cJSON_GetObjectItem(see_elem, "id");
                        see_comments = cJSON_GetObjectItem(see_elem, "comments");

                        if (see == see_id ->valueint)
                        {
                            shown = 1;
                            int number_of_see_comments = cJSON_GetArraySize(see_comments);

                            for (size_t j = 0 ; j < number_of_see_comments ; j++)
                            {
                                show_comments = cJSON_GetArrayItem(see_comments,j);
                                printf("\n%s: %s\n\n",show_comments->string,show_comments->valuestring);
                            }
                        }
                    }
                    if (shown == 0){
                        printf("There is no match for this id in your timeline\n");
                    }
                    system("pause");
                    cJSON_Delete(root);
                    refresh();
                    break;
                case 4:
                    cJSON_Delete(root);
                    refresh();
                    break;
                default:
                    printf("\nInvalid choice\nPlease try again\n\n");
                    cJSON_Delete(root);
                    refresh();
                    break;
            }

            break;
        case 2:
            fflush(stdin);
            system("cls");
            cJSON_Delete(root);
            time_line();
            break;
        default:
            printf("\nInvalid choice\nPlease try again\n\n");
            cJSON_Delete(root);
            refresh();
            break;
    }
}

// search menu
void search_function(){
    char search[1000];
    char *result;
    memset(search,0,1000);
    char back[] = "-1";

    printf("Search \n\n");
    printf("To go back write -1 and press enter\n");
    printf("Please enter the profile name that you looking for: ");

    scanf("%s",search);

    if (strcmp(search,back)==0){
        fflush(stdin);
        system("cls");
        main_menu();
    }
    else
    {
        system("cls");
        char search_command[SIZE];
        sprintf(search_command,"search %s, %s\n",token,search);
        result = send_data(search_command);
        char *type_error = "Error";

        cJSON *username;
        cJSON *bio;
        cJSON *numberOfFollowers;
        cJSON *numberOfFollowings;
        cJSON *followStatus;
        cJSON *allTweets;
        cJSON *element;
        cJSON *id;
        cJSON *author;
        cJSON *content;
        cJSON *comments;
        cJSON *likes;

        cJSON *root = cJSON_Parse(result);

        cJSON * message = cJSON_GetObjectItemCaseSensitive(root,"message");\
        cJSON * type = cJSON_GetObjectItemCaseSensitive(root,"type");

        char *type_result = type->valuestring;

        if (strcmp(type_result,type_error)==0){

            printf("%s\n\n",message->valuestring);
            search_function();
        }
        else
        {
            username = cJSON_GetObjectItemCaseSensitive(message, "username");
            bio = cJSON_GetObjectItemCaseSensitive(message, "bio");
            numberOfFollowers = cJSON_GetObjectItemCaseSensitive(message, "numberOfFollowers");
            numberOfFollowings = cJSON_GetObjectItemCaseSensitive(message, "numberOfFollowings");
            followStatus = cJSON_GetObjectItemCaseSensitive(message, "followStatus");
            allTweets = cJSON_GetObjectItemCaseSensitive(message, "allTweets");

            int numberoftweets = cJSON_GetArraySize(allTweets);

            printf("Username: %s \n\nBio:\n%s\n\n", username->valuestring, bio->valuestring);
            printf("Followers:%d   Followings:%d\n\n", numberOfFollowers->valueint, numberOfFollowings->valueint);
            printf("%s\n\n", followStatus->valuestring);

            printf("Tweets\n\n");

            for (size_t j = 0; j < numberoftweets; j++) {
                element = cJSON_GetArrayItem(allTweets, j);
                id = cJSON_GetObjectItem(element, "id");
                author = cJSON_GetObjectItem(element, "author");
                content = cJSON_GetObjectItem(element, "content");
                comments = cJSON_GetObjectItem(element, "comments");
                int numberofcomments = cJSON_GetArraySize(comments);
                likes = cJSON_GetObjectItem(element, "likes");

                printf("%s   %d\n", author->valuestring, id->valueint);
                printf("%s\n", content->valuestring);
                printf("likes:%d   comments:%d\n", likes->valueint, numberofcomments);
                printf("\n");
            }
            printf("You can follow or unfollow this account by write it down\nFor going back to the search menu write back and press enter\n");

            char chosen_command;
            fflush(stdin);
            scanf("%c", &chosen_command);


            if (chosen_command == 'f' || chosen_command == 'F') {
                char follow_command[SIZE];

                sprintf(follow_command, "follow %s, %s\n", token, search);

                send_data(follow_command);

                printf("Now you follow this account.\n");

                system("pause");
                cJSON_Delete(root);
                free(result);
                fflush(stdin);
                system("cls");
                search_function();

            } else if (chosen_command == 'u' || chosen_command == 'U') {
                char follow_command[SIZE];

                sprintf(follow_command, "unfollow %s, %s\n", token, search);

                send_data(follow_command);

                printf("Now you unfollow this account.\n");

                system("pause");
                cJSON_Delete(root);
                free(result);
                fflush(stdin);
                system("cls");
                search_function();

            } else {
                cJSON_Delete(root);
                free(result);
                fflush(stdin);
                system("cls");
                search_function();
            }
        }
    }

}

// tweet profile menu
void tweet_profile(){

    char tweet_profile_command[SIZE];
    char *result;
    memset(tweet_profile_command,0,1000);

        sprintf(tweet_profile_command,"profile %s\n",token);
        result = send_data(tweet_profile_command);

        cJSON *username;
        cJSON *bio;
        cJSON *numberOfFollowers;
        cJSON *numberOfFollowings;
        cJSON *allTweets;
        cJSON *element;
        cJSON *id;
        cJSON *author;
        cJSON *content;
        cJSON *comments;
        cJSON *likes;

        cJSON *root = cJSON_Parse(result);

        cJSON * message = cJSON_GetObjectItemCaseSensitive(root,"message");\
        cJSON * type = cJSON_GetObjectItemCaseSensitive(root,"type");

        char *type_result = type->valuestring;

    if (strcmp("Error",type_result)!=0) {
        username = cJSON_GetObjectItemCaseSensitive(message, "username");
        bio = cJSON_GetObjectItemCaseSensitive(message, "bio");
        numberOfFollowers = cJSON_GetObjectItemCaseSensitive(message, "numberOfFollowers");
        numberOfFollowings = cJSON_GetObjectItemCaseSensitive(message, "numberOfFollowings");
        allTweets = cJSON_GetObjectItemCaseSensitive(message, "allTweets");

        int numberoftweets = cJSON_GetArraySize(allTweets);

        printf("Username: %s \n\nBio:\n%s\n\n", username->valuestring, bio->valuestring);
        printf("Followers:%d   Followings:%d\n\n", numberOfFollowers->valueint, numberOfFollowings->valueint);

        printf("Tweets\n\n");

        for (size_t j = 0; j < numberoftweets; j++) {
            element = cJSON_GetArrayItem(allTweets, j);
            id = cJSON_GetObjectItem(element, "id");
            author = cJSON_GetObjectItem(element, "author");
            content = cJSON_GetObjectItem(element, "content");
            comments = cJSON_GetObjectItem(element, "comments");
            int numberofcomments = cJSON_GetArraySize(comments);
            likes = cJSON_GetObjectItem(element, "likes");

            printf("%s   %d\n", author->valuestring, id->valueint);
            printf("%s\n", content->valuestring);
            printf("likes:%d   comments:%d\n", likes->valueint, numberofcomments);
            printf("\n");
        }

        printf("1.Personal area \n2.Delete a tweet\n3.Back\n");
        printf("Please enter your chosen number: ");

        int chosen_menu = 0;

        fflush(stdin);
        scanf("%d", &chosen_menu);

        switch (chosen_menu) {

            case 1:
                fflush(stdin);
                system("cls");
                cJSON_Delete(root);
                free(result);
                personal_area();
                break;
            case 2:
                fflush(stdin);
                char delete_command[SIZE];
                printf("\nPlease enter the id of the tweet that you want to delete: ");

                int deletetweet ;

                scanf("%d",&deletetweet);

                sprintf(delete_command,"delete %s, %d\n",token,deletetweet);

                char * delete_result =  send_data(delete_command);

                cJSON * delete_root = cJSON_Parse(delete_result);

                cJSON * delete_message =  cJSON_GetObjectItemCaseSensitive(delete_root,"message");
                cJSON * delete_type =  cJSON_GetObjectItemCaseSensitive(delete_root,"type");

                char  *deleted_error_cjson = delete_type->valuestring;

                if (strcmp(deleted_error_cjson,"Error")==0) {
                    printf("%s\n",delete_message -> valuestring);
                    system("pause");
                } else{
                    printf("\nYou delete tweet number %d\n", deletetweet);
                    system("pause");
                }
                cJSON_Delete(root);
                cJSON_Delete(delete_root);
                free(result);
                free(delete_result);
                tweet_profile();
                break;
            case 3:
                fflush(stdin);
                system("cls");
                cJSON_Delete(root);
                free(result);
                main_menu();
                break;
            default:
                cJSON_Delete(root);
                free(result);
                fflush(stdin);
                system("cls");
                printf("Invalid choice\nPlease try again\n\n");
                tweet_profile();
                break;
        }

    }else {
        fflush(stdin);
        printf("%s",message->valuestring);
        main_menu();
    }

}

// personal area menu
void personal_area(){

    fflush(stdin);
    system("cls");
    printf("1.Set bio\n2.Change your password\n3.Back\n");
    printf("Please enter your chosen command: ");

    int chosen_command = 0;

    scanf("%d", &chosen_command);

    switch (chosen_command) {

        case 1:
            fflush(stdin);
            system("cls");
            set_bio();
            break;
        case 2:
            fflush(stdin);
            system("cls");
            change_pass();
            break;
        case 3:
            fflush(stdin);
            system("cls");
            main_menu();
            break;
        default:
            fflush(stdin);
            system("cls");
            printf("Invalid choice\nPlease try again\n\n");
            personal_area();
            break;
    }
}

// set bio fanction
void set_bio(){
    char tweet_profile_command[SIZE];
    char *result;
    memset(tweet_profile_command,0,1000);

    sprintf(tweet_profile_command,"profile %s\n",token);
    result = send_data(tweet_profile_command);
    char *type_error = "Error";


    cJSON *bio;

    cJSON *root = cJSON_Parse(result);

    cJSON * message = cJSON_GetObjectItemCaseSensitive(root,"message");
    cJSON * type = cJSON_GetObjectItemCaseSensitive(root,"type");

    char *type_result = type->valuestring;

    if (strcmp(type_error,type_result)!=0) {

        bio = cJSON_GetObjectItemCaseSensitive(message, "bio");

        if (strlen(bio->valuestring)==0)
        {
            char set_bio_command[SIZE];
            char bio_content[SIZE];
            int i = 0;
            char c;

            printf("Please write your bio below and press enter to set it\n");

            while ((c = getchar()) != '\n')
            {
                bio_content[i] = c;
                i += 1;
            }

            sprintf(set_bio_command,"set bio %s, %s\n",token,bio_content);

            send_data(set_bio_command);

            system("cls");
            cJSON_Delete(root);
            free(result);
            personal_area();
        } else
        {
            printf("You set your bio before and you can not change it anymore.\n");
            printf("This is your bio: %s\n\n",bio->valuestring);
            system("pause");
            personal_area();
        }
    }else {
        fflush(stdin);
        printf("%s\n",message->valuestring);
        cJSON_Delete(root);
        free(result);
        personal_area();
    }
}

// change password function
void change_pass(){
    char current_pass[SMALLER_SIZE];
    char new_pass[SMALLER_SIZE];

    memset(current_pass,0,SMALLER_SIZE);
    memset(new_pass,0,SMALLER_SIZE);

    printf("Change password Menu\n\n");
    printf("Please enter your current password first and then write your new password\n");
    printf("Current Password:\n");
    scanf("%s",current_pass);
    printf("New Password:\n");
    scanf("%s",new_pass);

    char *server_ans;
    char change_pass_command[SIZE];

    sprintf(change_pass_command,"change password %s, %s, %s\n",token,current_pass,new_pass);

    server_ans = send_data(change_pass_command);

    cJSON * ans = cJSON_Parse(server_ans);

    cJSON * message = cJSON_GetObjectItemCaseSensitive(ans,"message");

    printf("%s\n",message->valuestring);
    system("pause");
    cJSON_Delete(ans);
    free(server_ans);
    personal_area();

}


// logout menu
void logout(){
    char logout_command[SIZE];

    sprintf(logout_command,"logout %s\n",token);

    send_data(logout_command);

    system("cls");
    printf("Logout successfully.\n\n");
    start_menu();
}
