
/*  Software para o trabalho final de laboratório de ICC, professor Jó
    Alunos: Arthur Gomes Mesquita/ Allan Vitor/ Matheus Aguiar  */

#include <stdio.h>
#include <stdlib.h>
#define MAX 100

typedef struct {
  char nome[MAX];
  int nusp;
  int senha;
} t_usuario;

typedef struct {
  char nome_livro[MAX];
  char autor[MAX];
  int ano_publicacao;
  int codigo;
  int volume;
  int edicao;
  char editora[MAX];
} t_livro;

void cadastro_livro(t_livro *livro);
void cadastro_pessoa(t_usuario *usuario);

int main(void) {
  int opcao;
  t_livro *livros = malloc(sizeof(t_livro));
  t_usuario *usuarios = malloc(sizeof(t_usuario));

  printf(">>>>Bem-vindo<<<<\n\n");
  printf("Escolha uma opcao: \n");
  printf("(1) Cadastro Livro\n");
  printf("(2) Cadastro Usuario\n");

  scanf("%d", &opcao);

  switch (opcao) {
    case 1:
      cadastro_livro(livros);
      break;
    case 2:
       cadastro_usuario(usuarios);
      break;
    default:break;
  }

  free(livros); // Libera a memória alocada para a estrutura t_livro
  return 0;
}

void cadastro_usuario (t_usuario *usuario)
{
  printf("Insira o nome do usuario: ");
  fflush(stdin);
  fgets (usuario->nome, 100, stdin);
//  printf("%s\n", usuario->nome);

  printf ("Digite o numero usp: ");
  scanf ("%d", &usuario->nusp);
// printf ("%d", usuario->nusp);

  printf ("Digite a senha de 4 a 8 digitos: ");
  scanf ("%d", &usuario->senha);
  printf ("%d", usuario->senha);
}

void cadastro_livro(t_livro *livro) 
{
  printf("Insira o nome do livro: ");
  fflush(stdin);
  fgets (livro->nome_livro, 100, stdin);
//  printf("%s\n", livro->nome_livro);
  
  printf ("Digite o nome do autor: ");
  fflush(stdin);
  fgets (livro->autor, 100, stdin);
 // printf("%s\n", livro->autor);

  printf ("Digite o ano de publicacao: ");
  scanf ("%d", &livro->ano_publicacao);
//  printf ("%d", livro->ano_publicacao);

  printf ("Digite o codigo: ");
  scanf ("%d", &livro->codigo);
  //printf ("%d", livro->codigo);

  printf ("Digite o volume: ");
  scanf ("%d", &livro->volume);
  //printf ("%d", livro->volume);

  printf ("Digite a edicao: ");
  scanf ("%d", &livro->edicao);
  //printf ("%d", livro->edicao);

   printf ("Digite o nome da editora: ");
  fflush(stdin);
  fgets (livro->editora, 100, stdin);
//  printf("%s\n", livro->editora);
  
}
