#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <string>
using namespace std;

#define NUM_THREADS 8
#define MAX 1000000
pthread_mutex_t mutx;

int data[MAX];
long global_sum = 0;

void *sum(void *threadid)
{
    long tid;
    long sum = 0;
    tid = (long)threadid;

    // Specifing the processing range for every thread in the data array
    int range = MAX / NUM_THREADS;
    int start = tid * range;
    int end = start + range;

    // Calculates sum of the data elements within range of thread.
    for (int i = start; i < end; i++)
    {
        sum += data[i];
    }

    // A mutex lock that makes sure global sum is updated in an order.
    pthread_mutex_lock(&mutx);
    global_sum += sum;

    // Disables the lock
    pthread_mutex_unlock(&mutx);

    //  Exits the thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // Starting the mutex lock
    pthread_mutex_init(&mutx, NULL);

    // To Determine the number of cores
    int cores = std::thread::hardware_concurrency();
    cout << " the number of cores on this machine = " << cores << endl;

    // Initializes data array with random numbers
    srand(time(NULL));
    for (int i = 0; i < MAX; i++)
    {
        data[i] = rand() % 20;
    }

    // Processing data by creating a pool of threads
    pthread_t threads[NUM_THREADS];
    long tid = 0;
    for (tid = 0; tid < NUM_THREADS; tid++)
    {
        pthread_create(&threads[tid], NULL, sum, (void *)tid);
    }

    // Waits for all threads to get finished
    for (tid = 0; tid < NUM_THREADS; tid++)
    {
        pthread_join(threads[tid], NULL);
    }

    // Prints the final sum
    cout << "the final sum =" << global_sum << endl;
}