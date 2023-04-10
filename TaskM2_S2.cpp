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

    // Specify the data array's processing range for each thread.
    int range = MAX / NUM_THREADS;
    int start = tid * range;
    int end = start + range;

    // Calculate the thread's range's sum of the data elements.
    for (int i = start; i < end; i++)
    {
        sum += data[i];
    }

    // Use a mutex lock to ensure that global sum is updated sequentially.
    pthread_mutex_lock(&mutx);
    global_sum += sum;
    pthread_mutex_unlock(&mutx);
    // To enable other threads to access global sum, release the lock.

    //  Exit the thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // Initialize mutex lock
    pthread_mutex_init(&mutx, NULL);

    // Determine the number of cores on the machine
    int cores = std::thread::hardware_concurrency();
    cout << " the number of cores on this machine = " << cores << endl;

    // Initializing data array with random values
    srand(time(NULL));
    for (int i = 0; i < MAX; i++)
    {
        data[i] = rand() % 20;
    }

    // Start the clock
    clock_t start_time = clock();

    // Create a pool of threads to process the data
    pthread_t threads[NUM_THREADS];
    long tid = 0;
    for (tid = 0; tid < NUM_THREADS; tid++)
    {
        pthread_create(&threads[tid], NULL, sum, (void *)tid);
    }

    // Wait for all threads to complete before printing the final sum
    for (tid = 0; tid < NUM_THREADS; tid++)
    {
        pthread_join(threads[tid], NULL);
    }

    // Stop the clock
    clock_t end_time = clock();

    // Calculate the total execution time
    double execution_time = double(end_time - start_time) / CLOCKS_PER_SEC;

    // Print the final sum and execution time
    cout << "the final sum = " << global_sum << endl;
    cout << "execution time = " << execution_time << " seconds" << endl;
}
