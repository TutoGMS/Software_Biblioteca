#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

typedef struct {
    char nome_livro[MAX];
    char autor[MAX];
    int ano_publicacao;
    int codigo;
    int volume;
    int edicao;
    int total;
    int emprest_dispo; //Quantidade disponível para empréstimo
    char editora[MAX];
} t_livro;

typedef struct {
    char nome[MAX];
    int nusp;
    int senha;
    int qntlivro_emprestados; //Quantidade de livros diferentes que o usário possui
    t_livro livro_emprestado[3]; //Espaço para armazenar os livros do usuário (máximo de 3 livros)
} t_usuario;

void cadastro_livro(t_livro *livro, FILE*arquivo);
int verifica_codigo (int *codigo, FILE*arqui);
void cadastro_usuario(t_usuario *usuario, FILE*arqui);
int verifica_nusp(int *nusp, FILE* arqui);
void mostra_usuario(t_usuario *usuarios);
void mostra_livro (t_livro *livro);
void pesquisa_livro(FILE *arquivo); //Pesquisa do livro(geral)
void pesquisa_nome(FILE *arqui); //Pesquisa por nome do livro
void pesquisa_autor (FILE *arqui); //Pesquisa por autor
void pesquisa_codigo(FILE *arqui); //Pesquisa por código
void emprestimo(FILE*arqui1, FILE*arq2);
void devolucao(FILE*arqui1, FILE*arq2);
void listar_livros (FILE *arqui1);
void alterar_estoque(FILE*arqui1);

int main() {

    int opcao;
    t_livro *livros = (t_livro*)malloc(sizeof(t_livro));
    t_usuario *usuarios = (t_usuario*)malloc(sizeof(t_usuario));
    FILE *arq_usuario=NULL, *arq_livro=NULL;

    int qnt_livro=0;
    int qnt_usuario =0;

    printf(">>>>Bem-vindo<<<<");
    for(int i=0;i<1;){
        printf("\n\nEscolha uma opcao: \n");
        printf("(1) Cadastro Livro\n");
        printf("(2) Cadastro Usuario\n");
        printf("(3) Pesquisar livro\n");
        printf ("(4) Emprestimo\n");
        printf ("(5) Devolucao\n");
        printf ("(6) Listar livros\n");
        printf ("(7) Aumentar estoque\n");
        printf("(0) Para sair\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                qnt_livro++;
                cadastro_livro(livros, arq_livro);
                break;
            case 2:
                qnt_usuario++;
                cadastro_usuario(usuarios, arq_usuario);
                break;
            case 3:
                pesquisa_livro(arq_livro);
                break;
            case 4:
                emprestimo(arq_livro, arq_usuario);
                break;
            case 5:
                devolucao(arq_livro, arq_usuario);
                break;
            case 6:
                listar_livros(arq_livro);
                break;
            case 7:
                alterar_estoque(arq_livro);
                break;
            case 0:
                i=2;
                break;
        }
    }

    free(livros);
    free(usuarios);
    return 0;
}


void alterar_estoque(FILE*arqui1){

    int codigo; //codigo do livro
    int estoque;
    t_livro livro;
    int livro_encontrado = 0;

    printf("\nDigite a quantidade que quer incrementar: ");
    scanf("%d", &estoque);

    printf("\nDigite o codigo do livro que deseja alterar estoque: ");
    scanf("%d", &codigo);

    arqui1 = fopen("Estoque.bin", "r+b");

    while (fread(&livro, sizeof(t_livro), 1, arqui1) == 1) { //Permanece no loop enquanto estiver lendo dados do arquivo do tamanho determinado
        if (livro.codigo == codigo) {
            printf("\nLivro atualizado:");
            livro.emprest_dispo = livro.emprest_dispo + estoque; //Adiciona o incremento no disponivel para emprestimo
            livro.total = livro.total + estoque; //Adiciona o incremento ao estoque
            mostra_livro(&livro);
            livro_encontrado = 1;
            break;  // Saia do loop após encontrar o livro
        }
    }

    if (livro_encontrado==0) {
        printf("\nLivro nao encontrado");
        fclose(arqui1);
        return;
    }

    fseek(arqui1, -(long)sizeof(t_livro), SEEK_CUR); //posiciona o cursor do arquivo no livro desejado
    fwrite(&livro, sizeof(t_livro), 1, arqui1); //atualiza as informações do livro (altera empréstimo)
    fclose (arqui1);
}


