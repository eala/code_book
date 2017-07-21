#include "apue.h"
#include <pthread.h>

void *fn1(void *arg){
    printf("thread 1 return\n");
    return((void *)1);
}

void *fn2(void *arg){
    printf("thread 2 exits\n");
    return((void *)2);
}

int main(int argc, char *argv[]){
    int err=0;
    pthread_t tid1, tid2;
    void *tret;

    err = pthread_create(&tid1, NULL, fn1, NULL);
    if(0 != err)
        err_exit(err, "can't create thread 1");
    err = pthread_create(&tid2, NULL, fn2, NULL);
    if(0 != err)
        err_exit(err, "can't create thread 2");

    // use join to fetch return value
    err = pthread_join(tid1, &tret);
    if(0 != err)
        err_exit(err, "can't join with thread 1");
    printf("thread 1 exit code: %ld\n", (long)tret);

    err = pthread_join(tid2, &tret);
    if(0 != err)
        err_exit(err, "can't join with thread 2");
    printf("thread 2 exit code: %ld\n", (long)tret);
    exit(0);
    return 0;
}
