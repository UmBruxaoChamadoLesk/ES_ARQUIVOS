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

typedef struct _Indice
{
    char cep[8];
    int posicao;
} Indice;

int comparaIndice(const void *a, const void *b) 
{
    return strncmp(((Indice *)a)->cep, ((Indice *)b)->cep, 8);
}

static void swap_bytes(char *a, char *b, size_t size)
{
    while (size--) {
        char tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    }
}

static void qsort_recursive(char *base, long lo, long hi, size_t size,
    int (*compar)(const void *, const void *))
{
    if (lo >= hi) {
        return;
    }

    char *pivot = base + ((lo + hi) / 2) * size;
    long i = lo;
    long j = hi;

    while (i <= j) {
        while (compar(base + i * size, pivot) < 0) {
            i++;
        }
        while (compar(base + j * size, pivot) > 0) {
            j--;
        }
        if (i <= j) {
            swap_bytes(base + i * size, base + j * size, size);
            i++;
            j--;
        }
    }

    if (lo < j) {
        qsort_recursive(base, lo, j, size, compar);
    }
    if (i < hi) {
        qsort_recursive(base, i, hi, size, compar);
    }
}

void qsort(void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *))
{
    if (nmemb <= 1 || size == 0) {
        return;
    }
    qsort_recursive((char *)base, 0, (long)nmemb - 1, size, compar);
}

int main(int argc, char** argv)
{
    FILE* f = fopen("argv[1]","rb");

    fseek(f,0,"SEEK_END");
    int tamArq = ftell(f);
    int tamReg = sizeof(Endereco);
    int qtdReg = tamArq/tamReg;

    Indice *idx = (indice*)malloc(tamReg*qtdReg);
    Endereco e;

    fseek(f,0,"SEEK_SET")
    int qt=0;
    for(int i = 0; i<qtdReg; i++)
    {
        idx[i].posicao = i;
        fread(&e,tamReg,1,f);
        strncpy(idx[i].cep,e.cep,8);//idx[i]->cep = e.cep;
        qt++;
    }

    qsort(idx, qtdReg, sizeof(Indice), comparaIndice);
    
    FILE* novo = fopen("cep_index.bin","wb");

    fwrite(idx,sizeof(Indice),qtdReg,novo);

    fclose(novo);

    char cepBusca[9];
    printf("Digite o CEP para busca (8 digitos): ");
    scanf("%8s", cepBusca);

    Indice chave;
    strncpy(chave.cep, cepBusca, 8);

    Indice *res = (Indice *)buscaBinaria(&chave, idx, qtdRegistros, sizeof(Indice), comparaIndice);

    if (res) {
        printf("CEP encontrado na posicao logica %ld\n", res->posicao);
        
        // 6. Recuperar dados do arquivo original usando o offset
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