void listar_livros (FILE *arqui1){

    arqui1 = fopen("Estoque.bin", "r+b");

    if (arqui1 == NULL) {
        printf("\nErro na abertura do arquivo de estoque\n");
        return;
    }

    t_livro livro;

    printf("\nLivros em estoque: \n");
    while (fread(&livro, sizeof(t_livro), 1, arqui1) == 1) { //Permanece no loop enquanto estiver lendo dados do arquivo
         mostra_livro(&livro);
    }

    fclose (arqui1);
}


void devolucao(FILE* arqui1, FILE* arqui2){

    int codigo; //codigo do livro
    int devolvidos; //quantidade que deseja devolver

    printf("\nDigite o codigo do livro que deseja devolver: ");
    scanf("%d", &codigo);

    arqui1 = fopen("Estoque.bin", "r+b");
    if (arqui1 == NULL) {
        printf("\nErro na abertura do arquivo de estoque\n");
        return;
    }

    arqui2 = fopen("Dados_usuario.bin", "r+b");
    if (arqui2 == NULL) {
        printf("\nErro na abertura do arquivo de dados do usuário\n");
        return;
    }

    t_livro livro;
    t_usuario usuario;

    int livro_encontrado = 0;

    while (fread(&livro, sizeof(t_livro), 1, arqui1) == 1) { //Permanece no loop enquanto estiver lendo dados do arquivo
        if (livro.codigo == codigo) {
            printf("\nLivro encontrado:\n");
            mostra_livro(&livro);
            livro_encontrado = 1;
            break;  // Saia do loop após encontrar o livro
        }
    }

    if (livro_encontrado==0) {
        printf("\nLivro nao encontrado");
        fclose(arqui1);
        fclose(arqui2);
        return;
    }

    printf("Digite quantos livros deseja devolver: ");
    scanf("%d", &devolvidos);

    livro.emprest_dispo += devolvidos; //Soma a quantidade de livros devolvidos aos livros disponível para empréstimo (estoque)

    int nusp;
    printf("Digite o numero USP do usuario: ");
    scanf("%d", &nusp);

    int usuario_encontrado = 0;
    int livro_emprestado_encontrado = 0;

    while (fread(&usuario, sizeof(t_usuario), 1, arqui2) == 1) { //Permanece no loop enquanto estiver lendo dados
        if (usuario.nusp == nusp) { //Procura usuario
            usuario_encontrado = 1;
            for (int i = 0; i < 3; i++) { //Procura os emprestados do usuario com o codigo informado
                if (usuario.livro_emprestado[i].codigo == codigo) {
                    livro_emprestado_encontrado = 1;
                    if ((usuario.livro_emprestado[i].emprest_dispo - devolvidos)>=0){ //Verifica se a quantidade confere com o estoque
                        usuario.livro_emprestado[i].emprest_dispo -= devolvidos;
                        if (usuario.livro_emprestado[i].emprest_dispo==0){ //Subtrai 1 da quantidade de livros emprestados se a quantidade de cópias emprestadas for 0
                            usuario.qntlivro_emprestados--;
                        }
                        printf ("\nLivro Devolvido!\n\n");
                        break;
                    }
                    else {
                        printf ("Erro! Quantidade a ser devolvida maior do que a emprestada.");
                        return;
                    }
                }
            }
            break;  // Saia do loop após encontrar o usuário
        }
    }

    if (!usuario_encontrado) {
        printf("\nUsuario nao encontrado");
        fclose(arqui1);
        fclose(arqui2);
        return;
    }

    if (!livro_emprestado_encontrado) {
        printf("\nLivro nao emprestado");
        fclose(arqui1);
        fclose(arqui2);
        return;
    }

    fseek(arqui2, -(long)sizeof(t_usuario), SEEK_CUR); //posiciona o cursor do arquivo no usuario
    fwrite(&usuario, sizeof(t_usuario), 1, arqui2); //Atualiza as informações do usuário
    mostra_usuario(&usuario);

    fseek(arqui1, -(long)sizeof(t_livro), SEEK_CUR); //posiciona o cursor do arquivo no livro desejado
    fwrite(&livro, sizeof(t_livro), 1, arqui1); //atualiza as informações do livro (altera empréstimo)

    fclose(arqui1);
    fclose(arqui2);
}


