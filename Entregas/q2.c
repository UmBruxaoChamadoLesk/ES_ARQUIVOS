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

    qsort(idx, qtdReg, sizeof(Indice), comparaIndice(0,qt));
    
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