#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int total;
    int emprestimo;
    char editora[MAX];
} t_livro;

void cadastro_livro(t_livro *livro, FILE*arquivo, int num_livro);
void cadastro_usuario(t_usuario *usuario);
void pesquisa_livro(FILE *arquivo);
void pesquisa_nome(FILE *arqui);
void pesquisa_autor (FILE *arqui);
void pesquisa_codigo(FILE *arqui);
void emprestimo(FILE*arqui);

int main() {
    int opcao;
    t_livro *livros = (t_livro*)malloc(sizeof(t_livro));
    t_usuario *usuarios = (t_usuario*)malloc(sizeof(t_usuario));
    FILE *arq;
    int qnt_livro=1;

    printf(">>>>Bem-vindo<<<<");
    for(int i=0;i<1;){
        printf("\n\nEscolha uma opcao: \n");
        printf("(1) Cadastro Livro\n");
        printf("(2) Cadastro Usuario\n");
        printf("(3) Pesquisar livro\n");
        printf ("(4) Emprestimo\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                qnt_livro++;
                cadastro_livro(livros, arq, qnt_livro);
                livros = realloc(livros, sizeof(t_livro)*qnt_livro);
                break;
            case 2:
                cadastro_usuario(usuarios);
                break;
            case 3:
                pesquisa_livro(arq);
                break;
            case 4:
                emprestimo(arq);
                break;
            default:
                i=2;
                break;
        }
    }

    free(livros); // Libera a memória alocada para a estrutura t_livro
    return 0;
    }

void emprestimo(FILE*arqui)
{
    int codigo2;
    int emprestados;
    printf("Digite o codigo do livro que deseja fazer emprestimo: ");
    scanf("%d",&codigo2);

    arqui = fopen("Estoque.bin", "rb");

    t_livro livro;
    int livro_encontrado = 0;

    while (fread(&livro, sizeof(t_livro), 1, arqui) == 1) {
        if (livro.codigo==codigo2) {
            printf("\nLivro encontrado:\n");
            printf("Nome: %s", livro.nome_livro);
            printf("Autor: %s", livro.autor);
            printf("Quantidade Total: %d\n", livro.total);
            printf("Quantidade disponivel: %d\n", livro.emprestimo);
            printf("\n");
            livro_encontrado =1;
        }
    }
    if (livro_encontrado==0) {
      printf("\nLivro nao encontrado");
      return;
    }
    printf("Digite quantos livros deseja fazer emprestimo: ");
    scanf("%d", &emprestados);

    fclose(arqui);

    arqui = fopen("Estoque.bin", "r+b");
      while (fread(&livro, sizeof(t_livro), 1, arqui) == 1) {
      if (livro.codigo==codigo2) {
          livro.emprestimo = livro.emprestimo - emprestados;
          fseek(arqui, -(long)sizeof(livro), SEEK_CUR);
          fwrite(&livro, sizeof(livro), 1, arqui);
          printf ("\nEMprestimo realizado com sucesso!\n");
          break;
      }
   }
   fclose(arqui);
}

void pesquisa_livro(FILE *arquivo)
{
    int opcao2;

    printf("Por qual mecanismo deseja pesquisar?\n");
    printf("(1) Nome do livro\n");
    printf("(2) Codigo do livro\n");
    printf("(3) Autor\n");
    scanf("%d",&opcao2);

    switch (opcao2){
        case 1:
            pesquisa_nome(arquivo);
            break;
        case 2:
            pesquisa_codigo(arquivo);
            break;
        case 3:
            pesquisa_autor(arquivo);
            break;
    }
}

void pesquisa_nome(FILE *arqui)
{
    char p_nome[MAX]; //Variável que recebe o nome do livro
    printf("Digite o nome do livro: ");
    fflush(stdin);
    fgets(p_nome, MAX, stdin);
//    p_nome[strcspn(p_nome, "\n")] = '\0';  // Remover o caractere '\n' da entrada

    arqui = fopen("Estoque.bin", "rb");
    if (arqui == NULL) {
        printf("Erro ao abrir o arquivo.\n\n");
        return;
    }

    t_livro livro;
    int livro_encontrado =0;

    while (fread(&livro, sizeof(t_livro), 1, arqui) == 1) {
        if (strncmp(livro.nome_livro, p_nome, strlen(p_nome)) == 0) {
            printf("\nLivro encontrado:\n");
            printf("Nome: %s", livro.nome_livro);
            printf("Autor: %s", livro.autor);
            printf("Ano de Publicacao: %d\n", livro.ano_publicacao);
            printf("Codigo: %d\n", livro.codigo);
            printf("Volume: %d\n", livro.volume);
            printf("Edicao: %d\n", livro.edicao);
            printf("Editora: %s", livro.editora);
            printf("Quantidade Total: %d\n", livro.total);
            printf("Quantidade disponivel: %d\n", livro.emprestimo);
            printf("\n");
            livro_encontrado =1;
        }
    }
    if (livro_encontrado==0) printf("\nLivro nao encontrado");
    fclose(arqui);
}

