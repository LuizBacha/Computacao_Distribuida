#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codigo;
    char *nome;
    float preco;
    int quantidade;
} Produto;

//=========================================================================================
//=========================================================================================
//=========================================================================================

int menu(){
    int opcao;
    printf(
        "\nMenu:\n"
        "1. Adicionar produto\n"
        "2. Listar produtos\n"
        "3. Buscar produto\n"
        "4. Atualizar estoque\n"
        "5. Remover produto\n"
        "6. Sair\n"
    );

    printf("\nOpção: ");
    scanf("%d", &opcao);  
    return opcao;  
}

//=========================================================================================
//=========================================================================================
//=========================================================================================

void adicionar_produto(Produto **lista, int *quantidade, int *capacidade, int *proximo_codigo){
    printf("--- Adicionar Produto ---\n");
    int nova_capacidade = *capacidade;

    if(*quantidade == *capacidade){
        if(*capacidade == 0){
            nova_capacidade = 2;
        }
        else{
            nova_capacidade = (*capacidade) * 2;
        }

        Produto *temp =  realloc(*lista, nova_capacidade * sizeof(Produto));
        if(temp == NULL){
            printf("Erro de memoria\n");
            return;
        }

        *lista = temp;
        *capacidade = nova_capacidade;
    }
        
    Produto *p = &((*lista)[*quantidade]);

    char buffer[100];

    printf("Nome: ");
    scanf(" %99[^\n]", buffer);

    p->nome = malloc(strlen(buffer) + 1);
    if (p->nome == NULL) {
        printf("Erro de memoria para nome\n");
        return;
    }

    strcpy(p->nome, buffer);

    printf("Preco: ");
    scanf("%f", &p->preco);

    printf("Quantidade: ");
    scanf("%d", &p->quantidade);

    p->codigo = *proximo_codigo;
    printf("Produto adiciona com o código %d!\n",p->codigo);
    (*proximo_codigo)++;

    (*quantidade)++;
}

//=========================================================================================
//=========================================================================================
//=========================================================================================

void listar_produtos(Produto *lista, int quantidade) {

    if (quantidade == 0) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }

    float valor_total = 0.0;

    printf("\n--- Lista de Produtos ---\n");
    printf("+--------+------------------+----------+------+---------------+\n");
    printf("| Código | Nome             | Preço    | Qtd  | Valor Estoque |\n");
    printf("+--------+------------------+----------+------+---------------+\n");

    for (int i = 0; i < quantidade; i++) {

        Produto *p = &lista[i];

        float valor_estoque = p->preco * p->quantidade;
        valor_total += valor_estoque;

        const char *nome_exibicao = (p->nome != NULL) ? p->nome : "Sem nome";

        printf("| %6d | %-16s | %8.2f | %4d | %13.2f |\n",
               p->codigo,
               nome_exibicao,
               p->preco,
               p->quantidade,
               valor_estoque);
    }

    printf("+--------+------------------+----------+------+---------------+\n");
    printf("Valor total do estoque: R$ %.2f\n", valor_total);
}

//=========================================================================================
//=========================================================================================
//=========================================================================================
Produto* buscar_produto(Produto *lista, int quantidade, int codigo) {

    for (int i = 0; i < quantidade; i++) {
        if (lista[i].codigo == codigo) {
            return &lista[i];
        }
    }

    return NULL;
}
//=========================================================================================
//=========================================================================================
//=========================================================================================
void atualizar_estoque(Produto *lista, int quantidade) {
    printf("--- Atualizar Estoque ---\n");
    int codigo;
    printf("Digite o codigo do produto: ");
    scanf("%d", &codigo);

    Produto *p = buscar_produto(lista, quantidade, codigo);

    if (p == NULL) {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("Quantidade atual: %d\n", p->quantidade);

    printf("Nova quantidade: ");
    scanf("%d", &p->quantidade);

    printf("Estoque atualizado com sucesso.\n");
}
//=========================================================================================
//=========================================================================================
//=========================================================================================
void remover_produto(Produto *lista, int *quantidade) {
    printf("--- Remover Produto ---\n");
    int codigo;
    printf("Digite o codigo do produto a remover: ");
    scanf("%d", &codigo);

    int indice = -1;

    for (int i = 0; i < *quantidade; i++) {
        if (lista[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }

    // Libera memoria do nome
    free(lista[indice].nome);

    // Desloca elementos
    for (int i = indice; i < *quantidade - 1; i++) {
        lista[i] = lista[i + 1];
    }

    (*quantidade)--;

    printf("Produto removido com sucesso.\n");
}
//=========================================================================================
//=========================================================================================
//=========================================================================================
void liberar_memoria(Produto *lista, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("Memoria do produto %s liberada\n",lista[i].nome);
        free(lista[i].nome);
    }
    printf("Vetor de produtos liberado!\n");
    free(lista);
}
//=========================================================================================
//=========================================================================================
//=========================================================================================
int main(){
    
    Produto *lista = NULL;
    int quantidade = 0;
    int capacidade = 0;
    int proximo_codigo = 1;

    printf("========================================\n"
            "   SISTEMA DE CADASTRO DE PRODUTOS\n"
            "========================================\n");

    while (true) {

        int opcao = menu();

        if(opcao == 6){
            printf("Sistema encerrado!\n");
            break;
        }

        if(opcao == 1){
            adicionar_produto(&lista, &quantidade, &capacidade, &proximo_codigo);
        }
        
        if(opcao == 2){
            listar_produtos(lista, quantidade);
        }
        if (opcao == 3) {
            printf("--- Buscar Produto ---\n");
            int codigo;
            printf("Digite o codigo: ");
            scanf("%d", &codigo);

            Produto *p = buscar_produto(lista, quantidade, codigo);

            if (p == NULL) {
                printf("Produto nao encontrado.\n");
            } else {
                printf("Produto encontrado:\n");
                printf("Codigo: %d\n", p->codigo);
                printf("Nome: %s\n", p->nome);
                printf("Preco: %.2f\n", p->preco);
                printf("Quantidade: %d\n", p->quantidade);
            }
        }

        if (opcao == 4) {
            atualizar_estoque(lista, quantidade);
        }

        if (opcao == 5) {
            remover_produto(lista, &quantidade);
        }
    }
    liberar_memoria(lista, quantidade);
    return 0;
}