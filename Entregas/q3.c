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

static void swap(void *a, void *b, size_t width)
{
    unsigned char *pa = a;
    unsigned char *pb = b;
    unsigned char tmp;

    while (width--) {
        tmp = *pa;
        *pa++ = *pb;
        *pb++ = tmp;
    }
}

static void quicksort_rec(void *base, size_t low, size_t high, size_t width, int (*comp)(const void *, const void *))
{
    if (low >= high) {
        return;
    }

    unsigned char *array = base;
    size_t i = low;
    size_t j = high;
    unsigned char *pivot = array + ((low + high) / 2) * width;

    while (i <= j) {
        while (comp(array + i * width, pivot) < 0) {
            i++;
        }

        while (comp(array + j * width, pivot) > 0) {
            if (j == 0) {
                break;
            }
            j--;
        }

        if (i <= j) {
            swap(array + i * width, array + j * width, width);
            i++;
            if (j == 0) {
                break;
            }
            j--;
        }
    }

    if (j > low) {
        quicksort_rec(base, low, j, width, comp);
    }

    if (i < high) {
        quicksort_rec(base, i, high, width, comp);
    }
}

void quicksort(void *base, size_t nel, size_t width, int (*comp)(const void *, const void *))
{
    if (!base || nel < 2 || width == 0 || comp == NULL) {
        return;
    }

    quicksort_rec(base, 0, nel - 1, width, comp);
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
    quicksort(k, qtdReg, sizeof(Endereco), comparaIndice);
    intercala(k, qtdReg);
    FILE* novo = fopen("arquivoOrdenado.bin", "wb");
    fwrite(k, sizeof(Endereco), qtdReg, novo);


    return 0;
}