void pesquisa_autor(FILE*arqui)
{
   char p_nome[MAX]; //Variável que recebe o nome do livro
    printf("Digite o nome do autor: ");
    fflush(stdin);
    fgets(p_nome, MAX, stdin);
//    p_nome[strcspn(p_nome, "\n")] = '\0';  // Remover o caractere '\n' da entrada

    arqui = fopen("Estoque.bin", "rb");
    if (arqui == NULL) {
        printf("Erro ao abrir o arquivo.\n\n");
        return;
    }

    t_livro livro;
    int livro_encontrado =0;

    while (fread(&livro, sizeof(t_livro), 1, arqui) == 1) {
        if (strncmp(livro.autor, p_nome, strlen(p_nome)) == 0) {
            printf("\nLivro encontrado:\n");
            printf("Nome: %s", livro.nome_livro);
            printf("Autor: %s", livro.autor);
            printf("Ano de Publicacao: %d\n", livro.ano_publicacao);
            printf("Codigo: %d\n", livro.codigo);
            printf("Volume: %d\n", livro.volume);
            printf("Edicao: %d\n", livro.edicao);
            printf("Editora: %s", livro.editora);
            printf("Quantidade Total: %d\n", livro.total);
            printf("Quantidade disponivel: %d\n", livro.emprestimo);
            printf("\n");
            livro_encontrado =1;
        }
    }
    if (livro_encontrado==0) printf("\nLivro nao encontrado");
    fclose(arqui);
}

void pesquisa_codigo(FILE *arqui)
{
    int p_codigo; //Variável que recebe o codigo do livro digitado pelo usuario
    printf("Digite o codigo do livro: ");
    scanf("%d",&p_codigo);
//    p_nome[strcspn(p_nome, "\n")] = '\0';  // Remover o caractere '\n' da entrada

    arqui = fopen("Estoque.bin", "rb");
    if (arqui == NULL) {
        printf("Erro ao abrir o arquivo.\n\n");
        return;
    }

    t_livro livro;
    int livro_encontrado =0;

    while (fread(&livro, sizeof(t_livro), 1, arqui) == 1) {
        if (livro.codigo==p_codigo) {
            printf("\nLivro encontrado:\n");
            printf("Nome: %s", livro.nome_livro);
            printf("Autor: %s", livro.autor);
            printf("Ano de Publicacao: %d\n", livro.ano_publicacao);
            printf("Codigo: %d\n", livro.codigo);
            printf("Volume: %d\n", livro.volume);
            printf("Edicao: %d\n", livro.edicao);
            printf("Editora: %s", livro.editora);
            printf("Quantidade Total: %d\n", livro.total);
            printf("Quantidade disponivel: %d\n", livro.emprestimo);
            printf("\n");
            livro_encontrado =1;
        }
    }
    if (livro_encontrado==0) printf("\nLivro nao encontrado");
    fclose(arqui);
}

void cadastro_usuario (t_usuario *usuario)
{
    printf("Insira o nome do usuario: ");
    fflush(stdin);
    fgets (usuario->nome, MAX, stdin);
//  printf("%s\n", usuario->nome);

    printf ("Digite o numero usp: ");
    scanf ("%d", &usuario->nusp);
// printf ("%d", usuario->nusp);

    printf ("Digite a senha de 4 a 8 digitos: ");
    scanf ("%d", &usuario->senha);
    printf ("%d", usuario->senha);
}

void cadastro_livro(t_livro *livro, FILE*arquivo, int num_livro)
{
  //  livro = realloc(livro, sizeof(t_livro)*num_livro);

    printf("Insira o nome do livro: ");
    fflush(stdin);
    fgets (livro->nome_livro, MAX, stdin);
//  printf("%s\n", livro->nome_livro);

    printf ("Digite o nome do autor: ");
    fflush(stdin);
    fgets (livro->autor, MAX, stdin);
    //printf("%s\n", livro->autor);

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
    fgets (livro->editora, MAX, stdin);
//  printf("%s\n", livro->editora);

    printf ("Digite a quantidade total de livros: ");
    scanf ("%d", &livro->total);
    livro->emprestimo = livro->total;
//  printf ("%d", livro->emprestimo);

    arquivo=fopen("Estoque.bin", "ab");
    if (arquivo==NULL) printf ("Erro ao abrir o arquivo");
    fwrite(livro, sizeof(t_livro), 1, arquivo);

//    free (livro);
    fclose (arquivo);
}