void emprestimo(FILE* arqui1, FILE* arqui2){

    int codigo2; //código do livro
    int emprestados; //quantidade de exemplares

    printf("\nDigite o codigo do livro que deseja fazer emprestimo: ");
    scanf("%d", &codigo2);

    arqui1 = fopen("Estoque.bin", "r+b");
    if (arqui1 == NULL) {
        printf("\nErro na abertura do arquivo de estoque\n");
        return;
    }

    arqui2 = fopen("Dados_usuario.bin", "r+b");
    if (arqui2 == NULL) {
        printf("\nErro na abertura do arquivo de dados do usuario\n");
        return;
    }

    t_livro livro;
    t_usuario usuario;

    int livro_encontrado = 0;

    while (fread(&livro, sizeof(t_livro), 1, arqui1) == 1) {
        if (livro.codigo == codigo2) {
            printf("\nLivro encontrado:\n");
            mostra_livro(&livro);
            livro_encontrado = 1;
            break;
        }
    }

    if (!livro_encontrado) {
        printf("\nLivro nao encontrado");
        fclose(arqui1);
        fclose(arqui2);
        return;
    }

    printf("Digite quantos livros deseja fazer emprestimo: ");
    scanf("%d", &emprestados);

    if (livro.emprest_dispo < emprestados) {
        printf("\nQuantidade indisponivel");
        fclose(arqui1);
        fclose(arqui2);
        return;
    }

    int nusp;
    int senha;

    printf("Digite o numero USP do usuario: ");
    scanf("%d", &nusp);

    printf("Digite a senha: ");
    scanf("%d", &senha);

    int usuario_encontrado = 0;
    int verificador_emprestimo = 0; //Auxilia na verificação do estoque do usuário

    while (fread(&usuario, sizeof(t_usuario), 1, arqui2) == 1) {
        if (usuario.nusp == nusp) {
            usuario_encontrado = 1;
            if (usuario.senha == senha) { //Verifica a senha
                if (usuario.qntlivro_emprestados >= 3) { //Verifica se o limite de livros foi atigindo
                    printf("\nLimite de emprestimos atingido para este usuario");
                    fclose(arqui1);
                    fclose(arqui2);
                    return;
                }
                for (int i =0; i<3; i++){
                    if (usuario.livro_emprestado[i].codigo==codigo2){ //Verifca se o livro a ser emprestado já tem no estoque do usuário
                        usuario.livro_emprestado[i].emprest_dispo += emprestados; //Se sim, soma a quantidade pedida ao número de exemplares do usuário
                        printf("\nEmprestimo realizado com sucesso!\n\n");
                        verificador_emprestimo = 1;
                        break;
                    }
                }
                if (verificador_emprestimo==0){ //se o livro a ser emprestado não tiver no estoque do usuário
                    for (int i=0; i<3; i++){
                        if (usuario.livro_emprestado[i].emprest_dispo==0){ //Dados do livro é adicionado no espaço vazio do estoque do usuário
                            usuario.livro_emprestado[i] = livro;
                            usuario.livro_emprestado[i].emprest_dispo = emprestados;
                            usuario.qntlivro_emprestados++;
                            printf("\nEmprestimo realizado com sucesso!\n\n");
                            break;
                        }
                    }
                }
                break;  // Saia do loop após encontrar o usuário
            }
            else {
                printf("\nSenha invalida");
                fclose(arqui1);
                fclose(arqui2);
                return;
            }
        }
    }

    if (usuario_encontrado==0) {
        printf("\nUsuario nao encontrado");
        fclose(arqui1);
        fclose(arqui2);
        return;
    }

    //Atualiza dados do usuário
    fseek(arqui2, -(long)sizeof(t_usuario), SEEK_CUR);
    fwrite(&usuario, sizeof(t_usuario), 1, arqui2);
    mostra_usuario(&usuario);

    //Atualiza dados do livro
    livro.emprest_dispo = livro.emprest_dispo - emprestados;
    fseek(arqui1, -(long)sizeof(t_livro), SEEK_CUR);
    fwrite(&livro, sizeof(t_livro), 1, arqui1);

    fclose(arqui1);
    fclose(arqui2);
}

