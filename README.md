# Sistema-Detran

## Descrição
Projeto desenvolvido para a disciplina de Estrutura de Dados para a Graduação do curso de Análise Desenvolvimento de Sistemas (ADS); este projeto simula o sistema de (Detran Departamento Estadual de Trânsito) e foi feito com os colaboradores Matheus N., Luiz F. e Guilherme B..

## Como instalar?

Vá em <> Code, baixe o arquivo .zip:

<img width="436" height="344" alt="image" src="https://github.com/user-attachments/assets/4fb66069-296b-47a0-bc49-99e588007968" />

e clique com botão direito e clique em extrair aqui:

<img width="306" height="532" alt="image" src="https://github.com/user-attachments/assets/70c62880-2ca3-4169-a21b-a99257b90008" />
<img width="302" height="113" alt="image" src="https://github.com/user-attachments/assets/ee35ed19-bc87-4b85-ae36-63b0f8adcfda" />

entre na pasta esqueleto e procure o arquivo esqueleto_G03_V6.exe e o clique duas vezes com o botão esquerdo, ou clique com o botão direito e clique em abrir.

<img width="640" height="471" alt="image" src="https://github.com/user-attachments/assets/788fde70-5d20-416b-bbc2-d65f2ca92cda" />
<img width="1115" height="628" alt="image" src="https://github.com/user-attachments/assets/e3456653-c736-4e0e-aa0e-749edfa3d7a8" />

## Funcionalidades

- Sistema de atendimento em fila com prioridade (deficientes têm atendimento preferencial),
- Cadastro de clientes com nome, CPF, senha automática e status de prioridade,
- Controle de fila utilizando estrutura circular com capacidade fixa,
- Histórico de atendimentos usando estrutura de pilha (últimos atendidos),
- Recurso de desfazer atendimento, retornando o último cliente atendido para o início da fila,
- Exibição da fila atual com ordenação de atendimento,
- Exibição do histórico completo de atendimentos,
- Lista encadeada para armazenamento e gerenciamento de clientes cadastrados,
- Remoção de clientes da lista encadeada por senha,
- Busca de clientes por senha com:
- Busca sequencial,
- Busca binária (com ordenação prévia via Selection Sort),
- Algoritmo de ordenação Selection Sort com contagem de trocas,
- Geração de relatório ordenado de atendimentos,
- Interface de menu interativo em terminal (CLI),
- Validação de fila cheia e fila vazia,
- Controle de memória para lista encadeada (liberação total ao encerrar o sistema);

## Objetivo
Criar um sistema para simular o Detran, ajudando a mostrar como funciona estes sistemas de filas internamente, aprimorando o conhecimento;Colocar em prática os conhecimentos adquiridos durante a disciplina.

## DIÁRIO DE DESENVOLVIMENTO — Grupo G03 (Detran)

## 1. Quem implementou cada função?
--------------------------------
- Nicolas:
  * filaVazia
  * filaCheia
  * filaInserir
  * filaRemover (versão inicial circular)
  * filaExibir
  * buscaSequencial
  * buscaBinaria
- Matheus:
  * Trace Manual do Selection Sort
  * limparBuffer
  * gerarRelatorio
  * ordenar (Selection Sort)
  * filaInserirInicio (versão inicial)
  * desfazerAtendimento
  * main (menu interativo)
- Guilherme Billig:
  * Conversão de codificação do arquivo C para UTF-8 limpo.
  * Correção da inicialização e ponteiros em filaInicializar (iniciar em 10000 para evitar índices negativos).
  * Correção e otimização da busca por prioridades e deslocamento circular em filaRemover.
  * Correção do decremento circular monotônico em filaInserirInicio.
- Luiz:
  * Testes de validação da fila, pilha e lista encadeada.
  * Elaboração e preenchimento das respostas do Diário de Desenvolvimento.

