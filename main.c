#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define QUANTUM 6

typedef struct Node_{
    int tempo;
    struct Node_ *prox;
}Node;

// insere o novo processo na fila
Node *insereProcesso(Node *begin) {
    Node *novo_no = (Node*)malloc(sizeof(Node));
    novo_no->tempo = rand() % 20 + 1; // número aleatório de 1-20
    Node *pont = begin;
    if(begin == NULL) {
        return novo_no;
    }
    while(pont->prox != NULL) { // percorrendo a fila até achar o fim
        pont = pont->prox;
    }
    pont->prox = novo_no;
    return begin; // retorna o começo da fila com o novo processo inserido no fim
}

Node *removeProcesso(Node *begin) {
    Node *pont = begin;
    if(pont != NULL) {
        begin = begin->prox;
        free(pont);
    }
    return begin;
}

void mostrarProcessos(Node *begin) {
    if(begin != NULL) {
        Node *pont = begin;
        while(pont != NULL) {
            printf("%d ", pont->tempo);
            pont = pont->prox;
        }
    }
    printf("\n-------------------------\n");
}

// retorna a chance de um novo processo ser criado
int chanceProcesso() {
    int chance = rand() % 100 + 1;
    if(chance < 30) { // 30% de chance de ser menor que 30
        return 1;
    }
    return 0;
}

// First come first saved
Node *processaFCFS(Node *begin) {
    if(begin != NULL) {
        int processos_criados = 0;
        for(int i = 0; i < begin->tempo; i++) {
            int chance = chanceProcesso();
            // se chance for verdadeira a lista recebe mais um processo nessa unidade de tempo
            if(chance == 1) {
                processos_criados++;
                begin = insereProcesso(begin);
            }
        }
        printf("Processo executado -- %d tempos -- %d processos criados.\n", begin->tempo, processos_criados);
        begin = removeProcesso(begin); // remove o processo já executado
        mostrarProcessos(begin); // o processo executado não aparece no print
    }
    return begin;
}

// insere já ordenando visando o método Job mais curto primeiro
Node *insereOrdenado(Node *begin) {
    Node *novo_no = (Node*)malloc(sizeof(Node));
    novo_no->tempo = rand() % 20 + 1; // número aleatório de 1-20
    novo_no->prox = NULL;
    if(begin == NULL) {
        return novo_no;
    }

    Node *ant = begin;
    Node *pont = begin->prox;
    // o primeiro elemento é ignorado, pois este está sendo processado
    if(pont != NULL && novo_no->tempo < pont->tempo) {
        begin->prox = novo_no;
        novo_no->prox = pont;
        return begin;
    }
    // percorre a lista até achar um elemento com tempo maior que o novo processo
    while(pont != NULL && novo_no->tempo > pont->tempo) {
        ant = pont;
        pont = pont->prox;
    }
    if(pont != NULL) {
        ant->prox = novo_no;
        novo_no->prox = pont;
    } else {
        ant->prox = novo_no;
    }

    return begin;
}

// Job mais curto primeiro
Node *processaMaisCurto(Node *begin) {
    if(begin != NULL) {
        int processos_criados = 0;
        for(int i = 0; i < begin->tempo; i++) {
            int chance = chanceProcesso();
            if(chance == 1) {
                processos_criados++;
                begin = insereOrdenado(begin); // insere ordenado, mas sem mexer no processo em execução
            }
        }
        printf("Processo executado -- %d unidades de tempo -- %d processos criados\n", begin->tempo, processos_criados);
        begin = removeProcesso(begin); // remove o processo já executado
        mostrarProcessos(begin); // o processo executado não aparece no print
    }
    return begin;
}

// a função recebe um novo NO com as unidades de tempo que não foram processadas
// e as coloca no final da lista, visando o método Round-Robin
Node *insereNoFinal(Node *begin, Node *resto) {
    Node *pont = begin;
    while(pont->prox != NULL) { // percorre a lista até o final
        pont = pont->prox;
    }
    pont->prox = resto; // coloca o restante do processo no final da lista
    return begin;
}

Node *processaRoundRobin(Node *begin) {
    if(begin != NULL) {
        int processos_criados = 0;
        for(int i = 0; i < QUANTUM; i++) { // processa até o tempo atingir o limite do Quantum
            int chance = chanceProcesso();
            if(chance == 1) {
                processos_criados++;
                begin = insereProcesso(begin);
            }
        }
        // checa se sobraram tempos (ELSE: o processo foi executado por completo)
        if(begin->tempo > QUANTUM) {
            Node *resto = (Node*)malloc(sizeof(Node));
            // coloca as unidades de tempo restantes em um novo NO (resto)
            resto->tempo = begin->tempo - QUANTUM;
            begin = insereNoFinal(begin, resto);
            printf("Processo -- %d unidades de tempo -- %d unidades de tempo restantes -- 0 QUANTUMs restantes -- %d processos criados.\n", begin->tempo, resto->tempo, processos_criados);
        } else {
            printf("Processo executado -- %d unidades de tempo -- 0 unidades de tempo restantes -- %d QUANTUMs restantes -- %d processos criados\n", begin->tempo, QUANTUM - begin->tempo, processos_criados);
        }
        begin = removeProcesso(begin); // remove o processo executado
        mostrarProcessos(begin); // o processo executado não aparece no print
    }
    return begin;
}

int main() {
    srand(time(NULL));
    Node *begin = NULL; //inicio da lista
    int aux = 0;
    int resposta = -1;

    printf("------------------------\nMÉTODOS DE ESCALONAMENTO (escolha um para executar):\n");
    printf("0 - Sair\n");
    printf("1 - First come first saved\n");
    printf("2 - Job mais curto primeiro\n");
    printf("3 - Round-Robin\n");
    scanf("%d", &resposta);
    
    if(resposta == 1) { 
        while(true) {
            if(begin == NULL) {
                printf("Sem processos nesse tempo\n");
                int chance = chanceProcesso();
                if(chance == 1) {
                    begin = insereProcesso(begin);
                    printf("Processo criado fora de um processamento\n");
                    mostrarProcessos(begin);
                }
            } else {
                begin = processaFCFS(begin);
            }
        }
    } else if(resposta == 2) {
        while(aux < 10) {
            if(begin == NULL) {
                printf("Sem processos nesse tempo\n");
                int chance = chanceProcesso();
                if(chance == 1) {
                    begin = insereOrdenado(begin);
                    printf("Processo criado fora de um processamento\n");
                    mostrarProcessos(begin);
                }
            } else {
                begin = processaMaisCurto(begin);
            }
        }
    } else if(resposta == 3) {
        while(true) {
            if(begin == NULL) {
                printf("Sem processos nesse tempo\n");
                int chance = chanceProcesso();
                if(chance == 1) {
                    begin = insereProcesso(begin);
                    printf("Processo criado fora de um processamento\n");
                    mostrarProcessos(begin);
                }
            } else {
                begin = processaRoundRobin(begin);
            }
        }
    } else if(resposta == 0) {
        printf("FIM!\n");
    } else {
        printf("Opção inválida\n");
    }

    return 0;
}