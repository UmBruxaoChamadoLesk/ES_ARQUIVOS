#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct registroTemporalCovid19
{
    int dia;
    int novos_casos;
    int obitos;
} typedef tempCovid;

int main(int argc, char ** argv)
{
    FILE *file = fopen(argv[1],"ab");

        if(file == NULL)
        {
            file = fopen(argv[1],"wb");
            if(file == NULL)
            {
                printf("arquivo não foi criado\n");
                return 1;
            }
        }
        fseek(file,0,SEEK_END);
        int tamanhoArq = ftell(file);
        int tamanhoReg = sizeof(tempCovid);
        int qtdReg = tamanhoArq/tamanhoReg;
        rewind(file);

        char resposta;
        printf("Deseja inserir novos dados? (s/n)\n");
        scanf("%c",&resposta);

        while(resposta =='s')
        {
            tempCovid reg;
            printf("Digite o dia\n");
            scanf("%d",&reg.dia);
            printf("Digite o numero de novos casos\n");
            scanf("%d",&reg.novos_casos);
            printf("Digite o numero de obitos\n");
            scanf("%d",&reg.obitos);

            fwrite(&reg,tamanhoReg,1,file);
            
            printf("Deseja inserir novos dados? (s/n)\n");
            getchar(); // Limpar o buffer do teclado
            scanf("%c",&resposta);
        }

        fclose(file);

        return 0;

}