#include <stdio.h>
#include <signal.h>
#include <unistd.h> //for pipe() and fork()
#include <stdlib.h>
#include <sys/wait.h>

//In order for the processes to be updated and exchanged information we created a common arr where one cell will be used
// for reading and another cell will be used for writing.rw[0] for read rw[1] for write.
int rw[2];
int flagToStop=1;
int value=0;
int temp=0;
pid_t pFather ,pSon ,ppid;

void signalHandle1(int signal) {
    read(rw[0], &temp, sizeof(int));
    if (temp < 6) {
        printf("From Father:%d\n", temp);
        value = temp + 1;
        write(rw[1], &value, sizeof(int));
        kill(pFather, SIGUSR1);
    }else flagToStop=0;
}

void signalHandle2(int signal)
{
    read(rw[0],&temp, sizeof(int));
    printf("From Son:%d\n",temp);
    value=temp+1;
    write(rw[1],&value, sizeof(int));
    kill(getppid(), SIGUSR1);
    if(value==6) exit(0);
}

int main()
{
    //In order for both processes to have a common memory, the pipe must be created before the fork
    if(pipe(rw)<0)
    {
        printf("Pipe did not succeed");
        return -1;
    }
    pFather=fork(); //make another process like the parent process,this will be child process
    //Father see pFather as the son pid.
    //Son see pFather as 0.

    if(pFather<0)
    {
        printf("Fork did not succeed");
        return -1;
    }
    if(pFather>0) //its the father processes because it is bigger then 0
    {
        signal(SIGUSR1, signalHandle1);
        printf("pFather from father:%d\n",pFather);
        while(flagToStop){} //keep live
    }
    else if(pFather==0)//son processes
    {
        printf("pFather from son:%d\n",pFather);
        printf("son PID from son:%d\n",getpid());
        signal(SIGUSR1, signalHandle2);
        write(rw[1], &value, sizeof(int));
        kill(getppid(),SIGUSR1);
        while(flagToStop){}//keep live
    }
    wait(NULL);
    printf("Child is term\n");
    printf("Father is term\n");
    close(rw[0]);
    close(rw[1]);

    return 0;
}