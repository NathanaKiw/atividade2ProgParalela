#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *funcao_thread1(void *arg) {
    pthread_mutex_lock(&mutex1);
    sleep(1);  
    pthread_mutex_lock(&mutex2);
    printf("Thread 1 adquiriu ambos os mutexes\n");

    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}

void *funcao_thread2(void *arg) {
    pthread_mutex_lock(&mutex2);
    sleep(1);  
    pthread_mutex_lock(&mutex1);
    printf("Thread 2 adquiriu ambos os mutexes\n");

    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, funcao_thread1, NULL);
    pthread_create(&t2, NULL, funcao_thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
