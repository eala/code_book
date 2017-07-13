/*****************************************************
 * pthread_cond_wait sample
 * 
 * increase (push) if count is zero (buffer is empty)
 *
******************************************************/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t count_mutex;
pthread_cond_t count_empty_cond;
int count = 0;
const int MAX_LEN = 64;
const int INC_RATE = 2;

enum THREAD_ENUM{
    FILL_THREAD=0,
    EMPTY_THREAD=1,
    ALL_THREADS
};

char THREAD_NAMES[ALL_THREADS][MAX_LEN]={
    "FILL",
    "EMPTY"
};

void *inc_count(void *id)
{
    long thread_id = (long)id;    

    //while(1){
        printf("%s::%d thread %ld, count: %d\n", __FUNCTION__, __LINE__, thread_id, count);
        pthread_mutex_lock(&count_mutex);
        while(0 != count){
            printf("%s::%d thread %ld count: %d, going to wait\n", __FUNCTION__, __LINE__, thread_id, count);
            pthread_cond_wait(&count_empty_cond, &count_mutex);
            printf("%s::%d thread %ld count: %d, receive signal\n", __FUNCTION__, __LINE__, thread_id, count);
        }
        printf("%s::%d thread %ld count: %d, going to increse count\n", __FUNCTION__, __LINE__, thread_id, count);
        count += INC_RATE;
        printf("%s::%d thread %ld count: %d, increse count\n", __FUNCTION__, __LINE__, thread_id, count);
        pthread_mutex_unlock(&count_mutex);
    //}
    printf("%s::%d thread %ld count: %d, going to exit thread\n", __FUNCTION__, __LINE__, thread_id, count);
    pthread_exit(NULL);
}

void *dec_count(void *id)
{
    long thread_id = (long)id;    

    //while(1){
        printf("%s::%d thread %ld, count: %d\n", __FUNCTION__, __LINE__, thread_id, count);
        pthread_mutex_lock(&count_mutex);
        while(0 != count){
            printf("%s::%d thread %ld count: %d, going to decrease\n", __FUNCTION__, __LINE__, thread_id, count);
            count--;
            printf("%s::%d thread %ld count: %d, decrease one\n", __FUNCTION__, __LINE__, thread_id, count);
            if(0 == count){
                printf("%s::%d thread %ld count: %d, going to broadcast\n", __FUNCTION__, __LINE__, thread_id, count);
                pthread_cond_broadcast(&count_empty_cond);
            }
        }
        pthread_mutex_unlock(&count_mutex);
    //}
    printf("%s::%d thread %ld count: %d, going to exit thread\n", __FUNCTION__, __LINE__, thread_id, count);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    long t1=1, t2=2;
    pthread_t fillThread, emptyThread;
    pthread_attr_t attr;

    pthread_mutex_init(&count_mutex, NULL);
    pthread_attr_init(&attr);
    pthread_create(&fillThread, &attr, inc_count, (void *)t1);
    pthread_create(&fillThread, &attr, dec_count, (void *)t2);

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_empty_cond);
    pthread_exit(NULL);

    return 0;
}
