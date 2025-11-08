#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILA_TAM 5
#define PILHA_TAM 3

// Estrutura para representar uma peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura para a fila circular
typedef struct {
    Peca pecas[FILA_TAM];
    int frente;
    int tras;
    int quantidade;
} Fila;

// Estrutura para a pilha
typedef struct {
    Peca pecas[PILHA_TAM];
    int topo;
} Pilha;

int id_global = 0;

// Protótipos das funções
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
Peca gerarPeca();
void enfileirar(Fila *f, Peca peca);
Peca desenfileirar(Fila *f);
void empilhar(Pilha *p, Peca peca);
Peca desempilhar(Pilha *p);
void mostrarEstado(Fila *f, Pilha *p);
void jogarPeca(Fila *f, Pilha *p);
void reservarPeca(Fila *f, Pilha *p);
void usarPecaReservada(Fila *f, Pilha *p);
void trocarPeca(Fila *f, Pilha *p);
void trocarMultipla(Fila *f, Pilha *p);

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    
    Fila fila;
    Pilha pilha;
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    int opcao;
    
    printf("=== GERENCIADOR DE PEÇAS DO TETRIS ===\n");
    
    do {
        mostrarEstado(&fila, &pilha);
        
        printf("\n=== OPÇÕES DISPONÍVEIS ===\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("6 - Visualizar estado atual\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                jogarPeca(&fila, &pilha);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                break;
            case 3:
                usarPecaReservada(&fila, &pilha);
                break;
            case 4:
                trocarPeca(&fila, &pilha);
                break;
            case 5:
                trocarMultipla(&fila, &pilha);
                break;
            case 6:
                mostrarEstado(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        
    } while(opcao != 0);
    
    return 0;
}

// Inicializa a fila com peças geradas automaticamente
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
    
    // Preenche a fila com peças iniciais
    for(int i = 0; i < FILA_TAM; i++) {
        Peca nova = gerarPeca();
        enfileirar(f, nova);
    }
}

// Inicializa a pilha vazia
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Gera uma nova peça com nome aleatório e ID único
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice = rand() % 4;
    nova.nome = tipos[indice];
    nova.id = id_global++;
    return nova;
}

// Adiciona uma peça no final da fila
void enfileirar(Fila *f, Peca peca) {
    if(f->quantidade == FILA_TAM) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    
    f->tras = (f->tras + 1) % FILA_TAM;
    f->pecas[f->tras] = peca;
    f->quantidade++;
}

// Remove e retorna a peça da frente da fila
Peca desenfileirar(Fila *f) {
    if(f->quantidade == 0) {
        printf("Erro: Fila vazia!\n");
        Peca invalida = {'?', -1};
        return invalida;
    }
    
    Peca removida = f->pecas[f->frente];
    f->frente = (f->frente + 1) % FILA_TAM;
    f->quantidade--;
    
    return removida;
}

// Adiciona uma peça no topo da pilha
void empilhar(Pilha *p, Peca peca) {
    if(p->topo == PILHA_TAM - 1) {
        printf("Erro: Pilha cheia!\n");
        return;
    }
    
    p->topo++;
    p->pecas[p->topo] = peca;
}

// Remove e retorna a peça do topo da pilha
Peca desempilhar(Pilha *p) {
    if(p->topo == -1) {
        printf("Erro: Pilha vazia!\n");
        Peca invalida = {'?', -1};
        return invalida;
    }
    
    Peca removida = p->pecas[p->topo];
    p->topo--;
    
    return removida;
}

// Mostra o estado atual da fila e da pilha
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n=== ESTADO ATUAL ===\n");
    
    // Mostra a fila
    printf("Fila de peças:\t");
    if(f->quantidade == 0) {
        printf("Vazia");
    } else {
        int i = f->frente;
        for(int cont = 0; cont < f->quantidade; cont++) {
            printf("[%c %d] ", f->pecas[i].nome, f->pecas[i].id);
            i = (i + 1) % FILA_TAM;
        }
    }
    printf("\n");
    
    // Mostra a pilha
    printf("Pilha de reserva (Topo -> base):\t");
    if(p->topo == -1) {
        printf("Vazia");
    } else {
        for(int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
        }
    }
    printf("\n");
}

// Joga a peça da frente da fila
void jogarPeca(Fila *f, Pilha *p) {
    if(f->quantidade == 0) {
        printf("Não há peças na fila para jogar!\n");
        return;
    }
    
    Peca removida = desenfileirar(f);
    printf("Peça jogada: [%c %d]\n", removida.nome, removida.id);
    
    // Gera uma nova peça para manter a fila cheia
    Peca nova = gerarPeca();
    enfileirar(f, nova);
}

// Reserva a peça da frente da fila (move para a pilha)
void reservarPeca(Fila *f, Pilha *p) {
    if(f->quantidade == 0) {
        printf("Não há peças na fila para reservar!\n");
        return;
    }
    
    if(p->topo == PILHA_TAM - 1) {
        printf("Pilha de reserva cheia! Não é possível reservar.\n");
        return;
    }
    
    Peca removida = desenfileirar(f);
    empilhar(p, removida);
    printf("Peça reservada: [%c %d]\n", removida.nome, removida.id);
    
    // Gera uma nova peça para manter a fila cheia
    Peca nova = gerarPeca();
    enfileirar(f, nova);
}

// Usa a peça do topo da pilha
void usarPecaReservada(Fila *f, Pilha *p) {
    if(p->topo == -1) {
        printf("Não há peças na pilha para usar!\n");
        return;
    }
    
    Peca removida = desempilhar(p);
    printf("Peça usada da reserva: [%c %d]\n", removida.nome, removida.id);
    
    // Gera uma nova peça para a fila
    Peca nova = gerarPeca();
    enfileirar(f, nova);
}

// Troca a peça da frente da fila com a do topo da pilha
void trocarPeca(Fila *f, Pilha *p) {
    if(f->quantidade == 0) {
        printf("Fila vazia! Não é possível trocar.\n");
        return;
    }
    
    if(p->topo == -1) {
        printf("Pilha vazia! Não é possível trocar.\n");
        return;
    }
    
    // Troca as peças
    Peca temp = f->pecas[f->frente];
    f->pecas[f->frente] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;
    
    printf("Troca realizada entre [%c %d] (fila) e [%c %d] (pilha).\n", 
           temp.nome, temp.id, f->pecas[f->frente].nome, f->pecas[f->frente].id);
}

// Troca os 3 primeiros da fila com as 3 peças da pilha
void trocarMultipla(Fila *f, Pilha *p) {
    if(f->quantidade < 3) {
        printf("Fila com menos de 3 peças! Não é possível realizar a troca múltipla.\n");
        return;
    }
    
    if(p->topo < 2) {
        printf("Pilha com menos de 3 peças! Não é possível realizar a troca múltipla.\n");
        return;
    }
    
    // Troca as três peças
    for(int i = 0; i < 3; i++) {
        int indiceFila = (f->frente + i) % FILA_TAM;
        int indicePilha = p->topo - i;
        
        Peca temp = f->pecas[indiceFila];
        f->pecas[indiceFila] = p->pecas[indicePilha];
        p->pecas[indicePilha] = temp;
    }
    
    printf("Troca múltipla realizada: 3 primeiras da fila com as 3 da pilha.\n");
}