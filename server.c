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

#define OP_REG 1
#define OP_LOGIN 2

struct train{
    int trainId;
    char trainName[20];
    int PNR;
    int vaccate;
    int uid;
    // int deleted;
};

// socket related declaration
struct sockaddr_in server, client;
int sd, clientSize, nsd;

// file locking declarations
struct flock fl;

int userType = -1;

char *filepath[3] = {"normal.txt", "agent.txt", "admin.txt"};
char *countentries[3] = {"countnormal.txt","countagent.txt","countadmin.txt"};

#define length 100

int LoginStatus = -1;


void add(){
    struct train newTrain;
    read(nsd,&newTrain,sizeof(newTrain));
        
    newTrain.trainId = ntohl(newTrain.trainId);
    newTrain.PNR = ntohl(newTrain.PNR);
    newTrain.vaccate = ntohl(newTrain.vaccate);
    newTrain.uid = ntohl(newTrain.uid);
        
    printf("new train id : %d\n",newTrain.trainId);
    printf("new train name : %s\n",newTrain.trainName);
    printf("new train PNR : %d\n", newTrain.PNR);

    int fd = open("data.txt",O_RDWR);

    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    lseek(fd,0,SEEK_END);

    fcntl(fd,F_SETLKW,&fl);

    write(fd,&newTrain,sizeof(newTrain));

    fl.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &fl);

    close(fd);
}

void bookTicket(){
    int trainId,PNR,userId;
    
    read(nsd, &trainId, sizeof(trainId));
    read(nsd, &PNR, sizeof(PNR));
    read(nsd, &userId, sizeof(userId));


    trainId = ntohl(trainId);
    PNR = ntohl(PNR);
    userId = ntohl(userId);

    printf("train id = %d PNR = %d userID = %d",trainId,PNR,userId);

    int fd = open("data.txt",O_RDWR);

    struct train  STrain;

    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    fcntl(fd,F_SETLKW,&fl);
    
    while(1){
        read(fd,&STrain,sizeof(STrain));
        if(trainId == STrain.trainId && PNR == STrain.PNR){
            break;
        }
    }

    STrain.vaccate = 1;
    STrain.uid = userId;

    if(lseek(fd,-1*sizeof(STrain),SEEK_CUR) < 0){
        printf("some error in lseek");
    }

    write(fd,&STrain,sizeof(STrain));

    STrain.trainId = htonl(STrain.trainId);
    STrain.PNR = htonl(STrain.PNR);
    STrain.vaccate = htonl(STrain.vaccate);
    STrain.uid = htonl(STrain.uid);

    write(nsd, &STrain, sizeof(STrain));

    fl.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &fl);

    close(fd);
}

void search(){
    
    int trainId,PNR;
    
    read(nsd, &trainId, sizeof(trainId));
    read(nsd, &PNR, sizeof(PNR));

    trainId = ntohl(trainId);
    PNR = ntohl(PNR);

    int fd = open("data.txt",O_RDONLY);

    struct train  STrain;

    fl.l_type = F_RDLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    fcntl(fd,F_SETLKW,&fl);

    
    while(read(fd,&STrain,sizeof(STrain)) != 0){

    printf("STrain id : %d\n",STrain.trainId);
    printf("train id : %d\n", trainId);
    printf("train name : %s\n",STrain.trainName);
    printf("ST PNR : %d\n",STrain.PNR);
    printf("PNR : %d\n",PNR);
    printf("vaccate(0 or 1) : %d\n",STrain.vaccate);
    printf("uid : %d\n",STrain.uid);
    if(trainId == STrain.trainId && PNR == STrain.PNR){
        break;
    }
    }

    // printf("train id : %d\n",STrain.trainId);
    // printf("train name : %s\n",STrain.trainName);
    // printf("PNR : %d\n",STrain.PNR);
    // printf("vaccate(0 or 1) : %d\n",STrain.vaccate);
    // printf("uid : %d\n",STrain.uid);
    
    fl.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &fl);

    close(fd);

    STrain.trainId = htonl(STrain.trainId);
    STrain.PNR = htonl(STrain.PNR);
    STrain.vaccate = htonl(STrain.vaccate);
    STrain.uid = htonl(STrain.uid);
    write(nsd,&STrain,sizeof(STrain));
}

