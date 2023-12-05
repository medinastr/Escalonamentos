#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Node_{
    int tempo;
    struct Node_ *prox;
}Node;

// insere o novo processo na fila
Node *insereProcesso(Node *begin, Node *no) {
    no->tempo = rand() % 20 + 1;
    Node *ant = begin;
    Node *pont = begin->prox;
    while(pont != NULL) { // percorrendo a fila até achar o fim
        ant = pont;
        pont = pont->prox;
    }
    ant->prox = no;
    free(pont);
    return begin; // retorna o começo da fila com o novo processo inserido no fim
}

// retorna a chance de um novo processo ser criado
int criaProcesso() {
    int chance = rand() % 100 + 1;
    if(chance < 30) { // 30% de chance de ser menor que 30
        return 1;
    }
    return 0;
}

// pelo o que eu entendi isso seria o processamento
void processa(int tempo, int processo) {
    for(int i = 0; i < tempo; i++) {
    }
    printf("Processo %d foi executado em %d tempos\n", processo, tempo);
}

int main() {
    srand(time(NULL));
    Node *begin = (Node*)malloc(sizeof(Node));
    int processo = 0;

    while(true) { // loop infinito
        int chance = criaProcesso();
        if(chance == 1) { // verifica se deve ser criado um novo processo na iteração
            processo++;
            Node *novo_no = (Node*)malloc(sizeof(Node));
            begin = insereProcesso(begin, novo_no);
            processa(novo_no->tempo, processo);
        }
    }

    return 0;
}