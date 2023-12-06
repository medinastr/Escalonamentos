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
    while(pont->prox != NULL) { // percorrendo a fila até achar o fim
        pont = pont->prox;
    }
    pont->prox = novo_no;
    return begin; // retorna o começo da fila com o novo processo inserido no fim
}

// insere já ordenando visando o método Job mais curto primeiro
Node *insereOrdenado(Node *begin) {
    Node *novo_no = (Node*)malloc(sizeof(Node));
    novo_no->tempo = rand() % 20 + 1; // numero aleatorio de 1-20
    if(begin == NULL) { // caso não tenha processos
        return novo_no;
    }
    Node *ant = begin;
    Node *pont = begin->prox;
    while(pont != NULL && novo_no->tempo < pont->tempo) { // percorre a lista
        ant = pont;
        pont = pont->prox;
    }
    ant->prox = novo_no; // insere novo no na lista
    novo_no->prox = pont;

    return begin;
}

Node *removeProcesso(Node *begin) {
    if(begin != NULL) {
        begin = begin->prox;
    }
    return begin;
}

// retorna a chance de um novo processo ser criado
int chanceProcesso() {
    int chance = rand() % 100 + 1;
    if(chance < 30) { // 30% de chance de ser menor que 30
        return 1;
    }
    return 0;
}

Node *processaFCFS(Node *begin, int conta_processos) {
    if(begin != NULL) {
        for(int i = 0; i < begin->tempo; i++) {
            int chance = chanceProcesso();
            // se chance for verdadeira a lista recebe mais um processo nessa unidade de tempo
            if(chance == 1) {
                begin = insereProcesso(begin);
            }
        }
        printf("Processo %d foi executado em %d tempos\n", conta_processos, begin->tempo);
        begin = removeProcesso(begin); // remove o processo já executado
    }
    return begin;
}

// O Job mais curto não está funcionando, é um problema nessa função,
// no segundo while do main ou no insereOrdenado
Node *processaMaisCurto(Node *begin, int conta_processos) {
    if(begin != NULL) {
        for(int i = 0; i < begin->tempo; i++) {
            int chance = chanceProcesso();
            if(chance == 1) {
                begin = insereOrdenado(begin->prox); // unica diferença para o processaFCFS
                // (begin->prox) para ordenar sem mexer no NO que está sendo processado
            }
        }
        printf("Processo %d foi executado em %d tempos\n", conta_processos, begin->tempo);
        begin = removeProcesso(begin);
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

Node *processaRoundRobin(Node *begin, int conta_processos) {
    if(begin != NULL) {
        for(int i = 0; i < QUANTUM; i++) { // processa até o tempo atingir o limite do Quantum
            int chance = chanceProcesso();
            if(chance == 1) {
                begin = insereProcesso(begin);
            }
        }
        // checa se sobraram tempos (ELSE: o processo foi executado por completo)
        if(begin->tempo > QUANTUM) {
            Node *resto = (Node*)malloc(sizeof(Node));
            // coloca as unidades de tempo restantes em um novo NO (resto)
            resto->tempo = begin->tempo - QUANTUM;
            begin = insereNoFinal(begin, resto);
            printf("Processo %d foi executado e restaram %d tempos\n", conta_processos, resto->tempo);
        } else {
            printf("Processo %d foi executado e não restaram unidades de tempos\n", conta_processos);
        }
        begin = removeProcesso(begin); // remove o processo executado
    }
    return begin;
}

int main() {
    srand(time(NULL));
    Node *begin = (Node*)malloc(sizeof(Node)); //inicio da lista
    int conta_processos = 0;
    int aux = 0;
    int resposta = -1;

    printf("------------------------\nMÉTODOS DE ESCALONAMENTO (escolha um para executar):\n");
    printf("1 - First come first saved\n");
    printf("2 - Job mais curto primeiro\n");
    printf("3 - Round-Robin\n");
    scanf("%d", &resposta);
    
    begin = insereProcesso(begin);
    if(resposta == 1) {
        // while(true) { 
        while(aux < 50) { // sem loop infinito para testar
            conta_processos++;
            // begin recebe o processo já executado e removido 
            // com os novos processos possivelmente criados
            begin = processaFCFS(begin, conta_processos);
            aux++;
        }
    } else if(resposta == 2) {
        while(aux < 50) {
            conta_processos++;
            begin = processaMaisCurto(begin, conta_processos);
            aux++;
        }
    } else if(resposta == 3) {
        while(aux < 50) {
            conta_processos++;
            begin = processaRoundRobin(begin, conta_processos);
            aux++;
        }
    } else {
        printf("Opção inválida!\n");
    }

    return 0;
}