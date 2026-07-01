#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ArvoreB.h"

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2]; // Ao Espaço no final da linha + quebra de linha
};

static void preparaChaveCep(const char* origem, char chave[TAM_CHAVE + 1])
{
    int i;
    memset(chave, '\0', TAM_CHAVE + 1);
    for (i = 0; i < TAM_CHAVE && origem[i] != '\0'; i++)
    {
        chave[i] = origem[i];
    }
    while (i < TAM_CHAVE)
    {
        chave[i++] = '0';
    }
}

int main(int argc, char** argv)
{
    FILE* entrada;
    Endereco endereco;
    ArvoreB* arvore;
    char chave[TAM_CHAVE + 1];
    long posicaoRegistro;
    int registrosIndexados = 0;

    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s <arquivo_de_enderecos> <arquivo_de_indice>\n", argv[0]);
        return 1;
    }

    entrada = fopen(argv[1], "rb");
    if (!entrada)
    {
        fprintf(stderr, "Nao foi possivel abrir o arquivo de entrada: %s\n", argv[1]);
        return 1;
    }

    remove(argv[2]);
    arvore = ArvoreB_Abre(argv[2]);
    if (!arvore)
    {
        fprintf(stderr, "Nao foi possivel criar o arquivo de indice: %s\n", argv[2]);
        fclose(entrada);
        return 1;
    }

    while (!feof(entrada))
    {
        posicaoRegistro = ftell(entrada);
        if (fread(&endereco, sizeof(Endereco), 1, entrada) != 1)
        {
            break;
        }

        preparaChaveCep(endereco.cep, chave);
        ArvoreB_Insere(arvore, chave, posicaoRegistro);
        registrosIndexados++;
    }

    ArvoreB_Fecha(arvore);
    fclose(entrada);

    printf("%d registros indexados em %s\n", registrosIndexados, argv[2]);
    return 0;
}
