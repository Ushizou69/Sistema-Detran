# Relatório de Implementações Técnico-Estruturais
**Aluno:** Guilherme Billig  
**Grupo:** G03 — Contexto: Detran  
**Disciplina:** Estrutura de Dados  

Este relatório documenta as correções e melhorias de lógica aplicadas na **Versão 6** do projeto de gerenciamento de fila de atendimentos com histórico.

---

## 1. Correção dos Ponteiros da Fila Circular (`filaInicializar`)

### O Problema
No C, a operação de módulo (`%`) aplicada a um número negativo retorna um resto negativo (por exemplo, `-1 % 10` resulta em `-1`). Na versão original da fila circular, as variáveis `inicio` e `final` começavam em `0`. Quando a operação de desfazer atendimento era executada, ela decrementava o ponteiro de início (`f->inicio--`), fazendo com que ele assumisse o valor `-1`. Isso gerava um acesso inválido à memória (`f->itens[-1]`), corrompendo os dados e travando o programa.

### A Solução
Ajustamos a inicialização da fila para começar no valor `10000` (um múltiplo da capacidade `10`).
* Como `10000 % 10 == 0`, a fila continua começando fisicamente no índice `0` do vetor.
* Quando o ponteiro recua na operação desfazer, ele cai para `9999`. Como `9999 % 10 == 9`, a fila acessa a última posição do vetor de forma segura e positiva, sem estourar a memória.

```c
/* Inicializa a fila
   [Guilherme Billig]: Começa em 10000 para a fila não bugar com índice negativo
   quando usarmos o 'desfazer' (já que decrementamos o início). */
void filaInicializar(Fila *f) {
    f->inicio = 10000;
    f->final  = 10000;
}
```

---

## 2. Remoção por Prioridade e Deslocamento Circular (`filaRemover`)

### O Problema
A remoção padrão de filas circulares apenas avança o ponteiro de início. Porém, o Detran necessita que clientes prioritários (`prioritario == 1`) passem à frente na chamada, o que exige buscar o cliente no meio da fila e remover o "buraco" deixado por ele, deslocando os clientes de trás para a frente. Na versão 5, o loop de deslocamento usava índices baseados em módulo dentro da condição do laço, gerando travamentos em loop infinito.

### A Solução
Implementamos a busca sequencial monotônica e o ajuste do loop usando limites lineares e seguros (`i < f->final - 1`), aplicando a operação de módulo `% 10` apenas no instante do acesso físico aos elementos do array:

```c
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
```

---

## 3. Reinserção de Elementos por Desfazer (`filaInserirInicio`)

### O Problema
O comando desfazer retira o último cliente atendido da Pilha (LIFO) de histórico e o coloca de volta na frente da Fila (FIFO). A manipulação de ponteiros na versão 5 decrementava o `inicio` aplicando o módulo direto na variável de controle, o que quebrava o cálculo matemático do tamanho da fila `(f->final - f->inicio)` usado nas checagens de fila cheia ou exibição.

### A Solução
Simplificamos a lógica para apenas decrementar `f->inicio` de forma contínua e linear (mantendo a consistência do tamanho da fila) e aplicando a indexação circular na hora de salvar o cliente no vetor:

```c
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
```
