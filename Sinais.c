#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
int pronto = 0;

void *esperar_sinal(void *arg) {
    pthread_mutex_lock(&mutex);
    while (!pronto) {
        printf("Thread %ld esperando sinal\n", (long)arg);
        pthread_cond_wait(&cond_var, &mutex);
    }
    printf("Thread %ld recebeu o sinal\n", (long)arg);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *enviar_sinal(void *arg) {
    sleep(1);
    pthread_mutex_lock(&mutex);
    pronto = 1;
    pthread_cond_broadcast(&cond_var);
    pthread_mutex_unlock(&mutex);
    printf("Sinal enviado pela thread %ld\n", (long)arg);
    return NULL;
}

int main() {
    pthread_t threads[3], thread_sinal;

    for (long i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, esperar_sinal, (void *)i);
    }

    pthread_create(&thread_sinal, NULL, enviar_sinal, (void *)3);

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_join(thread_sinal, NULL);

    return 0;
}
