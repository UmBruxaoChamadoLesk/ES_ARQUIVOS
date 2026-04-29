#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int main(int argc, char **argv)
{
    FILE *f;
    Endereco e;
    int qt, c,inicio,meio,fim;

    if(argc != 2)
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
	}

    c = 0;

    printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));
	f = fopen("cep_ordenado.dat","r");
    fseek(f,0,SEEK_END);
    int TamanhoBytes = ftell(f);
    printf("\nTAMANHO %ld",TamanhoBytes);
    int TamanhoRegistros = TamanhoBytes/sizeof(Endereco);
    inicio = 0;
    fim = TamanhoRegistros - 1;
    meio = (inicio+fim)/2;
    fseek(f,meio*sizeof(Endereco),SEEK_SET);

    if(!f){
		fprintf(stderr, "Arquivo não encontrado", argv[1]);
		return 1;
	}

    qt=fread(&e,sizeof(Endereco),1,f);

    while(inicio <= fim)
	{
        c++;
        meio = (inicio + fim)/2;
        fseek(f,meio*sizeof(Endereco),SEEK_SET);
        fread(&e,sizeof(Endereco),1,f); 

        //printf("\nMEIO %ld",meio);

        if(strncmp(argv[1],e.cep,8)==0)
		{
			printf("\n%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
			break;//não é null terminated, então precisa desse .72s para que tenha a quebra de linha
		}
		else if(strncmp(argv[1],e.cep,8) < 0)
        {
            fim=meio-1;
            //printf("\nFIM %ld",fim);
        }
        else
        {
            inicio=meio+1;
            //("\nINICIO %ld",inicio);
        }
		// argv[1] > e.cep  => strcmp(argv[1],e.cep) > 0
		// argv[1] == e.cep  => strcmp(argv[1],e.cep) == 0
		// qt = fread(&e,sizeof(Endereco),1,f);		
	}

    
	printf("\nTotal Lido: %d\n", c);
	fclose(f);
    

    return 0;
}