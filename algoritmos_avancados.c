#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala da mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Função para criar uma nova sala dinamicamente
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro: não foi possível alocar memória para a sala!\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

// Função para explorar as salas da mansão
void explorarSalas(Sala* salaAtual) {
    if (salaAtual == NULL) {
        return;
    }
    
    printf("\nVocê está na: %s\n", salaAtual->nome);
    
    // Verifica se é uma sala sem saídas (nó folha)
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("Esta sala não tem mais saídas. Exploração concluída!\n");
        return;
    }
    
    // Mostra opções disponíveis
    printf("Saídas disponíveis:\n");
    if (salaAtual->esquerda != NULL) {
        printf("  (e) Esquerda -> %s\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("  (d) Direita -> %s\n", salaAtual->direita->nome);
    }
    printf("  (s) Sair do jogo\n");
    
    // Loop para obter uma escolha válida do jogador
    char escolha;
    int escolhaValida = 0;
    
    while (!escolhaValida) {
        printf("\nPara onde deseja ir? (e/d/s): ");
        scanf(" %c", &escolha);
        
        switch (escolha) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    explorarSalas(salaAtual->esquerda);
                    escolhaValida = 1;
                } else {
                    printf("Não há saída pela esquerda! Escolha outra direção.\n");
                }
                break;
                
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    explorarSalas(salaAtual->direita);
                    escolhaValida = 1;
                } else {
                    printf("Não há saída pela direita! Escolha outra direção.\n");
                }
                break;
                
            case 's':
            case 'S':
                printf("Saindo do jogo...\n");
                return;
                
            default:
                printf("Opção inválida! Use 'e' para esquerda, 'd' para direita ou 's' para sair.\n");
                break;
        }
    }
}

// Função para liberar a memória alocada para a árvore
void liberarArvore(Sala* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

int main() {
    printf("=== Detective Quest - Exploração da Mansão ===\n");
    printf("Bem-vindo, detetive! Explore a mansão para encontrar pistas.\n");
    
    // Montagem do mapa da mansão (árvore binária)
    // Nível 0: Raiz
    Sala* hallEntrada = criarSala("Hall de Entrada");
    
    // Nível 1: Filhos do Hall
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* corredorPrincipal = criarSala("Corredor Principal");
    
    // Nível 2: Filhos da Sala de Estar
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardimInverno = criarSala("Jardim de Inverno");
    
    // Nível 2: Filhos do Corredor Principal
    Sala* cozinha = criarSala("Cozinha");
    Sala* escadaria = criarSala("Escadaria");
    
    // Nível 3: Filhos da Biblioteca
    Sala* escritorio = criarSala("Escritório Secreto");
    Sala* salaMusica = criarSala("Sala de Música");
    
    // Nível 3: Filhos da Cozinha
    Sala* despensa = criarSala("Despensa");
    Sala* jardim = criarSala("Jardim");
    
    // Nível 3: Filhos da Escadaria  
    Sala* quartoHospedes = criarSala("Quarto de Hóspedes");
    Sala* corredorSuperior = criarSala("Corredor Superior");
    
    // Nível 4: Filhos do Corredor Superior
    Sala* suiteMaster = criarSala("Suíte Master");
    Sala* quartoEstudos = criarSala("Quarto de Estudos");
    
    // Montagem da árvore - conectando os nós
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = corredorPrincipal;
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardimInverno;
    
    corredorPrincipal->esquerda = cozinha;
    corredorPrincipal->direita = escadaria;
    
    biblioteca->esquerda = escritorio;
    biblioteca->direita = salaMusica;
    
    cozinha->esquerda = despensa;
    cozinha->direita = jardim;
    
    escadaria->esquerda = quartoHospedes;
    escadaria->direita = corredorSuperior;
    
    corredorSuperior->esquerda = suiteMaster;
    corredorSuperior->direita = quartoEstudos;
    
    // Inicia a exploração a partir do Hall de Entrada
    explorarSalas(hallEntrada);
    
    // Libera a memória alocada para a árvore
    liberarArvore(hallEntrada);
    
    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}