void pesquisa_livro(FILE *arquivo){

    int opcao2;

    printf("\nPor qual mecanismo deseja pesquisar?\n");
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
      default:
            printf("\nErro! Escolha 1, 2 ou 3!");
            return;
            break;
    }
}

void pesquisa_nome(FILE *arqui){

    char p_nome[MAX]; //Variável que recebe o nome do livro

    printf("\nDigite o nome do livro: ");
    fflush(stdin);
    fgets(p_nome, MAX, stdin);

    arqui = fopen("Estoque.bin", "rb");

    if (arqui == NULL) {
        printf("Erro ao abrir o arquivo.\n\n");
        return;
    }

    t_livro livro;
    int livro_encontrado =0;

    while (fread(&livro, sizeof(t_livro), 1, arqui) == 1) {
        if (strncmp(livro.nome_livro, p_nome, strlen(p_nome)) == 0) { //Compara o nome declarado com o nome armazenado
            printf("\nLivro encontrado:\n");
            mostra_livro(&livro);
            livro_encontrado =1;
        }
    }

    if (livro_encontrado==0) printf("\nLivro nao encontrado");

    fclose(arqui);
}

void pesquisa_autor(FILE*arqui){

    char p_nome[MAX];

    printf("Digite o nome do autor: ");
    fflush(stdin);
    fgets(p_nome, MAX, stdin);

    arqui = fopen("Estoque.bin", "rb");
    if (arqui == NULL) {
        printf("Erro ao abrir o arquivo.\n\n");
        return;
    }

    t_livro livro;
    int livro_encontrado =0;

    while (fread(&livro, sizeof(t_livro), 1, arqui) == 1) {
        if (strncmp(livro.autor, p_nome, strlen(p_nome)) == 0) { //Compara o nome declarado com o nome armazenado
            printf("\nLivro encontrado:\n");
            mostra_livro(&livro);
            livro_encontrado =1;
        }
    }

    if (livro_encontrado==0) printf("\nLivro nao encontrado");

    fclose(arqui);
}

void pesquisa_codigo(FILE *arqui){

    int p_codigo;

    printf("Digite o codigo do livro: ");
    scanf("%d",&p_codigo);

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
            mostra_livro (&livro);
            livro_encontrado =1;
        }
    }

    if (livro_encontrado==0) printf("\nLivro nao encontrado");

    fclose(arqui);
}


void cadastro_usuario (t_usuario *usuario, FILE *arqui2){

    arqui2 = fopen("dados_usuario.bin", "ab");
    if(arqui2 == NULL){
      printf("\n>Erro na abertura dos dados de usuários<\n");
      return;
    }

    printf("\nInsira o nome do usuario: ");
    fflush(stdin);
    fgets(usuario->nome, MAX, stdin);

    int auxiliar;
    for (auxiliar=0; auxiliar<1;){
        printf("Digite o numero USP: ");
        scanf("%d", &usuario->nusp);
        auxiliar = verifica_nusp(&usuario->nusp, arqui2);
        if (auxiliar==2) return;
    }

    printf("Crie uma senha numerica: ");
    scanf("%d", &(usuario->senha));

    usuario->qntlivro_emprestados = 0; //zera o numero de livros emprestados ao usuário

    for (int i = 0; i<3; i++){
        (usuario->livro_emprestado[i].emprest_dispo)=0; //zera o número de exemplares dos espaços reservados aos livros
    }

    fwrite(usuario, sizeof(t_usuario), 1, arqui2); //esceve os dados do usuário no arquivo

    fclose(arqui2);
}

