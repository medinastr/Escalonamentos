#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Node_{
    int tempo;
    struct Node_ *prox;
}Node;

// insere o novo processo na fila
Node *insereProcesso(Node *begin) {
    Node *novo_no = (Node*)malloc(sizeof(Node));
    novo_no->tempo = rand() % 20 + 1;
    Node *pont = begin;
    while(pont->prox != NULL) { // percorrendo a fila até achar o fim
        pont = pont->prox;
    }
    pont->prox = novo_no;
    return begin; // retorna o começo da fila com o novo processo inserido no fim
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

// pelo o que eu entendi isso seria o processamento
Node *processa(Node *begin, int tempo) {
    for(int i = 0; i < tempo; i++) {
        int chance = chanceProcesso();
        if(chance == 1) {
            begin = insereProcesso(begin);
        }
    }
    return begin;
}

int main() {
    srand(time(NULL));
    Node *begin = (Node*)malloc(sizeof(Node));
    int processo = 0;

    while(true) { // loop infinito
        int chance = chanceProcesso();
        if(chance == 1) { // verifica se deve ser criado um novo processo na iteração
            processo++;
            begin = insereProcesso(begin);
            begin = processa(begin, begin->tempo);
            printf("Processo %d foi executado em %d tempos\n", processo, begin->tempo);
            begin = removeProcesso(begin);
        }
    }

    return 0;
}