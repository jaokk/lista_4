#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FABRICANTES 5
#define MAX_PRODUTOS 50
#define UF_SIZE 3

typedef struct {
    char marca[50];
    char site[100];
    char telefone[20];
    char uf[UF_SIZE];
} Fabricante;

typedef struct {
    char descricao[100];
    float peso;
    float valorCompra;
    float valorVenda;
    float valorLucro;
    float percentualLucro;
    Fabricante* fabricante;
} Produto;

int le_valida_peso(float* peso) {
    printf("Peso (em gramas): ");
    scanf("%f", peso);
    if (*peso < 50 || *peso > 50000) {
        printf("Peso inválido! O peso deve estar entre 50g e 50kg.\n");
        return 0;
    }
    return 1;
}

int le_valida_valor(float* valor, const char* campo, float min, float max) {
    printf("%s: ", campo);
    scanf("%f", valor);
    if (*valor < min || *valor > max) {
        printf("%s inválido! O valor deve estar entre R$ %.2f e R$ %.2f.\n", campo, min, max);
        return 0;
    }
    return 1;
}

int le_valida_uf(char* uf) {
    printf("UF: ");
    scanf("%s", uf);
    // Lista das Unidades da Federação válidas
    const char* ufs_validas[] = {"AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO",
                                 "MA", "MG", "MS", "MT", "PA", "PB", "PE", "PI", "PR",
                                 "RJ", "RN", "RO", "RR", "RS", "SC", "SE", "SP", "TO"};
    int num_ufs_validas = sizeof(ufs_validas) / sizeof(ufs_validas[0]);
    for (int i = 0; i < num_ufs_validas; i++) {
        if (strcmp(uf, ufs_validas[i]) == 0)
            return 1;
    }
    printf("UF inválida! Insira uma UF válida, por exemplo, SP para São Paulo.\n");
    return 0;
}

int comparar_produtos(const void* a, const void* b) {
    Produto* produtoA = *(Produto**)a;
    Produto* produtoB = *(Produto**)b;
    return strcmp(produtoA->descricao, produtoB->descricao);
}

int comparar_marcas(const void* a, const void* b) {
    Produto* produtoA = *(Produto**)a;
    Produto* produtoB = *(Produto**)b;
    return strcmp(produtoB->fabricante->marca, produtoA->fabricante->marca);
}

void listar_produtos(Produto* produtos[], int numProdutos) {
    qsort(produtos, numProdutos, sizeof(Produto*), comparar_produtos);
    printf("--- Lista de Produtos (A-Z) ---\n");
    for (int i = 0; i < numProdutos; i++) {
        Produto* produto = produtos[i];
        printf("Produto %d:\n", i + 1);
        printf("Descrição: %s\n", produto->descricao);
        printf("Peso: %.2fg\n", produto->peso);
        printf("Valor de compra: R$ %.2f\n", produto->valorCompra);
        printf("Valor de venda: R$ %.2f\n", produto->valorVenda);
        printf("Valor do lucro: R$ %.2f\n", produto->valorLucro);
        printf("Percentual do lucro: %.2f%%\n", produto->percentualLucro);
        printf("Fabricante: %s\n", produto->fabricante->marca);
        printf("\n");
    }
}

void listar_marcas(Produto* produtos[], int numProdutos) {
    qsort(produtos, numProdutos, sizeof(Produto*), comparar_marcas);
    printf("--- Lista de Marcas (Z-A) ---\n");
    for (int i = 0; i < numProdutos; i++) {
        Produto* produto = produtos[i];
        printf("Marca: %s\n", produto->fabricante->marca);
    }
}

int main() {
    Fabricante* fabricantes[MAX_FABRICANTES];
    Produto* produtos[MAX_PRODUTOS];
    int numFabricantes = 0;
    int numProdutos = 0;
    int i;

    // Cadastrar fabricantes
    printf("Cadastro de fabricantes:\n");
    for (i = 0; i < MAX_FABRICANTES; i++) {
        Fabricante* fabricante = (Fabricante*)malloc(sizeof(Fabricante));
        printf("Fabricante %d:\n", i + 1);
        printf("Marca: ");
        scanf("%s", fabricante->marca);
        printf("Site: ");
        scanf("%s", fabricante->site);
        printf("Telefone: ");
        scanf("%s", fabricante->telefone);
        while (!le_valida_uf(fabricante->uf))
            ;
        fabricantes[i] = fabricante;
        numFabricantes++;
        if (i == MAX_FABRICANTES - 1)
            break;
        printf("Deseja cadastrar outro fabricante? (S/N): ");
        char choice;
        scanf(" %c", &choice);
        if (choice != 'S' && choice != 's')
            break;
    }

    // Cadastrar produtos
    printf("\nCadastro de produtos:\n");
    for (i = 0; i < MAX_PRODUTOS; i++) {
        Produto* produto = (Produto*)malloc(sizeof(Produto));
        printf("Produto %d:\n", i + 1);
        printf("Descrição: ");
        scanf("%s", produto->descricao);
        while (!le_valida_peso(&produto->peso))
            ;
        while (!le_valida_valor(&produto->valorCompra, "Valor de compra", 0.5, 8000.0))
            ;
        while (!le_valida_valor(&produto->valorVenda, "Valor de venda", 1.0, 10000.0))
            ;

        // Cálculo do lucro e percentual de lucro
        produto->valorLucro = produto->valorVenda - produto->valorCompra;
        produto->percentualLucro = (produto->valorLucro / produto->valorCompra) * 100;

        // Selecionar fabricante
        printf("Fabricante (1-%d): ", numFabricantes);
        int fabricanteIndex;
        scanf("%d", &fabricanteIndex);
        if (fabricanteIndex < 1 || fabricanteIndex > numFabricantes) {
            printf("Fabricante inválido!\n");
            free(produto);
            i--;
            continue;
        }
        produto->fabricante = fabricantes[fabricanteIndex - 1];

        produtos[i] = produto;
        numProdutos++;
        if (i == MAX_PRODUTOS - 1)
            break;
        printf("Deseja cadastrar outro produto? (S/N): ");
        char choice;
        scanf(" %c", &choice);
        if (choice != 'S' && choice != 's')
            break;
    }

    // Exibir informações cadastradas
    printf("\n--- Informações Cadastradas ---\n");
    printf("Fabricantes:\n");
    for (i = 0; i < numFabricantes; i++) {
        printf("Fabricante %d:\n", i + 1);
        printf("Marca: %s\n", fabricantes[i]->marca);
        printf("Site: %s\n", fabricantes[i]->site);
        printf("Telefone: %s\n", fabricantes[i]->telefone);
        printf("UF: %s\n", fabricantes[i]->uf);
        printf("\n");
    }

    // Menu de opções
    char opcao;
    do {
        printf("Opções:\n");
        printf("[A] Listar todos os produtos em ordem alfabética crescente A-Z\n");
        printf("[Z] Listar todas as marcas em ordem alfabética decrescente Z-A\n");
        printf("[S] Sair\n");
        printf("Escolha uma opção: ");
        scanf(" %c", &opcao);

        switch (opcao) {
            case 'A':
            case 'a':
                listar_produtos(produtos, numProdutos);
                break;
            case 'Z':
            case 'z':
                listar_marcas(produtos, numProdutos);
                break;
            case 'S':
            case 's':
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
        printf("\n");
    } while (opcao != 'S' && opcao != 's');

    // Liberar memória alocada
    for (i = 0; i < numFabricantes; i++)
        free(fabricantes[i]);
    for (i = 0; i < numProdutos; i++)
        free(produtos[i]);

    return 0;
}
