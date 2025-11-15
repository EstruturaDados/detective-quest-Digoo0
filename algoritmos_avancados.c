#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para o nó da árvore de pistas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura para representar uma sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];  // Pista associada à sala (pode estar vazia)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Função para criar uma nova sala dinamicamente
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro: não foi possível alocar memória para a sala!\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

// Função para criar um novo nó de pista
PistaNode* criarNoPista(const char* pista) {
    PistaNode* novoNo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        printf("Erro: não foi possível alocar memória para a pista!\n");
        exit(1);
    }
    
    strcpy(novoNo->pista, pista);
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    
    return novoNo;
}

// Função para inserir uma pista na BST (ordem alfabética)
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    // Se a árvore está vazia, cria um novo nó
    if (raiz == NULL) {
        return criarNoPista(pista);
    }
    
    // Compara as pistas para decidir a posição na BST
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao < 0) {
        // Pista menor, insere na subárvore esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        // Pista maior, insere na subárvore direita
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se as pistas são iguais, não insere duplicatas
    
    return raiz;
}

// Função para exibir as pistas em ordem alfabética (in-order traversal)
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("  - %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Função para explorar as salas e coletar pistas
void explorarSalasComPistas(Sala* salaAtual, PistaNode** raizPistas) {
    if (salaAtual == NULL) {
        return;
    }
    
    printf("\n=== VOCÊ ESTÁ NA: %s ===\n", salaAtual->nome);
    
    // Verifica se há pista nesta sala e se ainda não foi coletada
    if (strlen(salaAtual->pista) > 0) {
        printf("*** PISTA ENCONTRADA: %s ***\n", salaAtual->pista);
        
        // Adiciona a pista à BST
        *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
        
        // Remove a pista da sala para não coletar novamente
        strcpy(salaAtual->pista, "");
    }
    
    // Mostra opções disponíveis
    printf("\nSaídas disponíveis:\n");
    if (salaAtual->esquerda != NULL) {
        printf("  (e) Esquerda -> %s\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("  (d) Direita -> %s\n", salaAtual->direita->nome);
    }
    printf("  (s) Sair da exploração\n");
    
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
                    explorarSalasComPistas(salaAtual->esquerda, raizPistas);
                    escolhaValida = 1;
                } else {
                    printf("Não há saída pela esquerda! Escolha outra direção.\n");
                }
                break;
                
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    explorarSalasComPistas(salaAtual->direita, raizPistas);
                    escolhaValida = 1;
                } else {
                    printf("Não há saída pela direita! Escolha outra direção.\n");
                }
                break;
                
            case 's':
            case 'S':
                printf("Encerrando exploração...\n");
                return;
                
            default:
                printf("Opção inválida! Use 'e' para esquerda, 'd' para direita ou 's' para sair.\n");
                break;
        }
    }
}

// Função para liberar a memória da árvore de salas
void liberarArvoreSalas(Sala* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}

// Função para liberar a memória da árvore de pistas
void liberarArvorePistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

int main() {
    printf("=== Detective Quest - Caçada por Pistas ===\n");
    printf("Bem-vindo, detetive! Explore a mansão e colete pistas para resolver o mistério.\n");
    printf("As pistas coletadas serão organizadas automaticamente.\n\n");
    
    // Montagem do mapa da mansão (árvore binária) com pistas
    // Nível 0: Raiz
    Sala* hallEntrada = criarSala("Hall de Entrada", "Porta principal trancada por dentro");
    
    // Nível 1: Filhos do Hall
    Sala* salaEstar = criarSala("Sala de Estar", "Copo de vinho quebrado no chão");
    Sala* corredorPrincipal = criarSala("Corredor Principal", "Arranhões na parede");
    
    // Nível 2: Filhos da Sala de Estar
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto na mesa");
    Sala* jardimInverno = criarSala("Jardim de Inverno", "Terra molhada perto da janela");
    
    // Nível 2: Filhos do Corredor Principal
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida do bloco de facas");
    Sala* escadaria = criarSala("Escadaria", "Botão de camisa encontrado no degrau");
    
    // Nível 3: Filhos da Biblioteca
    Sala* escritorio = criarSala("Escritório Secreto", "Documento sigiloso na gaveta");
    Sala* salaMusica = criarSala("Sala de Música", "Partitura com anotações suspeitas");
    
    // Nível 3: Filhos da Cozinha
    Sala* despensa = criarSala("Despensa", "Pó branco em um saquinho");
    Sala* jardim = criarSala("Jardim", "Pegadas que levam ao muro");
    
    // Nível 3: Filhos da Escadaria  
    Sala* quartoHospedes = criarSala("Quarto de Hóspedes", "Mala semi-aberta com documentos");
    Sala* corredorSuperior = criarSala("Corredor Superior", "Quadro levemente desalinhado");
    
    // Nível 4: Filhos do Corredor Superior
    Sala* suiteMaster = criarSala("Suíte Master", "Relógio parado às 23:47");
    Sala* quartoEstudos = criarSala("Quarto de Estudos", "Computador com email aberto");
    
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
    
    // Árvore BST para armazenar pistas coletadas
    PistaNode* raizPistas = NULL;
    
    // Inicia a exploração a partir do Hall de Entrada
    explorarSalasComPistas(hallEntrada, &raizPistas);
    
    // Exibe todas as pistas coletadas em ordem alfabética
    printf("\n\n=== RELATÓRIO FINAL - PISTAS COLETADAS ===\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        printf("Pistas organizadas em ordem alfabética:\n");
        exibirPistas(raizPistas);
    }
    
    // Libera a memória alocada
    liberarArvoreSalas(hallEntrada);
    liberarArvorePistas(raizPistas);
    
    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}