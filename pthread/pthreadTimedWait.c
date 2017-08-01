/*****************************************************
 * pthread timed wait 
 *
 * build: gcc -o a.out main.c -lpthread -lrt
 * librt for clocktime
 *
 * Note: timed wait should use while loop with return condition
 * 
 *
******************************************************/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

// we use BUF_SIZE as signal criteria, but only filled FILLED_SIZE
// and use timeout to break
#define BUF_SIZE 60
#define FILLED_SIZE 20

#define MAX_LEN 64
#define MAX_ITER 100

pthread_mutex_t count_mutex;
pthread_mutex_t stop_mutex;
pthread_cond_t count_empty_cond;
pthread_cond_t count_full_cond;
int count=0;
int iteration=0;

enum THREAD_ENUM{
    FILL_THREAD=0,
    EMPTY_THREAD=1,
    ALL_THREADS
};

char THREAD_NAMES[ALL_THREADS][MAX_LEN]={
    "FILL",
    "EMPTY"
};

void *fillCount(void *id)
{
    long thread_id = (long)id;

    while(1){
        printf("%s::%d thread %ld, count: %d\n", __FUNCTION__, __LINE__, thread_id, count);

        pthread_mutex_lock(&count_mutex);
        while(0 != count){
            printf("%s::%d thread %ld count: %d, wait buffer empty\n", __FUNCTION__, __LINE__, thread_id, count);
            pthread_cond_wait(&count_empty_cond, &count_mutex);

            printf("%s::%d thread %ld count: %d, receive empty signal\n", __FUNCTION__, __LINE__, thread_id, count);
        }

        // in fill pthread, we only fill FILLED_SIZE
        while(FILLED_SIZE > count){
            count++;
            printf("%s::%d thread %ld fill count: %d\n", __FUNCTION__, __LINE__, thread_id, count);
        }

        if(BUF_SIZE < count){
            printf("%s::%d thread %ld count: %d, count filled\n", __FUNCTION__, __LINE__, thread_id, count);
            pthread_cond_signal(&count_full_cond);
            printf("%s::%d thread %ld count: %d, signaled full\n", __FUNCTION__, __LINE__, thread_id, count);
        }

        pthread_mutex_unlock(&count_mutex);

	pthread_mutex_lock(&stop_mutex);
	iteration++;
	if(iteration > MAX_ITER){
		pthread_mutex_unlock(&stop_mutex);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&stop_mutex);
    }
    printf("%s::%d thread %ld count: %d, going to exit thread\n", __FUNCTION__, __LINE__, thread_id, count);

    pthread_exit(NULL);
}

void *emptyCount(void *id)
{
    long thread_id = (long)id;

    while(1){
        printf("%s::%d thread %ld, count: %d\n", __FUNCTION__, __LINE__, thread_id, count);

        pthread_mutex_lock(&count_mutex);
        int rtn=EINVAL; 
        while(BUF_SIZE != count && ETIMEDOUT != rtn){
        //while(BUF_SIZE != count){
            pthread_mutex_lock(&stop_mutex);
            printf("iteration: %d\n", iteration);
            pthread_mutex_unlock(&stop_mutex);
            printf("%s::%d thread %ld count: %d, wait buffer filled\n", __FUNCTION__, __LINE__, thread_id, count);
            // time wait added part, wait 3 seconds or wait until buffer full
            struct timespec max_wait;
            clock_gettime(CLOCK_REALTIME, &max_wait);
            const int MAX_WAIT_SEC = 3; 
            max_wait.tv_sec += MAX_WAIT_SEC;
            rtn = pthread_cond_timedwait(&count_full_cond, &count_mutex, &max_wait);
            printf("%s::%d thread %ld count: %d, break cond_timedwait\n", __FUNCTION__, __LINE__, thread_id, count);
        }

        while( 0 != count){
            count--;
            printf("%s::%d thread %ld empty count: %d\n", __FUNCTION__, __LINE__, thread_id, count);
        }

        printf("%s::%d thread %ld count: %d, count emptyed\n", __FUNCTION__, __LINE__, thread_id, count);
        pthread_cond_signal(&count_empty_cond);
        printf("%s::%d thread %ld count: %d, signaled empty\n", __FUNCTION__, __LINE__, thread_id, count);

        pthread_mutex_unlock(&count_mutex);

	pthread_mutex_lock(&stop_mutex);
	iteration++;
	if(iteration > MAX_ITER){
		pthread_mutex_unlock(&stop_mutex);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&stop_mutex);
    }
    printf("%s::%d thread %ld count: %d, going to exit thread\n", __FUNCTION__, __LINE__, thread_id, count);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    long t1=1, t2=2;
    pthread_t fillThread, emptyThread;
    pthread_attr_t attr;

    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_empty_cond, NULL);
    pthread_cond_init(&count_full_cond, NULL);
    pthread_attr_init(&attr);
    pthread_create(&fillThread, &attr, fillCount, (void *)t1);
    pthread_create(&emptyThread, &attr, emptyCount, (void *)t2);

    pthread_join(fillThread, NULL);
    pthread_join(emptyThread, NULL);

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_empty_cond);
    pthread_cond_destroy(&count_full_cond);

    pthread_exit(NULL);

    return 0;
}

