#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

typedef struct _Indice {
    char cep[9];  // +1 para o terminador nulo
    int posicao;
} Indice;

int comparaIndice(const void *a, const void *b) {
    return strncmp(((Indice *)a)->cep, ((Indice *)b)->cep, 8);
}

// Implementação da busca binária
Indice* buscaBinaria(Indice *chave, Indice *base, int n, size_t size, 
                     int (*compar)(const void *, const void *)) {
    int esq = 0, dir = n - 1;
    
    while (esq <= dir) {
        int meio = (esq + dir) / 2;
        int cmp = compar(chave, &base[meio]);
        
        if (cmp == 0)
            return &base[meio];
        else if (cmp < 0)
            dir = meio - 1;
        else
            esq = meio + 1;
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_cep.bin>\n", argv[0]);
        return 1;
    }
    
    FILE* f = fopen(argv[1], "rb");  
    if (!f) {
        printf("Erro ao abrir arquivo!\n");
        return 1;
    }
    
    fseek(f, 0, SEEK_END);           
    long tamArq = ftell(f);
    int tamReg = sizeof(Endereco);
    int qtdReg = tamArq / tamReg;
    
    
    Indice *idx = (Indice*)malloc(sizeof(Indice) * qtdReg);
    if (!idx) {
        printf("Erro de alocação!\n");
        fclose(f);
        return 1;
    }
    
    Endereco e;
    fseek(f, 0, SEEK_SET);
    
    for(int i = 0; i < qtdReg; i++) {
        idx[i].posicao = i;
        fread(&e, tamReg, 1, f);
        memcpy(idx[i].cep, e.cep, 8);
        idx[i].cep[8] = '\0';
    }
    
    // Usar qsort padrão da libc
    qsort(idx, qtdReg, sizeof(Indice), comparaIndice);
    
    FILE* novo = fopen("cep_index.bin", "wb");
    if (novo) {
        fwrite(idx, sizeof(Indice), qtdReg, novo);
        fclose(novo);
        printf("Índice criado com sucesso!\n");
    }
    
    char cepBusca[9];
    printf("Digite o CEP para busca (8 digitos): ");
    scanf("%8s", cepBusca);
    
    Indice chave;
    memcpy(chave.cep, cepBusca, 8);
    chave.cep[8] = '\0';
    
    
    Indice *res = buscaBinaria(&chave, idx, qtdReg, sizeof(Indice), comparaIndice);
    
    if (res) {
        printf("CEP encontrado na posicao logica %d\n", res->posicao);
        
        fseek(f, res->posicao * sizeof(Endereco), SEEK_SET);
        fread(&e, sizeof(Endereco), 1, f);
        
        printf("\n--- Dados Encontrados ---\n");
        printf("Logradouro: %.72s\n", e.logradouro);
        printf("Bairro:     %.72s\n", e.bairro);
        printf("Cidade:     %.72s\n", e.cidade);
        printf("UF:         %.72s\n", e.uf);
        printf("CEP:        %.8s\n", e.cep);
    } else {
        printf("CEP nao encontrado.\n");
    }
    
    free(idx);
    fclose(f);
    return 0;
}