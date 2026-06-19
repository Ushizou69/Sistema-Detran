/*
 * ============================================================
 * TRABALHO PRÁTICO — Estrutura de Dados
 * Grupo: G03 | Contexto: Detran
 * ============================================================
 * Parâmetros do grupo:
 *   Capacidade máxima da fila  : 10
 *   Capacidade do histórico    : 4
 *   Algoritmo de ordenação     : Selection Sort
 *   Algoritmo de busca         : Binária
 *   Tipo de prioridade         : Deficiente (prioritario=1)
 *   Campo extra no cadastro    : char cpf[15]
 *   Questão 4 do Diário        : inserir 8, remover 3
 *
 * Requisito único: Desfazer atendimento: comando 'desfazer' reinseire o último atendido de volta ao início da fila usando a pilha.
 * ============================================================
 * Contribuições de Guilherme Billig:
 *   - Ajustou a remoção para prioridades e reordenar a fila sem travar.
 *   - Corrigiu início/fim para evitar índice negativo no desfazer.
 *   - Converteu para UTF-8 para arrumar os acentos do código.
 * ============================================================
 *
 * TRACE MANUAL — (preencher aqui antes da entrega)
 * Algoritmo: Selection Sort
 * Dados iniciais: [4, 1, 3, 2]
 *
 * Passagem 1:
 * - Busca o menor valor entre [4, 1, 3, 2]. Menor é 1.
 * - Troca 4 com 1.
 * - Vetor resultante: [1, 4, 3, 2] | Trocas: 1
 *
 * Passagem 2:
 * - Busca o menor valor entre [4, 3, 2] (índices 1 a 3). Menor é 2.
 * - Troca 4 com 2.
 * - Vetor resultante: [1, 2, 3, 4] | Trocas: 2
 *
 * Passagem 3:
 * - Busca o menor valor entre [3, 4] (índices 2 a 3). Menor é 3.
 * - Nenhuma troca necessária.
 * - Vetor resultante: [1, 2, 3, 4] | Trocas: 2
 *
 * Resultado final: [1, 2, 3, 4]
 * Total de trocas: 2
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * DEFINIÇÃO DAS STRUCTS
 * ============================================================ */

/* Struct do cliente — adaptar conforme parâmetros do grupo */
typedef struct cliente {
    int senha;
    char nome[50];
    int prioritario;   /* 0 = normal, 1 = prioritário (Deficiente (prioritario=1)) */
    char cpf[15];        /* campo extra do grupo */
} Cliente;

/* Nó da lista encadeada — mantém nomes 'dado' e 'prox' das aulas */
typedef struct no {
    Cliente dado;
    struct no *prox;
} No;

/* Fila implementada com vetor */
typedef struct {
    Cliente itens[10];
    int inicio;
    int final;
    /* TODO G05: adicionar campo 'quantidade' aqui */
} Fila;

/* Pilha implementada com vetor */
typedef struct {
    Cliente itens[4];
    int topo;
} Pilha;

void limparBuffer() {
    int ch;
    while ((ch = fgetc(stdin)) != EOF && ch != '\n') {}
}

/* ============================================================
 * FUNÇÕES DA FILA
 * ============================================================ */

/* Inicializa a fila
   [Guilherme Billig]: Começa em 10000 para a fila não bugar com índice negativo
   quando usarmos o 'desfazer' (já que decrementamos o início). */
void filaInicializar(Fila *f) {
    f->inicio = 10000;
    f->final  = 10000;
}

/* Retorna 1 se a fila estiver vazia, 0 caso contrário */
int filaVazia(Fila *f) {
	
	if(f->inicio == f->final){
		return 1;	
	}
    return 0;
}

/* Retorna 1 se a fila estiver cheia, 0 caso contrário */
int filaCheia(Fila *f) {
	
	if((f->final - f->inicio) == 10){
		return 1;	
	}
    return 0;
}

/* Insere cliente na fila */
void filaInserir(Fila *f, Cliente c) {
	int fim = 0;
	
	if(filaCheia(f) == 1){
		printf("Fila cheia!\n");
		exit(0);
	}
	
	fim = f->final % 10;
	f->itens[fim] = c;
	f->final++;
	printf("%s inserido!\n", c.nome);
    /* Lembrar: fila circular usa (f->final % 10) */
}

/* Remove e retorna o primeiro cliente da fila
   [Guilherme Billig]: Ajustado para achar o prioritário primeiro e puxar o resto
   dos clientes para frente na fila circular sem dar loop infinito. */
