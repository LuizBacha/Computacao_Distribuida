#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa um produto no sistema.
// "nome" e um ponteiro para char, pois a string sera alocada dinamicamente.
// Isso permite armazenar nomes de tamanhos diferentes sem desperdicar memoria.
typedef struct {
    int codigo;
    char *nome;
    float preco;
    int quantidade;
} Produto;

//=========================================================================================
//=========================================================================================
//=========================================================================================

int menu() {
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

    printf("\nOpcao: ");
    scanf("%d", &opcao);
    return opcao;
}

//=========================================================================================
//=========================================================================================
//=========================================================================================

void adicionar_produto(Produto **lista, int *quantidade, int *capacidade, int *proximo_codigo) {
    printf("--- Adicionar Produto ---\n");

    int nova_capacidade = *capacidade;

    // Verifica se o vetor esta cheio.
    // Se quantidade == capacidade significa que nao ha espaco para novos produtos.
    if (*quantidade == *capacidade) {
        // Crescimento exponencial do vetor para evitar muitas realocacoes.
        // Se ainda nao existe vetor, iniciamos com capacidade 2.
        if (*capacidade == 0) {
            nova_capacidade = 2;
        } else {
            // Dobrar capacidade e mais eficiente que aumentar de 1 em 1.
            nova_capacidade = (*capacidade) * 2;
        }

        // realloc tenta redimensionar o bloco de memoria do vetor de produtos.
        // Pode retornar um novo endereco de memoria.
        Produto *temp = realloc(*lista, nova_capacidade * sizeof(Produto));

        // Se realloc falhar ele retorna NULL, entao precisamos verificar.
        if (temp == NULL) {
            printf("Erro de memoria\n");
            return;
        }

        *lista = temp;
        *capacidade = nova_capacidade;
    }

    // p aponta para a proxima posicao livre do vetor.
    // (*lista) e o vetor de produtos.
    // (*quantidade) indica quantos produtos ja existem.
    Produto *p = &((*lista)[*quantidade]);

    // Buffer temporario para ler o nome digitado pelo usuario.
    char buffer[100];

    printf("Nome: ");
    scanf(" %99[^\n]", buffer); // Permite ler nomes com espacos.

    // Alocamos memoria suficiente apenas para o tamanho real do nome.
    p->nome = malloc(strlen(buffer) + 1);

    // Se malloc falhar retornara NULL.
    if (p->nome == NULL) {
        printf("Erro de memoria para nome\n");
        return;
    }

    // Copiamos o conteúdo do buffer para a memoria recem-alocada.
    strcpy(p->nome, buffer);

    printf("Preco: ");
    scanf("%f", &p->preco);

    printf("Quantidade: ");
    scanf("%d", &p->quantidade);

    p->codigo = *proximo_codigo;
    printf("Produto adicionado com o codigo %d!\n", p->codigo);
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
    printf("| Codigo | Nome             | Preco    | Qtd  | Valor Estoque |\n");
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
    // Busca sequencial no vetor de produtos.
    // Retorna um ponteiro para o produto encontrado.
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

void remover_produto(Produto **lista, int *quantidade, int *capacidade) {
    printf("--- Remover Produto ---\n");

    int codigo;
    printf("Digite o codigo do produto a remover: ");
    scanf("%d", &codigo);

    int indice = -1;

    for (int i = 0; i < *quantidade; i++) {
        if ((*lista)[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }

    // Libera memoria do nome do produto removido
    free((*lista)[indice].nome);

    // Desloca elementos para preencher o buraco
    for (int i = indice; i < *quantidade - 1; i++) {
        (*lista)[i] = (*lista)[i + 1];
    }

    (*quantidade)--;

    // Realoca vetor para o novo tamanho (requisito do trabalho)
    if (*quantidade > 0) {
        Produto *temp = realloc(*lista, (*quantidade) * sizeof(Produto));
        if (temp == NULL) {
            printf("Erro ao realocar memoria.\n");
            return;
        }
        *lista = temp;
        *capacidade = *quantidade;
    } else {
        free(*lista);
        *lista = NULL;
        *capacidade = 0;
    }

    printf("Produto removido com sucesso.\n");
}

//=========================================================================================
//=========================================================================================
//=========================================================================================

void liberar_memoria(Produto *lista, int quantidade) {
    // Antes de liberar o vetor precisamos liberar todas as strings
    // que foram alocadas dinamicamente para os nomes dos produtos.
    for (int i = 0; i < quantidade; i++) {
        if (lista[i].nome != NULL) {
            printf("Memoria do produto %s liberada\n", lista[i].nome);
            free(lista[i].nome);
        }
    }
    // Apos liberar todos os nomes liberamos o vetor de produtos.
    free(lista);
    printf("Vetor de produtos liberado!\n");
}

//=========================================================================================
//=========================================================================================
//=========================================================================================

int main() {
    Produto *lista = NULL;
    int quantidade = 0;
    int capacidade = 0;
    int proximo_codigo = 1;

    printf("========================================\n"
           "   SISTEMA DE CADASTRO DE PRODUTOS\n"
           "========================================\n");

    while (true) {
        int opcao = menu();

        if (opcao == 6) {
            printf("Sistema encerrado!\n");
            break;
        }

        if (opcao == 1) {
            adicionar_produto(&lista, &quantidade, &capacidade, &proximo_codigo);
        }

        if (opcao == 2) {
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
            remover_produto(&lista, &quantidade, &capacidade);
        }
    }

    liberar_memoria(lista, quantidade);
    return 0;
}

/*
Este programa utiliza ponteiros para manipular memoria dinamica.

Produto*  -> aponta para um produto ou para o primeiro elemento do vetor.
Produto** -> ponteiro para o vetor de produtos, usado quando precisamos
             alterar o endereco do vetor (ex: realloc).

malloc  -> aloca memoria dinamicamente.
realloc -> redimensiona um bloco de memoria existente.
free    -> libera memoria previamente alocada.

E fundamental liberar toda memoria alocada para evitar memory leaks.
*/