## 2. Qual foi a maior dificuldade? Como resolveram?
-------------------------------------------------
A maior dificuldade foi fazer a fila circular funcionar junto com a função de desfazer e a remoção com prioridade. Ao desfazer o atendimento, o início da fila tem que ir para trás (inicio--). O problema é que, no C, fazer o resto da divisão de um número negativo (como -1 % 10) retorna -1, o que faz o programa dar crash ao tentar ler fora do vetor. 
A gente resolveu isso mudando a fila para iniciar em 10000 em vez de 0. Como 10000 é múltiplo de 10, o início do vetor continua na posição 0 (% 10), mas se fizermos o desfazer, a variável cai para 9999. Como 9999 % 10 é 9 (índice positivo e válido), o programa funciona perfeitamente sem dar tela azul. Também consertamos um loop de deslocamento dos elementos que estava travando o programa ao chamar clientes prioritários.

## 3. Por que o algoritmo de busca/ordenação atribuído ao grupo é adequado (ou não) para o contexto escolhido?
-------------------------------------------------------------------------------------------------------------
A busca binária é ótima para o nosso caso porque a gente consegue pesquisar um cliente de forma super rápida (em tempo O(log n)), mas ela exige que a lista de clientes esteja ordenada antes no vetor auxiliar.
Já o Selection Sort serve para o nosso trabalho porque a fila e o histórico são bem pequenos (máximo de 10 e 4 elementos), então ele ordena quase que instantaneamente. Mas se fosse um sistema real do Detran com milhares de pessoas por dia, ele seria muito lento e o ideal seria usar algo como o Quick Sort ou Insertion Sort.

## 4. Qual é o valor de inicio e final da fila após inserir [N] elementos e remover [M]? Mostre o estado do vetor. (N e M conforme tabela do grupo: N=8, M=3)
---------------------------------------------------------------------------------------------------------------------------------------------
- Fila circular clássica teórica (onde a remoção apenas incrementa o ponteiro inicio):
  * Começa em: inicio = 0, final = 0
  * Insere 8: inicio = 0, final = 8
  * Remove 3: inicio = 3, final = 8
  * Estado do vetor: os dados válidos ficam guardados nos índices de 3 a 7 do vetor.

- Fila implementada por nós (deslocando os elementos ao remover por prioridade):
  * Começa em: inicio = 10000, final = 10000
  * Insere 8: inicio = 10000, final = 10008
  * Remove 3: inicio = 10000, final = 10005 (como a remoção puxa os elementos da fila para frente, o inicio não muda)
  * Estado do vetor: os dados válidos ficam salvos nos índices físicos de 0 a 4 (mapeados por 10000 % 10 até 10004 % 10).

## 5. O algoritmo de ordenação do grupo é estável? Isso importa no contexto escolhido? Justifique.
-----------------------------------------------------------------------------------------------
O Selection Sort é instável porque ele pode trocar de posição elementos com chaves idênticas durante as passagens de busca do menor valor.
No nosso contexto (senhas do Detran), isso não faz diferença nenhuma porque as senhas geradas são sempre sequenciais e únicas (não tem como dois clientes terem o mesmo número de senha). Então, a instabilidade dele não altera em nada o resultado final das ordenações do relatório.

## 6. Demonstre o estado da lista encadeada após as três primeiras inserções com dados reais do contexto.
-------------------------------------------------------------------------------------------------------
Como a nossa lista insere os elementos no início (agindo como uma pilha LIFO), as inserções foram feitas na seguinte ordem:
1. Ana (Senha: 001, CPF: 111.111.111-11, Prioridade: 0)
2. Bruno (Senha: 002, CPF: 222.222.222-22, Prioridade: 1)
3. Carlos (Senha: 003, CPF: 333.333.333-33, Prioridade: 0)

O estado final da lista encadeada fica:
[lista] -> [Carlos (Senha 003, CPF: 333..., Prio: 0)] 
        -> [Bruno (Senha 002, CPF: 222..., Prio: 1)] 
        -> [Ana (Senha 001, CPF: 111..., Prio: 0)] 
        -> NULL
