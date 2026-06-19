#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
int codigo;
char descricao[40];
float preco;
} Produto;

// Variaveis globais ´
FILE *arquivo_dados; // Arquivo de dados ja aberto ´
void *arvore_bplus; // Ponteiro para a arvore B+ inicializada ´

// −−− Assinaturas das func¸oes da ˜ arvore B+ −−− ´
// (assuma que essas func¸oes j ˜ a est ´ ao implementadas em outro m ˜ odulo) ´
int bplus insert(void *arvore, int chave, long posicao);
// Retorna 1 se inseriu, 0 se ja existia ´
int bplus search(void *arvore, int chave, long *posicao);
// Retorna 1 se achou, 0 se nao˜
void bplus traverse(void *arvore, void (*callback)(int chave, long posicao));

int insere(Produto p) {
    if(c=fread(&p,sizeof(produto),1,arquivo_dados))
    {
        insert(arvore_bplus,p.codigo,p.preco);
        fwrite(&p,sizeof(produto),1,arquivo_dados);
        return 1;
    }
    
return 0; // Retorna 1 se inserido com sucesso, 0 se o codigo j ´ a existia. ´
}

void busca(int codigo) {
// Usar a arvore B+ para obter a posic¸ ´ ao no arquivo ˜
// Buscar no arquivo e imprimir
}
void lista() {
// Usar bplus traverse para percorrer e imprimir os produtos
// Requer func¸ao callback auxiliar ˜
}

// Func¸ao callback usada pela ˜ arvore B+ durante a listagem ´
void imprimir produto(int codigo, long posicao) {
// Ler produto da posic¸ao e imprimir ˜
}