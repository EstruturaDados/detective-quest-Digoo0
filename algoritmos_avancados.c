#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 10
#define MAX_SUSPEITOS 5
#define MIN_PISTAS_CULPADO 2

// Estrutura para o nó da árvore de pistas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura para entrada da tabela hash
typedef struct HashEntry {
    char pista[100];
    char suspeito[50];
    struct HashEntry *proximo; // Para encadeamento
} HashEntry;

// Estrutura para representar uma sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];  // Pista associada à sala (pode estar vazia)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Tabela Hash global
HashEntry* tabelaHash[TAMANHO_HASH];

// Lista de suspeitos
char suspeitos[MAX_SUSPEITOS][50] = {
    "Carlos", "Maria", "João", "Ana", "Pedro"
};

// Função hash simples baseada no primeiro caractere da pista
int hash(const char* pista) {
    if (strlen(pista) == 0) return 0;
    return (pista[0] * 31) % TAMANHO_HASH;
}

// Função para inicializar a tabela hash
void inicializarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

// Função para criar uma nova entrada na hash
HashEntry* criarEntradaHash(const char* pista, const char* suspeito) {
    HashEntry* novaEntrada = (HashEntry*)malloc(sizeof(HashEntry));
    if (novaEntrada == NULL) {
        printf("Erro: não foi possível alocar memória para entrada hash!\n");
        exit(1);
    }
    
    strcpy(novaEntrada->pista, pista);
    strcpy(novaEntrada->suspeito, suspeito);
    novaEntrada->proximo = NULL;
    
    return novaEntrada;
}

// Função para inserir associação pista-suspeito na tabela hash
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = hash(pista);
    HashEntry* novaEntrada = criarEntradaHash(pista, suspeito);
    
    // Inserção no início da lista encadeada
    novaEntrada->proximo = tabelaHash[indice];
    tabelaHash[indice] = novaEntrada;
}

// Função para encontrar suspeito associado a uma pista
char* encontrarSuspeito(const char* pista) {
    int indice = hash(pista);
    HashEntry* atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return "Desconhecido"; // Pista não encontrada
}

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
    if (raiz == NULL) {
        return criarNoPista(pista);
    }
    
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    
    return raiz;
}

// Função para exibir as pistas em ordem alfabética (in-order traversal)
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        char* suspeito = encontrarSuspeito(raiz->pista);
        printf("  - %s (Aponta para: %s)\n", raiz->pista, suspeito);
        exibirPistas(raiz->direita);
    }
}

// Função para contar quantas pistas apontam para um suspeito específico
int contarPistasPorSuspeito(PistaNode* raiz, const char* suspeito) {
    if (raiz == NULL) {
        return 0;
    }
    
    int count = 0;
    if (raiz->esquerda != NULL) {
        count += contarPistasPorSuspeito(raiz->esquerda, suspeito);
    }
    
    char* suspeitoPista = encontrarSuspeito(raiz->pista);
    if (strcmp(suspeitoPista, suspeito) == 0) {
        count++;
    }
    
    if (raiz->direita != NULL) {
        count += contarPistasPorSuspeito(raiz->direita, suspeito);
    }
    
    return count;
}

// Função para explorar as salas e coletar pistas
void explorarSalas(Sala* salaAtual, PistaNode** raizPistas) {
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
                    explorarSalas(salaAtual->esquerda, raizPistas);
                    escolhaValida = 1;
                } else {
                    printf("Não há saída pela esquerda! Escolha outra direção.\n");
                }
                break;
                
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    explorarSalas(salaAtual->direita, raizPistas);
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

// Função para verificar se o suspeito acusado é o culpado
void verificarSuspeitoFinal(PistaNode* raizPistas) {
    printf("\n\n=== FASE DE ACUSAÇÃO ===\n");
    printf("Suspeitos disponíveis:\n");
    for (int i = 0; i < MAX_SUSPEITOS; i++) {
        printf("  %d. %s\n", i + 1, suspeitos[i]);
    }
    
    int escolha;
    printf("\nQuem você acusa como culpado? (Digite o número): ");
    scanf("%d", &escolha);
    
    if (escolha < 1 || escolha > MAX_SUSPEITOS) {
        printf("Acusação inválida! O caso será arquivado.\n");
        return;
    }
    
    char* suspeitoAcusado = suspeitos[escolha - 1];
    int pistasEncontradas = contarPistasPorSuspeito(raizPistas, suspeitoAcusado);
    
    printf("\n=== VERDICT ===\n");
    printf("Suspeito acusado: %s\n", suspeitoAcusado);
    printf("Pistas que apontam para %s: %d\n", suspeitoAcusado, pistasEncontradas);
    
    if (pistasEncontradas >= MIN_PISTAS_CULPADO) {
        printf("✅ PARABÉNS! Você reuniu evidências suficientes.\n");
        printf("%s é declarado CULPADO pelo crime!\n", suspeitoAcusado);
    } else {
        printf("❌ INSUFICIENTE! Você não tem provas concretas.\n");
        printf("%s é INOCENTE por falta de evidências.\n", suspeitoAcusado);
        printf("É necessário pelo menos %d pistas para condenar um suspeito.\n", MIN_PISTAS_CULPADO);
    }
}