Cliente filaRemover(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        Cliente vazio = {0};
        return vazio;
    }

    int i, indiceParaRemover = f->inicio;

    // 1. Procurar o primeiro cliente prioritário na fila
    for (i = 0; i < (f->final - f->inicio); i++) {
        int pos = (f->inicio + i) % 10;
        if (f->itens[pos].prioritario == 1) {
            indiceParaRemover = f->inicio + i;
            break;
        }
    }

    // 2. Armazena o cliente que será removido
    Cliente atendido = f->itens[indiceParaRemover % 10];

    // 3. Remover o elemento e deslocar os itens para trás (para "fechar o buraco")
    for (i = indiceParaRemover; i < f->final - 1; i++) {
        f->itens[i % 10] = f->itens[(i + 1) % 10];
    }

    // 4. Ajusta o ponteiro final
    f->final--;

    printf("%s removido (Prioridade: %d)!\n", atendido.nome, atendido.prioritario);
    return atendido;
}

/* Exibe o estado atual da fila */
void filaExibir(Fila *f) {
	int i = 0;
	
	printf("Fila: \n");
	
	for(i = 0; i < (f->final - f->inicio); i++){
		int pos = (f->inicio + i) % 10;
		 printf("[Senha %d Nome %s]\n",f->itens[pos].senha, f->itens[pos].nome);
	}
}

/* Insere cliente na primeira posição da fila (usado pelo desfazer)
   [Guilherme Billig]: Só decrementa o início linearmente para o cálculo do
   tamanho da fila (f->final - f->inicio) continuar funcionando certo. */
void filaInserirInicio(Fila *f, Cliente c) {
    if (filaCheia(f)) {
        printf("Erro: fila cheia. Não é possível desfazer atendimento.\n");
        return;
    }
    /* Move o início para trás monotonicamente */
    f->inicio--;
    f->itens[f->inicio % 10] = c;
}

/* ============================================================
 * FUNÇÕES DA PILHA (HISTÓRICO)
 * ============================================================ */

/* Inicializa a pilha */
void pilhaInicializar(Pilha *p) {
    p->topo = -1;
}

/* Retorna 1 se a pilha estiver vazia, 0 caso contrário */
int pilhaVazia(Pilha *p) {
    if (p->topo == -1)
        return 1;
    return 0;
}
/* Retorna 1 se a pilha estiver cheia, 0 caso contrário */
int pilhaCheia(Pilha *p) {
    if (p->topo == 3)
        return 1;
    return 0;
}

/* Empilha cliente no histórico */
void pilhaEmpilhar(Pilha *p, Cliente c) {
    int i;

    if (pilhaCheia(p)) {
        for (i = 0; i < p->topo; i++) {
            p->itens[i] = p->itens[i + 1];
        }
        p->topo--;
    }

    p->topo++;
    p->itens[p->topo] = c;
}
/* Desempilha e retorna o topo */
Cliente pilhaDesempilhar(Pilha *p) {
    Cliente vazio = {0};

    if (pilhaVazia(p)) {
        printf("Historico vazio!\n");
        return vazio;
    }

    vazio = p->itens[p->topo];
    p->topo--;

    return vazio;
}

/* Exibe o histórico de atendimentos */
void pilhaExibir(Pilha *p) {
    int i;

    if (pilhaVazia(p)) {
        printf("Historico vazio!\n");
        return;
    }

    printf("=== Historico de Atendimentos ===\n");

    for (i = p->topo; i >= 0; i--) {
        printf("[Senha %d Nome %s]\n",
               p->itens[i].senha,
               p->itens[i].nome);
    }
}
/* ============================================================
 * FUNÇÕES DA LISTA ENCADEADA
 * ============================================================ */

/* Insere cliente na lista */
No* listaInserir(No *inicio, Cliente c) {
    No *noh = (No*)malloc(sizeof(No));

    if (noh == NULL) {
        printf("Erro de memoria!\n");
        return inicio;
    }

    noh->dado = c;
    noh->prox = inicio;

    return noh;
}

