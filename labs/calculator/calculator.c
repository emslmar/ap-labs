#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values) {
    int result = values[0]; 
    printf("%d", values[0]);
    if(operator == 1){
        for(int i = 1; i < nValues; i++){
            result += values[i];
            printf(" + %d", values[i]);
        }
    }else if(operator == 2){
        for(int i = 1; i < nValues; i++){
            result -= values[i];
            printf(" - %d", values[i]);
        }
    }else if(operator == 3){
        for(int i = 1; i < nValues; i++){
            result *= values[i];
            printf(" * %d", values[i]);
        }
    }
    
    printf(" = %d \n", result);
    return result;
}

int main(int argc, char** argv) {
    //printf("Amount of words: %d\n", argc);
    int operator  = 0; // 1 - suma, 2 - resta, 3 - multiplicacion
    //printf("%s\n", argv[0]); //0 contains the name of the file
    if(strcmp(argv[1], "add") == 0){
        operator = 1;
    }else if(strcmp(argv[1], "sub") == 0){
        operator = 2;
    }else if(strcmp(argv[1], "mult") == 0){
        operator = 3;
    }else{
        printf("No se reconocio el operador, recuerda  que solo aceptamos las palabras sum, sub y mult");
        return -1;
    }

    int  nValues = argc - 2;
    int arr[nValues];
    int pos = 0;
    long lnum;
    char *end;
    for(int i = 2; i < argc; i++){
        lnum = strtol(argv[i], &end, 10);
        if(end == argv[i]){
            printf("ERROR: can't convert string to number\n");
            return 0;
        }else{
            arr[pos] = atoi(argv[i]);
            pos++;
        }
    }
    int *number = arr;

    calc(operator, nValues, number);

    return -1;
}