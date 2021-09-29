#include <stdio.h>
#include <string.h>

int mystrfind(char *origin, char *substr);
char *mystradd(char *origin, char *addition);
int mystrlen(char *str);

int main(int argc, char** argv) {

    if(argc != 4){
        printf("Faltan o se introdujeron demasiados parametros\n");
        return -1;
    }
    //printf("%d \n", argc);
    //printf("Amount of words: %d\n", argc);
    int operator  = 0; // 1 - add, 2 - find
    //printf("%s\n", argv[0]); //0 contains the name of the file
    if(strcmp(argv[1], "-add") == 0){
        operator = 1;
    }else if(strcmp(argv[1], "-find") == 0){
        operator = 2;
    }else{
        printf("No se reconocio la operacion, recuerda  que solo aceptamos los comandos -add o -find");
        return -1;
    }

    if(operator == 1){
        char *str1 = argv[2];
        char *str2 = argv[3];

        printf("Initial Lenght      : %d\n", mystrlen(str1));
        char* newWord = mystradd(str1, str2);
        printf("New String          : %s\n", newWord);
        int newLen = mystrlen(newWord);
        printf("New length          : %d\n", newLen);
        return 0;
    }

    if(operator == 2){
        char *str1 = argv[2];
        char *str2 = argv[3];
        int pos = mystrfind(str1, str2);
        if(pos == -1){
            printf("No se encontro el substring en el string\n");
            return -1;
        }
        printf("['%s'] string was found at [%d] position\n", str2, pos);
        return 0;
    }

    return -1;
}