/* Remove cliente da lista pelo número da senha */
No* listaRemover(No *inicio, int senha) {

    No *atual = inicio;
    No *anterior = NULL;

    while (atual != NULL && atual->dado.senha != senha) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Cliente nao encontrado!\n");
        return inicio;
    }

    if (anterior == NULL) {
        inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    return inicio;
}
/* Exibe todos os clientes da lista */
void listaExibir(No *inicio) {

    No *aux = inicio;

    if (aux == NULL) {
        printf("Lista vazia!\n");
        return;
    }

    printf("=== Lista de Clientes ===\n");

    while (aux != NULL) {
        printf("Senha: %d\n", aux->dado.senha);
        printf("Nome: %s\n", aux->dado.nome);
        printf("CPF: %s\n", aux->dado.cpf);
        printf("Prioridade: %s\n",
               aux->dado.prioritario ? "Sim" : "Nao");
        printf("---------------------\n");

        aux = aux->prox;
    }
}
/* Libera toda a memória da lista */
No* listaApagar(No *inicio) {
    No *atual = inicio;
    while (atual != NULL) {
        No *t = atual->prox;
        free(atual);
        atual = t;
    }
    return NULL;
}


/* ============================================================
 * ALGORITMO DE BUSCA — Binária
 * ============================================================ */

/*
 * IMPORTANTE: a busca binária exige que o vetor esteja ordenado.
 * Antes de chamar buscaBinaria(), ordenar o vetor com Selection Sort.
 */

/* Busca sequencial por senha — percorre elemento a elemento */
int buscaSequencial(Cliente vet[], int n, int senha) {
	int i = 0;
	
	for(i = 0; i < n; i++){
		if(senha == vet[i].senha)
			return i;
		else if (senha < vet[i].senha)
			return -1;
	}
	return -1;
	
}

void ordenar(Cliente vet[], int n);
/* Busca binária por senha — exige vetor ordenado */
int buscaBinaria(Cliente vet[], int n, int senha) {
    int low = 0, high = n - 1;
    
    ordenar(vet, n);
	
	while(low <= high){
		int meio = (low + high) / 2;
		if (senha < vet[meio].senha)
			high = meio - 1;
		else if (senha > vet[meio].senha)
			low = meio + 1;
		else
			return meio;
	}
	return -1;
    
}


/* ============================================================
 * ALGORITMO DE ORDENAÇÃO — Selection Sort
 * ============================================================ */

/* Ordena vetor de clientes por número de senha (crescente) */
void ordenar(Cliente vet[], int n) {
    /*
     * TODO: implementar Selection Sort
     *
     * G09: contar e retornar (ou exibir) o número de trocas
     * G10: implementar dois critérios (por senha e por nome)
     *      — use um parâmetro int criterio (0=senha, 1=nome)
     */
    int trocas = 0;
    int i, j, min_index;
    for (i = 0; i < n - 1; i++) {
        min_index = i;
        for (j = i + 1; j < n; j++) {
            if (vet[j].senha < vet[min_index].senha) {
                min_index = j;
            }
        }
        if (min_index != i) {
            Cliente temp = vet[i];
            vet[i] = vet[min_index];
            vet[min_index] = temp;
            trocas++;
        }
    }
    printf("Selection Sort concluído. Total de trocas: %d\n", trocas);
}


/* ============================================================
 * RELATÓRIO
 * ============================================================ */


/* Gera relatório de atendimentos ordenado */
void gerarRelatorio(Cliente historico[], int n) {
    /* TODO:
     * 1. Copiar histórico para vetor auxiliar
     * 2. Ordenar com Selection Sort
     * 3. Exibir
     * G04: filtrar por intervalo de senhas (usa busca binária)
     * G10: perguntar critério ao usuário antes de ordenar
     */
    if (n == 0) {
        printf("Nenhum atendimento no histórico para gerar relatório.\n");
        return;
    }


    /* 1. Copiar histórico para vetor auxiliar */
    Cliente aux[4];
    int i;
    for (i = 0; i < n; i++) {
        aux[i] = historico[i];
    }


    /* 2. Ordenar com Selection Sort por senha */
    ordenar(aux, n);


    /* 3. Exibir relatório ordenado */
    printf("\n=== Relatório de Atendimentos (ordenado por senha) ===\n");
    printf("Senha | Nome | CPF | Prioridade");
    printf("--------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%d %s %s %s\n",
               aux[i].senha,
               aux[i].nome,
               aux[i].cpf,
               aux[i].prioritario ? "Prioritario" : "Normal");
    }
}



/* ============================================================
 * REQUISITO ÚNICO DO GRUPO G03
 * ============================================================ */

