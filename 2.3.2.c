//
//In order not to create a livelock, we will work similarly to the situation where the livelock was so we will take
//the right fork and if we cannot take a left fork then we will lower the right fork.
//It is only when this time that everyone picks up the forks and takes the forks down together what will happen is that
//everyone will make the move in turn and so every round will be someone to eat and live lock prevent.
//

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define N 5
#define THINKING 0
#define HUNGRY 3
#define EATING 4
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N
//--Add--//
#define RIGHT_TAKEN 2
#define LEFT_TAKEN 1
#define PUT_DOWN 5

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

sem_t mutex;
sem_t S[N];

//CHANGES:We split the take fork function into the following two functions:
//LeftTaken, RightTaken so that we can better control the taking and placing of the philosophers' forks.
bool LeftTaken(int phnum)
{
    bool toReturn=false;
    if ((state[LEFT] != EATING && state[LEFT] != RIGHT_TAKEN && state[phnum] != LEFT_TAKEN && state[LEFT] != PUT_DOWN)
        || ((state[phnum] == PUT_DOWN || (state[phnum] == RIGHT_TAKEN)) && state[LEFT] == PUT_DOWN)) {
        if (state[phnum] == HUNGRY || state[phnum] ==PUT_DOWN) {
            state[phnum] = LEFT_TAKEN;
            toReturn=true;
            printf("Philosopher %d takes fork %d \n",phnum + 1, LEFT + 1);
        }
        else if (state[phnum] == RIGHT_TAKEN) {
            state[phnum] = EATING;
            toReturn=true;
            printf("Philosopher %d takes fork %d\n",phnum + 1, LEFT + 1);
            printf("Philosopher %d is *EATING*\n",phnum + 1);
            sleep(2);
        }
        sem_post(&S[phnum]);
    }
    return toReturn;
}

bool RightTaken(int phnum){
    bool toReturn=false;
    if ((state[RIGHT] != EATING && state[RIGHT] != LEFT_TAKEN && state[phnum] != RIGHT_TAKEN && state[RIGHT] != PUT_DOWN)
        || ((state[phnum] == PUT_DOWN  || (state[phnum] == LEFT_TAKEN))&& state[RIGHT] == PUT_DOWN)) {
        if (state[phnum] == HUNGRY || state[phnum] == PUT_DOWN) {
            state[phnum] = RIGHT_TAKEN;
            toReturn=true;
            printf("Philosopher %d takes fork %d\n", phnum + 1, phnum + 1);
        } else if (state[phnum] == LEFT_TAKEN) {
            state[phnum] = EATING;
            toReturn=true;
            printf("Philosopher %d takes fork %d\n", phnum + 1, phnum + 1);
            printf("Philosopher %d is *EATING*\n", phnum + 1);
            sleep(2);
        }
        sem_post(&S[phnum]);
    }
    return toReturn;
}

void test(int phnum)
{
    if (state[phnum] == LEFT_TAKEN || state[phnum] == PUT_DOWN || state[phnum] == HUNGRY || state[phnum] == RIGHT_TAKEN){
        //can take right fork
        bool flagR = RightTaken(phnum);
        //can take left fork
        if (!flagR) {
            bool flagL = LeftTaken(phnum);
            //can't take any of the forks
            if (!flagL) {
                if (state[phnum] != PUT_DOWN) {
                    state[phnum] = PUT_DOWN;
                    printf("Philosopher %d put the fork %d down\n", phnum + 1, phnum + 1);
                }
                else{
                    printf("Philosopher %d is Waiting\n", phnum + 1);
                }
                sem_post(&S[phnum]);
            }
        }
    }
}

// take up chopsticks
void take_fork(int phnum) //get hungry
{
    sem_wait(&mutex);

    if (state[phnum] == THINKING) {
        // state that hungry
        state[phnum] = HUNGRY;
        printf("Philosopher %d is Hungry\n", phnum + 1);
    }

    // eat if neighbours are not eating
    test(phnum);

    sem_post(&mutex);

    // if unable to eat wait to be signalled
    sem_wait(&S[phnum]);

    sleep(1);
}

// put down chopsticks
void put_fork(int phnum) //get thinking
{
    sem_wait(&mutex);
    if (state[phnum] == EATING) {
        // state that thinking
        state[phnum] = THINKING;

        printf("Philosopher %d putting fork %d and %d down\n",
               phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is thinking\n", phnum + 1);

        test(LEFT);
        test(RIGHT);
    }
    sem_post(&mutex);
}

void* philospher(void* num)
{
    while (1) {

        int* i = num;

        sleep(1);

        take_fork(*i);

        sleep(0);

        put_fork(*i);
    }
}

int main()
{

    int i;
    pthread_t thread_id[N];

    // initialize the mutexes
    sem_init(&mutex, 0, 1);

    for (i = 0; i < N; i++)

        sem_init(&S[i], 0, 0);

    for (i = 0; i < N; i++) {

        // create philosopher processes
        pthread_create(&thread_id[i], NULL,
                       philospher, &phil[i]);

        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < N; i++)

        pthread_join(thread_id[i], NULL);
}