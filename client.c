#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define EER_EXIT(msg) do {perror(msg); exit(EXIT_FAILURE);}while(0)
#define PORT_NO 8082

#define Normal 1
#define Agent 2
#define Admin 3

#define SUCCESS 1
#define FAIL -1

struct train{
    int trainId;
    char trainName[20];
    int PNR;
    int vaccate;
    int uid;
    // int deleted;
};

int LoginStatus = 0;

int userType = -1;

int sfd;
struct sockaddr_in serverAdd;

int userId = -1;

void bookTicket(){
    int trainId, PNR;
    printf("Enter train id : ");
    scanf("%d",&trainId);
    printf("Enter PNR : ");
    scanf("%d",&PNR);

    trainId = htonl(trainId);
    PNR = htonl(PNR);
    userId = htonl(userId);

    write(sfd, &trainId, sizeof(trainId));
    write(sfd , &PNR, sizeof(PNR));
    write(sfd, &userId, sizeof(userId));

    userId = ntohl(userId);

    struct train STrain;
    read(sfd, &STrain, sizeof(STrain));

    //convertion
    STrain.trainId = ntohl(STrain.trainId);
    STrain.PNR = ntohl(STrain.PNR);
    STrain.vaccate = ntohl(STrain.vaccate);
    STrain.uid = ntohl(STrain.uid);

    printf("train id : %d\n",STrain.trainId);
    printf("train name : %s\n",STrain.trainName);
    printf("PNR : %d\n",STrain.PNR);
    printf("vaccate(0 or 1) : %d\n",STrain.vaccate);
    printf("uid : %d\n",STrain.uid);
}

// void viewBooking(){

// }

void updateBooking(){
    int trainId, PNR, uid;
    printf("Enter train id : ");
    scanf("%d",&trainId);
    printf("Enter PNR : ");
    scanf("%d",&PNR);
    printf("Enter new uid to be updated : ");
    scanf("%d",&uid);

    trainId = htonl(trainId);
    PNR = htonl(PNR);
    uid = htonl(uid);

    write(sfd, &trainId, sizeof(trainId));
    write(sfd, &PNR, sizeof(PNR));
    write(sfd, &uid, sizeof(uid));

    struct train STrain;
    read(sfd, &STrain, sizeof(STrain));

    //convertion
    STrain.trainId = ntohl(STrain.trainId);
    STrain.PNR = ntohl(STrain.PNR);
    STrain.vaccate = ntohl(STrain.vaccate);
    STrain.uid = ntohl(STrain.uid);

    printf("train id : %d\n",STrain.trainId);
    printf("train name : %s\n",STrain.trainName);
    printf("PNR : %d\n",STrain.PNR);
    printf("vaccate(0 or 1) : %d\n",STrain.vaccate);
    printf("uid : %d\n",STrain.uid);
}

void cancle(){
    int trainId, PNR;
    printf("Enter train id : ");
    scanf("%d",&trainId);
    printf("Enter PNR : ");
    scanf("%d",&PNR);

    trainId = htonl(trainId);
    PNR = htonl(PNR);

    write(sfd, &trainId, sizeof(trainId));
    write(sfd , &PNR, sizeof(PNR));

    
    
    struct train STrain;
    read(sfd, &STrain, sizeof(STrain));
    

    STrain.trainId = ntohl(STrain.trainId);
    STrain.PNR = ntohl(STrain.PNR);
    STrain.vaccate = ntohl(STrain.vaccate);
    STrain.uid = ntohl(STrain.uid);

    printf("train id : %d\n",STrain.trainId);
    printf("train name : %s\n",STrain.trainName);
    printf("PNR : %d\n",STrain.PNR);
    printf("vaccate(0 or 1) : %d\n",STrain.vaccate);
    printf("uid : %d\n",STrain.uid);
}