void updateBooking(){
    int trainId,PNR,uid;
    
    read(nsd, &trainId, sizeof(trainId));
    read(nsd, &PNR, sizeof(PNR));
    read(nsd, &uid, sizeof(uid));

    trainId = ntohl(trainId);
    PNR = ntohl(PNR);
    uid = ntohl(uid);

    int fd = open("data.txt",O_RDWR);

    struct train  STrain;

    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    fcntl(fd,F_SETLKW,&fl);
    
    while(1){
        read(fd,&STrain,sizeof(STrain));
        if(trainId == STrain.trainId && PNR == STrain.PNR){
            break;
        }
    }

    STrain.uid = uid;

    if(lseek(fd,-1*sizeof(STrain),SEEK_CUR) < 0){
        printf("some error in lseek");
    }

    write(fd,&STrain,sizeof(STrain));

    STrain.trainId = htonl(STrain.trainId);
    STrain.PNR = htonl(STrain.PNR);
    STrain.vaccate = htonl(STrain.vaccate);
    STrain.uid = htonl(STrain.uid);

    write(nsd, &STrain, sizeof(STrain));

    fl.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &fl);

    close(fd);
}

void cancle(){
    int trainId,PNR;
    
    read(nsd, &trainId, sizeof(trainId));
    read(nsd, &PNR, sizeof(PNR));

    trainId = ntohl(trainId);
    PNR = ntohl(PNR);

    int fd = open("data.txt",O_RDWR);

    struct train  STrain;

    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    fcntl(fd,F_SETLKW,&fl);
    
    while(1){
        read(fd,&STrain,sizeof(STrain));

        printf("STrain id : %d\n",STrain.trainId);
        // printf("train id : %d\n", trainId);
        printf("train name : %s\n",STrain.trainName);
        printf("ST PNR : %d\n",STrain.PNR);
        printf("PNR : %d\n",PNR);
        // printf("vaccate(0 or 1) : %d\n",STrain.vaccate);
        printf("uid : %d\n",STrain.uid);
        if(trainId == STrain.trainId && PNR == STrain.PNR){
            break;
        }
    }

    STrain.vaccate = 0;

    if(lseek(fd,-1*sizeof(STrain),SEEK_CUR) < 0){
        printf("some error in lseek");
    }

    write(fd,&STrain,sizeof(STrain));

    STrain.trainId = htonl(STrain.trainId);
    STrain.PNR = htonl(STrain.PNR);
    STrain.vaccate = htonl(STrain.vaccate);
    STrain.uid = htonl(STrain.uid);

    write(nsd, &STrain, sizeof(STrain));

    fl.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &fl);

    close(fd);
}

void delete(){
    printf("delete called\n");
    int trainId,PNR;
    
    read(nsd, &trainId, sizeof(trainId));
    read(nsd, &PNR, sizeof(PNR));

    trainId = ntohl(trainId);
    PNR = ntohl(PNR);

    int fd = open("data.txt",O_RDWR);

    struct train  STrain;

    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    fcntl(fd,F_SETLKW,&fl);
    
    while(1){
        int status = read(fd,&STrain,sizeof(STrain));
        if(status==0) break;
        if(trainId == STrain.trainId && PNR == STrain.PNR){
            break;
        }
    }

    struct train dummy;
    dummy.PNR = 0;
    dummy.trainId = 0;
    strcpy(dummy.trainName,"");
    dummy.uid = 0;
    dummy.vaccate = 0;

    if(lseek(fd,-1*sizeof(STrain),SEEK_CUR) < 0){
        printf("some error in lseek");
    }

    write(fd,&dummy,sizeof(dummy));

    fl.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &fl);

    close(fd);

    int status = 2;
    status = htonl(status);
    write(nsd, &status, sizeof(status));
}

