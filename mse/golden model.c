#include <time.h> // necessaria para a utilizacao da funcao rand
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //necessária para a utilizacao da funcao memcmp
#include <math.h>
#define bits 8 // Quantidade de bits por numero
#define bitspot 16 // bits*2
#define nmat 1 // Quantidade de matrizes
#define nnum 64 // Quantidade de numeros por matriz
#define numzeros 6 // Numero de 0 para concatenar

// Função para converter um número decimal para binário
void dec_para_bin(int decimal, int bin[bitspot]) {
    int bit_count = bitspot; 

    // Preencher o array de bits com a representação binária do número
    for (int i = bit_count - 1; i >= 0; i--) {
        bin[i] = decimal % 2;
        decimal /= 2;
    }
}

int bin_para_decimal(int *bin, int n) { // Func conversao binario para decimal
    int decimal = 0;
    for (int i = 0; i < n; i++) {
        decimal = (decimal << 1) | bin[i];
    }
    return decimal;
}

int main() {
    int i, num1[bits], num2[bits], sub[bits], pot[(bits*2)-1], n, bin[bitspot], bin_con[(bitspot+numzeros)],resultado[(bits+bits)], soma = 0, divi = 0;  //array para armazenar numeros 1 e 2, bem como o valor da subtracao
    FILE *arq;

    srand(time(NULL));
    arq = fopen("estimulos1.dat", "w"); // abrindo arquivo

    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    for (int u = 0; u < nmat; u++) {
        for (int j = 0; j < nnum; j++) {
            // Gerar primeiro número de 8 bits
            for (i = 0; i < bits; i++) {
                num1[i] = 1;       // valor aleatório entre 0 e 1 repetido 8 vezes para gerar o valor de 8bits
            }
            // Gerar segundo número de 8 bits
            for (i = 0; i < bits; i++) {
                num2[i] = 0;
            }
            
            // Determinar qual número é maior para poder subtrair sem a preocupacao do sinal
	    // Para evitar ter que calcular o módulo da subtracao

            int borrow = 0; // variavel auxiliar para a subtracão dos valores
            if (memcmp(num1, num2, bits * sizeof(int)) < 0) { // avalia o tamanho do array
                // num1 < num2, então subtrair num1 de num2
                for (i = bits - 1; i >= 0; i--) {
                    int diff = num2[i] - num1[i] - borrow;
                    if (diff < 0) {
                        diff += 2;
                        borrow = 1;
                    } else {
                        borrow = 0;
                    }
                    sub[i] = diff;
                }
            } else {

                // num1 >= num2, então subtrair num2 de num1

                for (i = bits - 1; i >= 0; i--) {
                    int diff = num1[i] - num2[i] - borrow;
                    if (diff < 0) {
                        diff += 2;
                        borrow = 1;
                    } else {
                        borrow = 0;
                    }
                    sub[i] = diff;
                }
            }
            
            int decimal = bin_para_decimal(sub, bits);
            int pot = pow(decimal,2);
            dec_para_bin(pot, bin);
            
            // Preencher os `numzeros` bits mais significativos de `bin_con` com zeros
            for (i = 0; i < numzeros; i++) {
                bin_con[i] = 0;
            }

            // Copiar os bits de `bin` para `bin_con` a partir de `numzeros`
            for (i = numzeros; i < bitspot + numzeros; i++) {
                bin_con[i] = bin[i - numzeros];
            }
            soma = soma + bin_para_decimal(bin_con, (bitspot+numzeros));
            
            divi = soma/(bits*bits);
            dec_para_bin(divi, resultado);
            
            // Escreve num1 no arquivo bit a bit
            for (i = 0; i < bits; i++) {
                fprintf(arq, "%d", num1[i]);
            }
            fprintf(arq, " "); // Separador das colunas
	    // Escreve num2 no arquivo bit a bit
            for (i = 0; i < bits; i++) {
                fprintf(arq, "%d", num2[i]);
            }  
            /* fprintf(arq, " "); // Separa a coluna do valor de SAD
	    // Escreve o valor da SAD bit a bit
            for (i = 0; i < bits; i++) {
                fprintf(arq, "%d", sub[i]);
            } 
            fprintf(arq, " Subtracao:%d", decimal);
            fprintf(arq, " Potencia:%d", pot);
            fprintf(arq, " "); 
        // Imprimir o array binário
            /*for (int i = 0; i < bitspot; i++) {
                fprintf(arq, "%d", bin[i]);
            }
                        fprintf(arq, " ");
        // Imprimir o array binário
            for (int i = 0; i < (bitspot+numzeros); i++) {
                fprintf(arq, "%d", bin_con[i]);
            } */
           // fprintf(arq, " Somador:%d", soma);
           // fprintf(arq, " Divisao:%d", divi);
           // fprintf(arq, " Resultado: ");
            for (int i = 0; i < (bits+bits); i++) {
              //  fprintf(arq, "%d", resultado[i]);
            } 
            fprintf(arq, "\n"); // Nova linha no arquivo

        }
       // fprintf(arq, "\n"); // Nova linha para separar as matrizes
    }
                            for (int i = 0; i < (bits+bits); i++) {
                //fprintf(arq, "%d", resultado[i]);
            } 

    fclose(arq); // fecha o arquivo
    
          FILE *arq2, *arq3;
          char numx[bits+1], numy[bits+1];

    // Abrir o arquivo para leitura
    arq2 = fopen("estimulos1.dat", "r");
    arq3 = fopen("estimulos.dat", "w");

    if (arq2 == NULL || arq3 == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    // Ler os dados do arquivo e exibi-los na tela
    while (fscanf(arq2, "%s %s", numx, numy) != EOF) {
        fprintf(arq3, "%s %s ", numx, numy);
        
            for (int i = 0; i < (bits+bits); i++) {
                fprintf(arq3, "%d", resultado[i]);
            } 
             fprintf(arq3, "\n");
        
    }

    // Fechar o arquivo
    fclose(arq2);
    fclose(arq3);

    return 0;
}