void search(){
    int trainId, PNR;
    printf("Enter train id : ");
    scanf("%d",&trainId);
    printf("Enter PNR : ");
    scanf("%d",&PNR);

    trainId = htonl(trainId);
    PNR = htonl(PNR);

    write(sfd, &trainId, sizeof(trainId));
    write(sfd , &PNR, sizeof(PNR));

    struct train STrain;
    read(sfd, &STrain, sizeof(STrain));

    //convertion
    STrain.trainId = ntohl(STrain.trainId);
    STrain.PNR = ntohl(STrain.PNR);
    STrain.vaccate = ntohl(STrain.vaccate);
    STrain.uid = ntohl(STrain.uid);

    printf("train id : %d\n",STrain.trainId);
    printf("train name : %s\n",STrain.trainName);
    printf("PNR : %d\n",STrain.PNR);
    printf("vaccate(0 or 1) : %d\n",STrain.vaccate);
    printf("uid : %d\n",STrain.uid);
}

void add(){
    struct train newTrain;
    printf("Enter train id : ");
    scanf("%d", &newTrain.trainId);
    printf("Enter train name : ");
    scanf("%s", newTrain.trainName);
    newTrain.PNR = rand();
    printf("Enter vaccate(0(empty) or 1(filled)) : ");
    scanf("%d", &newTrain.vaccate);
    // newTrain.uid = userId;
    printf("Enter uid : ");
    scanf("%d", &newTrain.uid);
    printf("PNR no to book ticket : %d",newTrain.PNR);

    newTrain.trainId = htonl(newTrain.trainId);
    newTrain.PNR = htonl(newTrain.PNR);
    newTrain.vaccate = htonl(newTrain.vaccate);
    newTrain.uid = htonl(newTrain.uid);

    write(sfd, &newTrain, sizeof(struct train));

    newTrain.trainId = ntohl(newTrain.trainId);
    newTrain.PNR = ntohl(newTrain.PNR);
    newTrain.vaccate = ntohl(newTrain.vaccate);
    newTrain.uid = ntohl(newTrain.uid);
}

void delete(){
    int trainId, PNR;
    printf("Enter train id : ");
    scanf("%d",&trainId);
    printf("Enter PNR : ");
    scanf("%d",&PNR);

    trainId = htonl(trainId);
    PNR = htonl(PNR);

    write(sfd, &trainId, sizeof(trainId));
    write(sfd, &PNR, sizeof(PNR));

    // struct train STrain;
    // read(sfd, &STrain, sizeof(STrain));

    //convertion
    // STrain.trainId = ntohl(STrain.trainId);
    // STrain.PNR = ntohl(STrain.PNR);
    // STrain.vaccate = ntohl(STrain.vaccate);
    // STrain.uid = ntohl(STrain.uid);

    // printf("train id : %d\n",STrain.trainId);
    // printf("train name : %s\n",STrain.trainName);
    // printf("PNR : %d\n",STrain.PNR);
    // printf("vaccate(0 or 1) : %d\n",STrain.vaccate);
    // printf("uid : %d\n",STrain.uid);
    int status;
    read(sfd, &status, sizeof(status));
    status = ntohl(status);
    if(status >= 0) printf("deletion suceessful\n");
    else printf("Some error in deletion");
}

void registerUser(int operation){
    char username[100],password[100];
    
    printf("1. Normal user\n");
    printf("2. Agent user\n");
    printf("3. Admin user\n");
    printf("Enter user type : ");

    scanf("%d",&userType);

    printf("Enter username : ");
    scanf("%s",username);

    printf("Enter password : ");
    scanf("%s",password);

    userType = htonl(userType);
    write(sfd, &userType, sizeof(userType));

    operation = htonl(operation);
    write(sfd, &operation, sizeof(operation));
    write(sfd, username, sizeof(username));
    write(sfd, password, sizeof(password));

    
    read(sfd, &userId,sizeof(userId));
    userId = ntohl(userId);
    printf("Your user id is - %d",userId);
}

