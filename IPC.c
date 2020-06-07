//
// Created by orian on 23/05/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


int fd[2];
pid_t pid;  //son process number;
int val,rVal;

void fsig_handler(int sig){
    read(fd[0],&rVal,sizeof(int));
    if(rVal<6){
        printf("%d\n",rVal);
        val=rVal+1;
        write(fd[1],&val,sizeof(int));
        kill(pid,SIGUSR1);
    }
}

void ssig_handler(int sig){
    read(fd[0],&rVal,sizeof(int));
    printf("%d\n",rVal);
    val=rVal+1;
    write(fd[1],&val,sizeof(int));
    kill(getppid(),SIGUSR1);
    if(val == 6)  exit(0);
}



int main(){
    val=rVal=0;
    if(pipe(fd)<0){
        printf("Failed to open pipe");
        return -1;
    }
    pid = fork();
    if(pid<0){
        printf("Failed to fork");
        return -1;
    }
    if(pid){                             //father process
        signal(SIGUSR1,fsig_handler);
        while(rVal<6){}
    }
    else{                                //son process
        signal(SIGUSR1,ssig_handler);
        write(fd[1],&val,sizeof(int));
        kill(getppid(),SIGUSR1);
        while(rVal<6){}
    }
    wait(NULL);
    printf("Child is going to be terminated\n");
    printf("Parent is going to be terminated\n");
    close(fd[0]);
    close(fd[1]);
    return 0;
}

