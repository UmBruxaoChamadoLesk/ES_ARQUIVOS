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

int comparaIndice(const void *a, const void *b) 
{
    return strncmp(((Endereco *)a)->cep, ((Endereco *)b)->cep, 8);
}


void intercala(Endereco *k, int qtdReg)
{
    Endereco *aux = (Endereco *)malloc(sizeof(Endereco) * qtdReg);
    int i = 0, j = qtdReg / 2, kIndex = 0;

    while (i < qtdReg / 2 && j < qtdReg) {
        if (comparaIndice(&k[i], &k[j]) <= 0) {
            aux[kIndex++] = k[i++];
        } else {
            aux[kIndex++] = k[j++];
        }
    }

    while (i < qtdReg / 2) {
        aux[kIndex++] = k[i++];
    }

    while (j < qtdReg) {
        aux[kIndex++] = k[j++];
    }

    memcpy(k, aux, sizeof(Endereco) * qtdReg);
    free(aux);
}

int main(int argc, char **argv)
{
    FILE *f = fopen(argv[1], "rb");

    fseek(f, 0, SEEK_END);
    int tamArq = ftell(f);
    int tamReg = sizeof(Endereco);
    int qtdReg = tamArq / tamReg;
    Endereco *k = (Endereco *)malloc(tamReg * qtdReg);

    fseek(f, 0, SEEK_SET);
    fread(k, tamReg, qtdReg, f);
    qsort(k, qtdReg, sizeof(Endereco), comparaIndice(0, qtdReg));
    intercala(k, qtdReg);
    FILE* novo = fopen("arquivoOrdenado.bin", "wb");
    fwrite(k, sizeof(Endereco), qtdReg, novo);


    return 0;
}
