# Simulador de Alocação de Memória com Threads (C)

Projeto em C que simula a alocação de memória em um ambiente concorrente, usando **mutex** e **variáveis de condição** (`pthread`) para sincronizar o acesso a um recurso compartilhado.

## Visão geral

O programa define uma memória virtual com capacidade total fixa (`MEMORIA_TOTAL = 1024` unidades). Uma thread solicita uma quantidade aleatória de memória (entre 100 e 500 unidades):

- Se houver espaço suficiente, a alocação é feita imediatamente.
- Se não houver, a thread **bloqueia** e espera até que haja memória disponível.

Esse é um exemplo clássico de problema de sincronização em Sistemas Operacionais (produtor/consumidor de recursos).

## Conceitos demonstrados

| Conceito | Onde aparece |
|---|---|
| Região crítica | `pthread_mutex_lock` / `pthread_mutex_unlock` em `alocar_memoria()` |
| Exclusão mútua | `mutex` protegendo a variável global `memoria_usada` |
| Espera condicional | `pthread_cond_wait` com `memoria_disponivel` |
| Proteção contra *spurious wakeups* | Uso de `while` (e não `if`) antes do `cond_wait` |

## Estrutura do código

```
.
├── main.c      # código-fonte (função main + alocar_memoria)
└── README.md
```

### Principais elementos

- `MEMORIA_TOTAL`: capacidade total de memória simulada (1024).
- `THREADS`: número de threads previsto para a simulação (5).
- `memoria_usada`: contador global da memória em uso.
- `mutex`: garante acesso exclusivo a `memoria_usada`.
- `memoria_disponivel`: variável de condição usada para bloquear/acordar threads.
- `alocar_memoria(int tamanho, int id)`: trava o mutex, espera enquanto não houver memória suficiente, realiza a alocação e libera o mutex.

## Como compilar e executar

Requisitos: `gcc` e suporte a POSIX threads (Linux/macOS/WSL).

```bash
gcc main.c -o alocador -pthread
./alocador
```

## Exemplo de saída

```
Thread 1 solicitou 342 unidades de memória
[Thread 1] Alocou 342 unidades (usado: 342)
Memória utilizada atualmente: 342
```

O valor solicitado muda a cada execução, pois é gerado com `rand()` inicializado por `srand(time(NULL))`.

## Estado atual e limitações

Na versão atual, o `main` executa **uma única solicitação** na thread principal, então o bloqueio nunca chega a acontecer (1024 comporta qualquer valor de 100 a 500). Além disso:

- A constante `THREADS` está definida, mas ainda não é usada (não há `pthread_create`).
- Não existe função de **liberação** de memória, então `pthread_cond_wait` nunca seria acordada por um `pthread_cond_signal`/`broadcast`.

## Próximos passos 

- [ ] Criar `THREADS` threads com `pthread_create`, cada uma solicitando memória aleatória.
- [ ] Implementar `liberar_memoria(int tamanho, int id)`, que diminui `memoria_usada` e chama `pthread_cond_broadcast(&memoria_disponivel)`.
- [ ] Simular tempo de uso da memória com `sleep()` / `usleep()` antes de liberar.
- [ ] Usar `pthread_join` para aguardar o término das threads.
- [ ] Usar `rand_r()` ou semente por thread, já que `rand()` não é thread-safe.
- [ ] Tratar o caso de uma requisição maior que `MEMORIA_TOTAL` (evita espera infinita).
