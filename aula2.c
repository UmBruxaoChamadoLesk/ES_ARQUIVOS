int main(int argc, char *argv[]){
    /*
    Impressoras eram com cabeça de leitura, cabeça de leituras são virtuais
    r+b é leitura e escrita o professor tbm explicou crlf e lf -> qubra de linha 
    modo a é de escrita e a cabeça de leitura vai para o final-> quando abro está no byte 0
    O prof vai ensinar hoje a como mexer com a cabeça de 
    arquivo de registro tem tamanho fixo
    fread (guarda o endereço, o tamanho de espaço necessário, o tamanho do bloco que está lendo, o arquivo)
    abrir o arquivo, começa um loop, abre de novo antes dele terminar o loop, volta pro loop -> para modelo 
    Printf vai até o nulo -> strcpy já deixa o final com nulo, mas strncpy não
    Struct é continuo -> se faço um x.a = "ABC" x.b = "DEF" se faço isso um strncpy sem dar parametro tem ABCDEF como resultado
    fseek -> recebe 3 parametros o arquivo, a posição em bytes e SEEK_SET posição inicial do arquivo/SEEK_END posição final do arquivo/SEEK_KUR posição que está
    ftell -> da a posição da cabeça absoluta -> ajuda a saber o tamanho do arquivo em bytes
    rewind é um fseek(f, 0, SEEK_SET)
    Ler o ultimo endereço é seek end -sizeof(valor)
    Se o arquivo estiver ordenada em array podemos usar busca binária

    Se sei qual direção seguir eu posso fazer iterativo e não recursivo
    */

}