#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

typedef struct
{
    char placa[9];
    char modelo[50];
    char cor[12];
    int status;
}carro;

int validar_placa(char *placa)
{
    if(strlen(placa) != 7)
    {
       printf("Erro!\nPlaca inválida\nPlaca deve ter 7 caractéres(3 letras e 4 números)\n");
       return 1;
    }

    for(int i = 0; i < 3;i++)
    {
        placa[i] = toupper(placa[i]);
        if(!isalpha(placa[i]))
        {
           printf("Erro!\nPlaca inválida\nPlaca deve ter 8 caractéres(3 letras e 4 números)\n");
           return 1;
        }
    }

    for(int i = 3; i < 7;i++)
    {
        if(!isdigit(placa[i]))
        {
            printf("Erro!\nPlaca inválida\nPlaca deve ter 8 caractéres(3 letras e 4 números)\n");
            return 1;
        }
    }

    return 0;
}

carro lecarro()
{
    carro aux;
    int erro = 1;

    while(erro != 0)
    {
        printf("Placa: ");
        scanf("%s",aux.placa);
        getchar();

        erro = validar_placa(aux.placa);
    }

    printf("Modelo: ");
    fgets(aux.modelo , 50 , stdin);
    aux.modelo[strlen(aux.modelo) - 1] = '\0';

    printf("Cor: ");
    fgets(aux.cor , 12 , stdin);
    aux.cor[strlen(aux.cor) - 1] = '\0';

    aux.status = 1;

    return aux;
}

int main()
{
    setlocale(LC_ALL,"Portuguese");
    char escolha = '0';
    int tam = 0;
    carro *novocarro, aux;
    FILE *arq = fopen("Lista.bin", "rb+");

    if(arq == NULL)
        printf("Erro! Arquivo não encontrado!");
    else
        printf("Arquivo Encontrado!");

    while(fread(&aux , sizeof(carro), 1 , arq) == 1)
        tam++;

    novocarro = malloc((tam + 1) * sizeof(carro));

    while(escolha != '4')
    {
        printf("\nBem-Vindo!\nMenu:\n1.Estacionar carro\n2.Apagar carro\n3.Listar carro\n4.Sair\nEscolha: ");
        scanf(" %s",&escolha);

        switch(escolha)
        {
        case '1':
            printf("\033[H\033[J");
            novocarro = realloc(novocarro, (tam + 1) * sizeof(carro));
            printf("1.Estacionar carro!\n");
            novocarro[tam] = lecarro();
            fseek(arq , 0 , SEEK_END);
            fwrite(&novocarro[tam], sizeof(carro), 1 , arq);
            tam++;
            printf("Carro estacionado com sucesso!\n");
            break;

        case '2':
            rewind(arq);

            int i = 0, j = 0, escolha2 = 0;
            printf("\033[H\033[J");
            printf("2.Apagar carro!\nListando opções.....\n");

            if(tam == 0)
            {
                printf("Não há carros para serem exibidos!\n");
                break;
            }

            fread(novocarro, sizeof(carro), tam ,arq);

            for(int i = 0; i < tam; i++)
                {
                    printf("------------------------------------\nCarro %d:\n", i + 1);
                    printf("Placa: %s\nModelo: %s\nCor: %s\nStatus: %d\n",novocarro[i].placa,novocarro[i].modelo,novocarro[i].cor,novocarro[i].status);
                }

            printf("\nEscolha um carro para apagar:\n");
            printf("Escolha: ");
            scanf("%d", &escolha2);

            while(escolha2 > tam || escolha2 < 1)
            {
                printf("Erro! Escolha inválida!\nEscolha deve estar entre as opções acima!\nEscolha: ");
                scanf("%d", &escolha2);
            }

            printf("\n%d\n",escolha2 - 1);

            escolha2 -= 1;

            FILE *temp = fopen("temp.bin", "wb");

            if(temp == NULL){
                printf("Erro ao abrir o arquivo temporário!");
                break;
            }

            for(int i = 0; i < tam; i++)
            {
                if(i != escolha2)
                {
                    novocarro[j] = novocarro[i];
                    fwrite(&novocarro[j], sizeof(carro), 1 , temp);
                    j++;
                }
            }

            fclose(temp);
            fclose(arq);

            remove("Lista.bin");
            rename("temp.bin","Lista.bin");

            tam--;

            novocarro = realloc(novocarro, tam * sizeof(carro));

            printf("Remoção feita com sucesso!");

            arq = fopen("Lista.bin","rb+");

            break;

        case '3':
            printf("\033[H\033[J");
            printf("3.Listar carro!\n");
            rewind(arq);

            i = 0;

            if(tam == 0)
                printf("Não há carros para serem exibidos!\n");
            else
            {
                while(fread(&aux, sizeof(carro), 1 , arq) != 0)
                {
                    printf("------------------------------------\nCarro %d:\n", i + 1);
                    printf("Placa: %s\nModelo: %s\nCor: %s\nStatus: %d\n",aux.placa,aux.modelo,aux.cor,aux.status);
                    i++;
                }
            }
            break;
        case '4':
            break;

        default:
            printf("Erro!\nEscolha deve estar entre as opções acima!\nTente Novamente!\n");
            break;
        }
    }

    printf("Obrigado por utilizar o programa!\nAté mais");

    free(novocarro);
    fclose(arq);

    return 0;
}
