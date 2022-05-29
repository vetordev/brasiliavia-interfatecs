#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct Grupo {
  int seguro;
  int ativo;
  int qtdPessoas;
  int *pessoas; // ARRAY
} Grupo;

typedef struct DescritorG {
  int qtdGrupos;
  int qtdGruposSeguros;
  Grupo *grupos; // ARRAY
} DescritorG;

Grupo* encontrarGrupoPorPessoa(DescritorG *descritor, int pessoa) {

  for (int i = 0; i < descritor->qtdGrupos; i++) {
      Grupo *grupo = descritor->grupos + i;

      for (int j = 0; j < grupo->qtdPessoas; j++) {

        if (grupo->pessoas[j] == pessoa && grupo->ativo == TRUE) return grupo;
      }
  }
}

// Comandos
void contatoPessoas(DescritorG *descritor, int pessoaA, int pessoaB) {
  Grupo *grupoPessoaA = encontrarGrupoPorPessoa(descritor, pessoaA);
  Grupo *grupoPessoaB = encontrarGrupoPorPessoa(descritor, pessoaB);

  int novaQtdPessoas = grupoPessoaA->qtdPessoas + grupoPessoaB->qtdPessoas;
  grupoPessoaA->pessoas = realloc(grupoPessoaA->pessoas, sizeof(int) * novaQtdPessoas);

  for (int i = 0; i < grupoPessoaB->qtdPessoas; i++) {
    grupoPessoaA->pessoas[grupoPessoaA->qtdPessoas + i] = grupoPessoaB->pessoas[i];
  }

  grupoPessoaA->qtdPessoas = novaQtdPessoas;

  if (grupoPessoaB->seguro == FALSE) grupoPessoaA->seguro = FALSE;

  // "desativei" este grupo
  grupoPessoaB->ativo = FALSE;
  descritor->qtdGrupos--;
  descritor->qtdGruposSeguros--;
}

void confirmarPessoaContaminada(DescritorG *descritor, int pessoa) {
  Grupo *grupoPessoa = encontrarGrupoPorPessoa(descritor, pessoa);

  grupoPessoa->seguro = FALSE;
  descritor->qtdGruposSeguros--;
}

void exibirQtdGrupos(DescritorG *descritor) {
  printf("%d\n", descritor->qtdGrupos);
}

void exibirQtdGruposSeguros(DescritorG *descritor) {
  printf("%d\n", descritor->qtdGruposSeguros);
}

void exibirQtdGruposInseguros(DescritorG *descritor) {
  printf("%d\n", descritor->qtdGrupos - descritor->qtdGruposSeguros);
}

void exibirPessoasInseguras(DescritorG *descritor) {

  int todosSeguros = TRUE;
  int len = descritor->qtdGrupos;
  for (int i = 0; i < len; i++) {
    Grupo *grupo = descritor->grupos + i;

    if (grupo->ativo == FALSE) len++;

    if (grupo->seguro == FALSE) {
      todosSeguros = FALSE;

      for (int j = 0; j < grupo->qtdPessoas; j++) {
        if (j == grupo->qtdPessoas - 1) printf("%d\n", grupo->pessoas[j]);
        else printf("%d ", grupo->pessoas[j]);
      }
    }
  }

  if (todosSeguros) printf("vazio\n");
}

int main() {

  int P, E, i; // Quantide de Pessoas e Eventos

  scanf("%d %d", &P, &E);

  DescritorG descritor = {P, P, malloc(sizeof(Grupo) * P)};

  // Inicializa os grupos unitários
  for (i = 0; i < P; i++) {
    Grupo *grupo = descritor.grupos + i;

    grupo->seguro = TRUE;
    grupo->ativo = TRUE;
    grupo->qtdPessoas = 1;
    grupo->pessoas = malloc(sizeof(int) * grupo->qtdPessoas);


    grupo->pessoas[0] = i;
  }

  for (i = 0; i < E; i++) { // Recebimento de eventos
    char comando[2];
    int pessoaA, pessoaB;

    scanf("%s %d %d", comando, &pessoaA, &pessoaB);

    if (comando[0] == 'c') contatoPessoas(&descritor, pessoaA, pessoaB);
    if (comando[0] == 'p') confirmarPessoaContaminada(&descritor, pessoaA);
    if (comando[0] == 'n' && comando[1] == 0) exibirQtdGrupos(&descritor);
    if (comando[0] == 'n' && comando[1] == 's') exibirQtdGruposSeguros(&descritor);
    if (comando[0] == 'n' && comando[1] == 'i') exibirQtdGruposInseguros(&descritor);
    if (comando[0] == 'i' && comando[1] == 'i') exibirPessoasInseguras(&descritor);
  }

  free(descritor.grupos);

  return 0;
}