#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MEMORIA_TOTAL 1024
#define THREADS 5

int memoria_usada = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t memoria_disponivel = PTHREAD_COND_INITIALIZER;

void alocar_memoria(int tamanho, int id) {

    // trava região crítica
    pthread_mutex_lock(&mutex);

    // enquanto não houver memória suficiente,
    // a thread fica esperando
    while (memoria_usada + tamanho > MEMORIA_TOTAL) {

        printf("[Thread %d] Esperando memória para %d unidades...\n",
               id, tamanho);

        pthread_cond_wait(&memoria_disponivel, &mutex);
    }

    // realiza alocação
    memoria_usada += tamanho;

    printf("[Thread %d] Alocou %d unidades (usado: %d)\n",
           id, tamanho, memoria_usada);

    // libera região crítica
    pthread_mutex_unlock(&mutex);
}

int main() {

    int id = 1;

    srand(time(NULL));

    // gera tamanho aleatório entre 100 e 500
    int tamanho = (rand() % 401) + 100;

    printf("Thread %d solicitou %d unidades de memória\n",
           id, tamanho);

    // chama a função de alocação
    alocar_memoria(tamanho, id);

    printf("Memória utilizada atualmente: %d\n", memoria_usada);

    return 0;
}