// Função para liberar a memória da árvore de salas
void liberarArvoreSalas(Sala* raiz) {
    if (raiz == NULL) return;
    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}

// Função para liberar a memória da árvore de pistas
void liberarArvorePistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

// Função para liberar a memória da tabela hash
void liberarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashEntry* atual = tabelaHash[i];
        while (atual != NULL) {
            HashEntry* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
}

int main() {
    printf("=== Detective Quest - O Caso Final ===\n");
    printf("Bem-vindo, detetive! Resolva o mistério coletando pistas e acusando o culpado.\n");
    printf("Você precisa de pelo menos %d pistas contra um suspeito para condená-lo.\n\n", MIN_PISTAS_CULPADO);
    
    // Inicializar tabela hash
    inicializarHash();
    
    // Configurar associações pista-suspeito na tabela hash
    inserirNaHash("Porta principal trancada por dentro", "Carlos");
    inserirNaHash("Copo de vinho quebrado no chão", "Maria");
    inserirNaHash("Arranhões na parede", "João");
    inserirNaHash("Livro sobre venenos aberto na mesa", "Ana");
    inserirNaHash("Terra molhada perto da janela", "Pedro");
    inserirNaHash("Faca desaparecida do bloco de facas", "Maria");
    inserirNaHash("Botão de camisa encontrado no degrau", "Carlos");
    inserirNaHash("Documento sigiloso na gaveta", "Ana");
    inserirNaHash("Partitura com anotações suspeitas", "João");
    inserirNaHash("Pó branco em um saquinho", "Pedro");
    inserirNaHash("Pegadas que levam ao muro", "Carlos");
    inserirNaHash("Mala semi-aberta com documentos", "Maria");
    inserirNaHash("Quadro levemente desalinhado", "João");
    inserirNaHash("Relógio parado às 23:47", "Ana");
    inserirNaHash("Computador com email aberto", "Pedro");
    
    // Montagem do mapa da mansão (árvore binária) com pistas
    Sala* hallEntrada = criarSala("Hall de Entrada", "Porta principal trancada por dentro");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo de vinho quebrado no chão");
    Sala* corredorPrincipal = criarSala("Corredor Principal", "Arranhões na parede");
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto na mesa");
    Sala* jardimInverno = criarSala("Jardim de Inverno", "Terra molhada perto da janela");
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida do bloco de facas");
    Sala* escadaria = criarSala("Escadaria", "Botão de camisa encontrado no degrau");
    Sala* escritorio = criarSala("Escritório Secreto", "Documento sigiloso na gaveta");
    Sala* salaMusica = criarSala("Sala de Música", "Partitura com anotações suspeitas");
    Sala* despensa = criarSala("Despensa", "Pó branco em um saquinho");
    Sala* jardim = criarSala("Jardim", "Pegadas que levam ao muro");
    Sala* quartoHospedes = criarSala("Quarto de Hóspedes", "Mala semi-aberta com documentos");
    Sala* corredorSuperior = criarSala("Corredor Superior", "Quadro levemente desalinhado");
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
    explorarSalas(hallEntrada, &raizPistas);
    
    // Exibe todas as pistas coletadas
    printf("\n\n=== RELATÓRIO DE INVESTIGAÇÃO ===\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        printf("Pistas coletadas (ordenadas alfabeticamente):\n");
        exibirPistas(raizPistas);
    }
    
    // Fase de acusação final
    verificarSuspeitoFinal(raizPistas);
    
    // Libera a memória alocada
    liberarArvoreSalas(hallEntrada);
    liberarArvorePistas(raizPistas);
    liberarHash();
    
    printf("\n=== Detective Quest - Caso Encerrado ===\n");
    return 0;
}