/*
 * Desfazer atendimento: comando 'desfazer' reinseire o último atendido de volta ao início da fila usando a pilha.
 * TODO: implementar a função (ou funções) necessárias aqui.
 */
void desfazerAtendimento(Fila *f, Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Nenhum atendimento para desfazer.\n");
        return;
    }
    if (filaCheia(f)) {
        printf("Fila cheia. Não é possível desfazer o atendimento agora.\n");
        return;
    }
    Cliente c = pilhaDesempilhar(p);
    filaInserirInicio(f, c);
    printf("Atendimento desfeito: %s (Senha %03d) voltou ao início da fila.\n",
           c.nome, c.senha);
}

/* ============================================================
 * FUNÇÃO PRINCIPAL
 * ============================================================ */

int main() {
    Fila   fila;
    Pilha  historico;
    No    *lista   = NULL;
    int    opcao   = 0;
    int    proxSenha = 1;

    filaInicializar(&fila);
    pilhaInicializar(&historico);

    printf("=== Sistema de Atendimento — Detran ===\n\n");

    do {
        printf("\n--- MENU ---\n");
        printf("1. Cadastrar novo cliente\n");
        printf("2. Chamar proximo cliente\n");
        printf("3. Buscar cliente\n");
        printf("4. Exibir fila atual\n");
        printf("5. Exibir historico (pilha)\n");
        printf("6. Exibir lista de clientes\n");
        printf("7. Gerar relatorio ordenado\n");
        printf("8. Desfazer ultimo atendimento\n");
        /* TODO G07: o menu de chamar já deve aplicar intercalação */
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: {
                Cliente c = {0};
                c.senha = proxSenha++;
                printf("Nome: ");
                scanf(" %49s", c.nome);
                limparBuffer();
                printf("Prioritario? (1=sim, 0=nao) [Deficiente (prioritario=1)]: ");
                scanf("%d", &c.prioritario);
                limparBuffer();
                printf("char cpf[15]: ");
                scanf(" %14s", c.cpf);
                limparBuffer();

                filaInserir(&fila, c);
                lista = listaInserir(lista, c);
                printf("Cliente cadastrado. Senha: %03d\n", c.senha);
                break;
            }
            case 2: {
                if (filaVazia(&fila)) {
                    printf("Fila vazia.\n");
                } else {
                    Cliente atendido = filaRemover(&fila);
                    pilhaEmpilhar(&historico, atendido);
                    printf("Atendendo: %s (Senha %03d)\n",
                           atendido.nome, atendido.senha);
                }
                break;
            }
            case 3: {
                int senha;
                printf("Numero da senha: ");
                scanf("%d", &senha);
                limparBuffer();
                /* Converter lista para vetor auxiliar antes da busca */

                /* Contar elementos da lista */
                int count = 0;
                No *temp = lista;
                while (temp != NULL) { count++; temp = temp->prox; }

                if (count == 0) {
                    printf("Nenhum cliente cadastrado.\n");
                    break;
                }

                /* Copiar lista para vetor auxiliar */
                Cliente *vet = (Cliente*)malloc(count * sizeof(Cliente));
                temp = lista;
                int i = 0;
                while (temp != NULL) {
                    vet[i++] = temp->dado;
                    temp = temp->prox;
                }

                /* Ordenar antes da busca binária */
                ordenar(vet, count);

                /* Busca binária */
                int indice = buscaBinaria(vet, count, senha);
                if (indice >= 0) {
                    printf("Cliente encontrado: %s | CPF: %s | %s\n",
                           vet[indice].nome,
                           vet[indice].cpf,
                           vet[indice].prioritario ? "Prioritario" : "Normal");
                } else {
                    printf("Cliente com senha %03d não encontrado.\n", senha);
                }
                free(vet);
                break;
            }
            case 4: filaExibir(&fila);      break;
            case 5: pilhaExibir(&historico); break;
            case 6: listaExibir(lista);      break;
            case 7: {
                /* Copiar pilha de histórico para vetor */
                int n = historico.topo + 1;
                Cliente vet[4];
                int i;
                for (i = 0; i <= historico.topo; i++) {
                    vet[i] = historico.itens[i];
                }
                gerarRelatorio(vet, n);
                break;
            }
            case 8: {
                /* Requisito único G03: Desfazer último atendimento */
                desfazerAtendimento(&fila, &historico);
                break;
            }
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    lista = listaApagar(lista);
    return 0;
}