void normalControl(){
    int operation;
    read(nsd, &operation, sizeof(operation));
    operation = ntohl(operation);
    
    switch (operation){
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
    int operation = -1;
    read(nsd,&operation,sizeof(operation));
    operation = ntohl(operation);
    
    if(operation==1){
        add();
    }
    if(operation == 2){
        delete();
    }
}

void adminControl(){
    int operation = -1;
    read(nsd,&operation,sizeof(operation));
    operation = ntohl(operation);

    int trainId,PNR;

    switch (operation){
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

void login(char username1[], char password1[],int uid){
    char username2[length], password2[length];

    int fd = open(filepath[userType-1],O_RDONLY);

    int jump = (uid-1)*(2*length);
    
    // locking file
    fl.l_type = F_RDLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    lseek(fd,jump,SEEK_SET);
    fcntl(fd,F_SETLKW,&fl);
    printf("jump - %d",jump);
    
    // for(int i=1 ; i<uid ; i++){
    //     read(fd,username2,length);
    //     read(fd,password2,length);
    //     printf("i - %d",i);
    //     printf("username from login : %s\n",username2);
    //     printf("password from login : %s\n",password2);
    // }
    read(fd,username2,length);
    read(fd,password2,length);
    printf("first username read - %s\n", username2);
    printf("first password read - %s\n", password2);
    // unlock file
    fl.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &fl);
    
    if((strcmp(username1,username2) == 0) && strcmp(password1,password2) == 0){
        printf("SUCCESSFULL LOGIN : username : %s password : %s\n",username1,password1);

        LoginStatus = 1;
        LoginStatus = htonl(LoginStatus);
        write(nsd,&LoginStatus,sizeof(LoginStatus));
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
        LoginStatus = htonl(LoginStatus);
        write(nsd,&LoginStatus,sizeof(LoginStatus));
        printf("Some error in login");
    }
}

void registerUser(char username[],char password[]){
    int fd = open(filepath[userType-1], O_RDWR | O_CREAT);
    int countEntriesFd = open(countentries[userType-1], O_RDWR);
    int noOfEntries = 0;
    
    read(countEntriesFd,&noOfEntries,sizeof(noOfEntries));

    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    fcntl(fd,F_SETLKW,&fl);

    lseek(fd,0,SEEK_END);

    write(fd,username,length);
    write(fd,password,length);

    fl.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &fl);

    printf("no of enrites before %d\n",noOfEntries);
    noOfEntries++;
    lseek(countEntriesFd,0,SEEK_SET);
    write(countEntriesFd,&noOfEntries,sizeof(noOfEntries));
    printf("no of entries after %d\n",noOfEntries);
    
    //sending noOfEntries as id to client;
    noOfEntries = htonl(noOfEntries);
    write(nsd,&noOfEntries,sizeof(noOfEntries));
    
    close(countEntriesFd);
    close(fd);
}

void mainMenu(int client){

    char username[100],password[100];

    read(client,&userType,sizeof(userType));

    userType = ntohl(userType);
    printf("user type : %d\n",userType);

    int operation = -1;
    read(client, &operation, sizeof(operation));

    operation = ntohl(operation);
    printf("operation : %d\n",operation);

    read(client, username,sizeof(username));
    // printf("username : %s\n",username);

    read(client, password,sizeof(password));
    // printf("password : %s\n",password);

    int uid = -1;
    // printf("user id : %d\n",uid);

    switch (operation)
    {
    case OP_REG:
        registerUser(username,password);
        break;
    case OP_LOGIN:
        read(client, &uid, sizeof(uid));
        uid = ntohl(uid);
        login(username,password,uid);
        break;
    default:
        break;
    }
}

int main(){

    sd = socket(AF_INET, SOCK_STREAM,0);

    if(sd == -1){
        printf("socket creation failed\n");
        EER_EXIT("socket()");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NO);

    if(bind(sd,(struct sockaddr *) (&server),sizeof(server)) < 0){
        printf("socket binding failed\n");
        EER_EXIT("binding()");
    }

    listen(sd, 10);
    clientSize = sizeof(client);
    while(1){
        nsd = accept(sd, (struct sockaddr *)(&client),&clientSize);

        if(!fork()){ // child
            close(sd);
            
            mainMenu(nsd);
            // int value;
            // read(nsd,&value,sizeof(value));
            // value = ntohl(value);

            // printf("Data from client is %d\n",value);
            // value = htonl(value+10);
            // printf("%d",value);
            // write(nsd,&value,sizeof(value));
            exit(0);
        }
        else{ //parent
            // perror("fork");
            close(nsd);
        }
    }

    return 0;
}