void normalControl(){
    printf("1. Book Ticket \n");
    printf("2. View Previous Bookings  \n");
    printf("3. Update Booking \n");
    printf("4. Cancel Booking \n");
    int input;
    scanf("%d",&input);

    input = htonl(input);
    write(sfd,&input,sizeof(input));
    input = ntohl(input);

    switch (input){
        case 1:
            bookTicket();
            break;
        case 2:
            search();
            break;
        case 3:
            updateBooking();
            break;
        case 4:
            cancle();
            break;
        default:
            break;
    }
}

void agentControl(){
    printf("1. Add Train or Ticket\n");
    printf("2. Delete Train or Ticket\n");
    printf("Enter your choice : ");
    int input;
    scanf("%d",&input);


    input = htonl(input);
    write(sfd,&input,sizeof(input));
    input = ntohl(input);


    switch (input)
    {
    case 1:
        add();
        break;
    case 2:
        delete();
        break;
    default:
        break;
    }
}

void adminControl(){
    printf("1. Modify \n");
    printf("2. Search  \n");
    printf("3. Add \n");
    printf("4. Delete \n");
    int input;
    scanf("%d",&input);

    input = htonl(input);
    write(sfd,&input,sizeof(input));
    input = ntohl(input);

    switch (input){
        case 1:
            updateBooking();
            break;
        case 2:
            search();
            break;
        case 3:
            add();
            break;
        case 4:
            delete();
            break;
        default:
            break;
    }
}

void login(int operation){
    char username[100],password[100];
    
    printf("1. Normal user\n");
    printf("2. Agent user\n");
    printf("3. Admin user\n");
    printf("Enter user type : ");

    scanf("%d",&userType);

    printf("Enter username : ");
    scanf("%s",username);

    printf("Enter password : ");
    scanf("%s",password);

    int uid;
    printf("Enter id : ");
    scanf("%d",&uid);

    userType = htonl(userType);
    write(sfd, &userType, sizeof(userType));
    userType = ntohl(userType);

    operation = htonl(operation);
    write(sfd, &operation, sizeof(operation));
    operation = ntohl(operation);

    write(sfd, username, sizeof(username));
    write(sfd, password, sizeof(password));

    uid = htonl(uid);
    write(sfd, &uid, sizeof(uid));
    uid = ntohl(uid);

    read(sfd,&LoginStatus,sizeof(LoginStatus));
    LoginStatus = ntohl(LoginStatus);
    if(LoginStatus == 1){
        userId = uid;
        printf("Login Successfull\n");
        // printf("user type - %d\n",userType);
        switch (userType)
        {
        case 1:
            normalControl();
            break;
        case 2:
            agentControl();
            break;
        case 3:
            adminControl();
            break;
        default:
            break;
        }
    }else{
        printf("Some Error while login\n");
    }
}

void mainMenu(){
    int choice = -1;
    printf("\e[1;1H\e[2J");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("Enter choice : \n");
    scanf("%d",&choice);

    switch (choice)
    {
    case 1:
        registerUser(choice);
        break;
    case 2:
        login(choice);
        break;
    default:
        break;
    }
}



int main(){

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    perror("socket");
    bzero(&serverAdd, sizeof(serverAdd));

    serverAdd.sin_family = AF_INET;
    serverAdd.sin_port = htons(PORT_NO);
    serverAdd.sin_addr.s_addr = INADDR_ANY;

    connect(sfd,(struct sockaddr*)(&serverAdd),sizeof(serverAdd));
    perror("connect");

    printf("Welcome to Online Railway Ticket Booking System");
    
    // printf("Enter a number to send to server\n");
    
    // int value;
    // scanf("%d",&value);
    // value = htonl(value);
    // write(sfd, &value, sizeof(value));

    // read(sfd, &value, sizeof(value));
    // value = ntohl(value);
    // printf("value returned by server = %d\n",value);

    mainMenu();

    close(sfd);
    return 0;
}