int verifica_nusp(int *nusp, FILE* arqui)
{
    arqui=fopen("dados_usuario.bin", "rb");
    if (arqui==NULL) {
        printf ("\nErro ao abrir o arquivo\n");
        fclose (arqui);
        return 2;
    }

    t_usuario usuario;
    while (fread(&usuario, sizeof(t_usuario), 1, arqui) == 1) {
        if (usuario.nusp==*nusp) {
            printf("\nNumero USP ja cadastrado. Tente outro!\n");
            fclose (arqui);
            return 0;
        }
    }
    fclose (arqui);
    return 1;
}

void cadastro_livro(t_livro *livro, FILE*arquivo)
{

    arquivo=fopen("Estoque.bin", "ab");
    if (arquivo==NULL) {
        printf ("Erro ao abrir o arquivo");
        return;
    }

    printf("\nInsira o nome do livro: ");
    fflush(stdin);
    fgets (livro->nome_livro, MAX, stdin);

    printf ("Digite o nome do autor: ");
    fflush(stdin);
    fgets (livro->autor, MAX, stdin);

    printf ("Digite o ano de publicacao: ");
    scanf ("%d", &livro->ano_publicacao);

    int auxiliar;
    for (auxiliar=0; auxiliar<1;){
        printf ("Digite o codigo: ");
        scanf ("%d", &livro->codigo);
        auxiliar = verifica_codigo(&livro->codigo, arquivo);
        if (auxiliar==2) return;
    }

    printf ("Digite o volume: ");
    scanf ("%d", &livro->volume);

    printf ("Digite a edicao: ");
    scanf ("%d", &livro->edicao);

    printf ("Digite o nome da editora: ");
    fflush(stdin);
    fgets (livro->editora, MAX, stdin);

    printf ("Digite a quantidade total de livros: ");
    scanf ("%d", &livro->total);
    livro->emprest_dispo = livro->total;

    fwrite(livro, sizeof(t_livro), 1, arquivo); //escreve os dados do livro no arquivo

    fclose (arquivo);
}

int verifica_codigo (int *codigo, FILE*arqui)
{
    arqui=fopen("Estoque.bin", "rb");
    if (arqui==NULL) {
        printf ("\nErro ao abrir o arquivo\n");
        fclose (arqui);
        return 2;
    }

    t_livro livro;
    while (fread(&livro, sizeof(t_livro), 1, arqui) == 1) {
        if (livro.codigo==*codigo) {
            printf("\nCodigo ja cadastrado. Tente outro!\n");
            fclose (arqui);
            return 0;
        }
    }
    fclose (arqui);
    return 1;
}

void mostra_livro (t_livro *livro) //função que imprime as informações do livro
{
    printf("\nNome: %s", livro->nome_livro);
    printf("Autor: %s", livro->autor);
    printf("Ano de Publicacao: %d\n", livro->ano_publicacao);
    printf("Codigo: %d\n", livro->codigo);
    printf("Volume: %d\n", livro->volume);
    printf("Edicao: %d\n", livro->edicao);
    printf("Editora: %s", livro->editora);
    printf("Quantidade Total em estoque: %d\n", livro->total);
    printf("Disponivel para emprestimo: %d\n", livro->emprest_dispo);
    printf("\n");
}

void mostra_usuario(t_usuario *usuarios) //função que imprime as imformações do usuário
{
    printf ("Dados do usuario:\n");
    printf("Nome: %s", usuarios->nome);
    printf("Numero USP: %d\n", usuarios->nusp);
    printf ("\nLivros emprestados:\n");

    for (int i = 0; i < 3; i++) {
        if ((usuarios->livro_emprestado[i].emprest_dispo)>0){
            printf("\nNome: %s", usuarios->livro_emprestado[i].nome_livro);
            printf("Autor: %s", usuarios->livro_emprestado[i].autor);
            printf("Ano de Publicacao: %d\n", usuarios->livro_emprestado[i].ano_publicacao);
            printf("Codigo: %d\n", usuarios->livro_emprestado[i].codigo);
            printf("Volume: %d\n", usuarios->livro_emprestado[i].volume);
            printf("Edicao: %d\n", usuarios->livro_emprestado[i].edicao);
            printf("Editora: %s", usuarios->livro_emprestado[i].editora);
            printf("Quantidade de exemplares: %d\n", usuarios->livro_emprestado[i].emprest_dispo);
            printf("\n");
        }
    }
}
