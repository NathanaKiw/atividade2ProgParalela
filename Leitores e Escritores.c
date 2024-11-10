#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex_recurso = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_leitores = PTHREAD_MUTEX_INITIALIZER;
int contador_leitores = 0;
int dado_compartilhado = 0;

void *leitor(void *arg) {
    pthread_mutex_lock(&mutex_leitores);
    contador_leitores++;
    if (contador_leitores == 1) {
        pthread_mutex_lock(&mutex_recurso);
    }
    pthread_mutex_unlock(&mutex_leitores);

    printf("Leitor %ld lendo dado compartilhado: %d\n", (long)arg, dado_compartilhado);
    sleep(1);

    pthread_mutex_lock(&mutex_leitores);
    contador_leitores--;
    if (contador_leitores == 0) {
        pthread_mutex_unlock(&mutex_recurso);
    }
    pthread_mutex_unlock(&mutex_leitores);

    return NULL;
}

void *escritor(void *arg) {
    pthread_mutex_lock(&mutex_recurso);

    dado_compartilhado += 1;
    printf("Escritor %ld atualizou o dado compartilhado para: %d\n", (long)arg, dado_compartilhado);
    sleep(1);

    pthread_mutex_unlock(&mutex_recurso);
    return NULL;
}

int main() {
    pthread_t leitores[5], escritores[2];
    for (long i = 0; i < 5; i++) {
        pthread_create(&leitores[i], NULL, leitor, (void *)i);
    }
    for (long i = 0; i < 2; i++) {
        pthread_create(&escritores[i], NULL, escritor, (void *)i);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(leitores[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(escritores[i], NULL);
    }
    
    return 0;
}
