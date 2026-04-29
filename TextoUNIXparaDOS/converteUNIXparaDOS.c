#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char **argv)
{
    if(argc < 2) {
        printf("Uso: %s <arquivo_origem>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "rb");
    FILE *copia = fopen("copia", "wb");

    if(file == NULL) {
        printf("arquivo nao existe\n");
        return 1;
    }
    
    if(copia == NULL) {
        printf("erro ao criar arquivo copia\n");
        fclose(file);
        return 1;
    }

    // Convertendo \n para \r\n durante a cópia
    int ch;
    while((ch = fgetc(file)) != EOF) {
        if(ch == '\n') {
            fputc('\r', copia);  // Primeiro CR
            fputc('\n', copia);  // Depois LF
        } else {
            fputc(ch, copia);
        }
    }

    // Fechar e reabrir para leitura (ou usar fflush + rewind)
    fclose(copia);
    copia = fopen("copia", "rb");
    
    if(copia == NULL) {
        printf("erro ao reabrir arquivo copia\n");
        fclose(file);
        return 1;
    }
    
    // Descobrir tamanho do arquivo
    fseek(copia, 0, SEEK_END);
    long tamanhoArq = ftell(copia);
    fseek(copia, 0, SEEK_SET);
    
    // Alocar memória para o buffer (+1 para o \0)
    char *buffer = (char *)malloc(tamanhoArq + 1);
    if(buffer == NULL) {
        printf("erro de alocacao de memoria\n");
        fclose(file);
        fclose(copia);
        return 1;
    }
    
    // Ler o arquivo inteiro
    fread(buffer, 1, tamanhoArq, copia);
    buffer[tamanhoArq] = '\0';  // Adicionar terminador
    
    printf("\nConteúdo do arquivo copia:\n%s", buffer);
    
    // Liberar memória
    free(buffer);
    fclose(file);
    fclose(copia);
    
    return 0;
}