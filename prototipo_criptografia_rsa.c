#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>

int menu();
void get_filePrimos();
int gera_pq();
int gera_e();
int checa_primo();
int gera_d();
int crip_descrip();
int criptografar();
int descriptografar();


int main(){
    setlocale(LC_ALL, "portuguese");
    int op, i, n, e, d, m, c, confere;

    op = menu();

    while(op != 4){
        switch(op){
            case 1:
                system("cls");
                int p, q, te, primos[6];

                //  pega primos no vetor
                get_filePrimos(&primos);

                do{
                    do{
                        p = gera_pq(&primos);
                        do{
                            q = gera_pq(&primos);
                        }while(q == p);
                        n = p * q;
                        te = (p - 1) * (q - 1);
                    }while( (te == 4) || (te == 6) );

                    // te = totiente de euler
                    // para te == 4 existe apenas a combinação de chaves e = 5 , d = 5
                    // para te == 6 existe apenas a combinação de chaver e = 3 , d = 3

                    e = gera_e(te);
                    d = gera_d(e, te);
                }while(e == d);

                printf("\tGerar chaves\n\n");
                printf("A sua chave pública é: e = %d , n = %d", e, n);
                printf("\nA sua chave privada é: d = %d , n = %d  ", d, n);

                printf("\n\n");
                system("pause");
            break;


            case 2:
                system("cls");

                printf("\tCriptografar mensagem\n\n");
                printf("Informe a chave pública e o N do usuário que receberá o número criptografado:\n\n");
                printf("Entre com a chave pública (e):   ");
                scanf("%d", &e);
                printf("Entre com o N:   ");
                scanf("%d", &n);

                confere = criptografar(e, n);

                system("cls");

                printf("\tCriptografar mensagem\n\n");
                if(confere == 0){
                    printf("A criptografia ocorreu com sucesso!");
                }else if(confere == 1){
                    printf("Aconteceu um erro na criptografia!\nCertifique-se de que os arquivos.txt estão como o recomendado pelo programa.");
                }

                printf("\n\n");
                system("pause");
            break;


            case 3:
                system("cls");

                printf("\tDesriptografar mensagem\n\n");
                printf("Informe a chave privada e o N do usuário que recebeu o número criptografado:\n\n");
                printf("Entre com a chave privada (d):   ");
                scanf("%d", &d);
                printf("Entre com o N:   ");
                scanf("%d", &n);

                confere = descriptografar(d, n);

                system("cls");

                printf("\tDescriptografar mensagem\n\n");
                if(confere == 0){
                    printf("A descriptografia ocorreu com sucesso!");
                }else if(confere == 1){
                    printf("Ocorreu um erro na descriptografia!\nCertifique-se de que os arquivos.txt estão como o recomendado pelo programa.");
                }

                printf("\n\n");
                system("pause");;
            break;
        }
        op = menu();
    }

    return 0;
}

int menu(){
    int op, aux = 0;
    do{
        system("cls");
        printf("\t\tSistema de Criptografia:\n\n");
        printf("01 - Gerar chaves.\n");
        printf("02 - Criptografar mensagem.\n");
        printf("03 - Descriptografar mensagem.\n");
        printf("04 - Sair.\n");
        printf("\nPor gentileza, escolha uma das opções:   ");
        scanf("%d", &op);
    }while( (op != 1) && (op != 2) && (op != 3) && (op != 4) );
    return op;
}

void get_filePrimos(int *primos){
    int i = 0;
    FILE *F = fopen("arquivos_programa\\primos.txt", "r");
    while(1){
        fscanf(F, "%d", (primos +i));
        if(feof(F)){
            break;
        }
        i++;
    }
    fclose(F);
}

int gera_pq(int *primos){
    srand(time(NULL));
    return primos[rand() % 6];
}

int gera_e(int te){
    srand(time(NULL));
    int e;

    do{
        e = rand() % te-1;
        if(e > 1 && checa_primo(e)){
            if(te%e != 0){
                return e;
            }
        }
    }while(1);
}

int checa_primo(int e){
    int i;

    for (i = 2; i < e; i++){
        if (e%i == 0){
            return 0;
        }
    }

    return 1;
}

int gera_d(int e, int te){
    int d;
    srand(time(NULL));

    do{
        d = rand() % te-1;
        if( (d > 1) && (d*e)%te == 1){
            return d;
        }
    }while(1);
}

int crip_descrip(int exp, int n, int base){
    unsigned long long int aux, i = 0, aux_exp = 1, aux_base = 1;

    do{
        aux = 0;
        aux_exp = 1;
        do{
            aux++;
            exp--;
        }while(aux != 2 && exp != 0);

        for(i = 1; i <= aux; i++){
            aux_exp = aux_exp * base;
        }

        aux_base = aux_base * (aux_exp%n);

    }while(exp != 0);

    return aux_base%n;
}

int criptografar(int e, int n){
    int a = 0, crip, num_arq, aux = 0;
    char arq_c;

    FILE *men = fopen("arquivos_usuario\\mensagem.txt", "r");
    FILE *cif = fopen("arquivos_usuario\\cifragem.txt", "w");

    if(men == NULL || cif == NULL){
        return 1;
    }

    do{
        fscanf(men, "%c", &arq_c);
        num_arq = arq_c - '0'; // converte em número inteiro

        if(feof(men) != 0){
            aux = 1;
        }

        a = a*10 + num_arq;

        if (a >= n || aux == 1){
            if(a >= n){
                a = a/10;
            }

            fseek(men, -1, 1);

            crip = crip_descrip(e, n, a);  //faz a criptografia

            fprintf(cif, "%d ", crip);
            a = 0;
        }

    }while(aux == 0);

    fclose(men);
    fclose(cif);

    return 0;
}

int descriptografar(int d, int n){
    int a = 0, descrip, num_arq;
    char arq_c;

    FILE *cif = fopen("arquivos_usuario\\cifragem.txt", "r");
    FILE *men = fopen("arquivos_usuario\\mensagem.txt", "w");

    if(cif == NULL || men == NULL){
        return 1;
    }

    fscanf(cif, "%c", &arq_c);

    do{
        if(arq_c != ' '){
            num_arq = arq_c - '0'; // converte em número inteiro

            a = a*10 + num_arq;
        }else{
            descrip = crip_descrip(d, n, a);  //faz a descriptografia

            fprintf(men, "%d", descrip);

            a = 0;
        }

        fscanf(cif, "%c", &arq_c);
    }while(feof(cif) == 0);

    fclose(cif);
    fclose(men);

    return 0;
}
