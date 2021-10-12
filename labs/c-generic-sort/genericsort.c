#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINES 5000

char *lines[MAX_LINES];

void quicksort(void *lineptr[], int left, int right, int (*comp)(void *, void *));

void merge_sort(void *lineptr[], int left, int right, int (*comp)(void *, void *));

int numcmp (const char *c1, const char *c2){
        double n1 = atof(c1);
        double n2 = atof(c2);
        if(n1 < n2)
            return -1;
        else if (n1 > n2)
            return 1;
        else
            return 0;
}

void printToFile(int size, char *datos[], char* results){
    FILE* reporte;
    reporte = fopen(results, "w");

    for (int i=0;i<size;i++){
        fprintf(reporte,"%s",(char*)datos[i]);
    }

    fclose(reporte);
    printf("... Results file can be found at ./%s\n",results);

}

int main(int argc, char **argv)
{
    int number = 0; //0 str 1 number
    char* fileName;
    char* producefile;
    int sort = 0; //1 merge - 2 quick
    if(argc < 5 || argc > 6){
        printf("Faltan o se introdujeron demasiados parametros\n");
        return -1;
    }
    if(argc == 5){
        number = 0;
        fileName = argv[1];
        if(strcmp(argv[2], "-mergesort") == 0){
            sort = 1;
        }else if(strcmp(argv[2], "-quicksort") == 0){
            sort = 2;
        }else{
            printf("no se introdujo -mergesort o -quicksort en la posicion correcta \n");
            return -1;
        }

        if (strcmp(argv[3], "-o") != 0)
        {
            printf("no se introdujo -o correctamente \n");
            return -1;
        }
        producefile = argv[4];
    }

    if(argc == 6){
        if(strcmp(argv[1], "-n") != 0){
            printf("no se encontro -n donde corresponde\n");
            return -1;
        }
        number = 1;
        fileName = argv[2];
        if(strcmp(argv[3], "-mergesort") == 0){
            sort = 1;
        }else if(strcmp(argv[3], "-quicksort") == 0){
            sort = 2;
        }else{
            printf("no se introdujo -mergesort o -quicksort en la posicion correcta \n");
            return -1;
        }

        if (strcmp(argv[4], "-o") != 0)
        {
            printf("no se introdujo -o correctamente \n");
            return -1;
        }
        producefile = argv[5];
    }

    FILE *archivo = fopen(fileName, "r");
    
    if(archivo == NULL){
        printf("Error con el nombre del archivo \n");
        return -1;
    }

    char line[256] = {0};
    int n = 0;

    while (fgets(line, 100, archivo))
    {
        lines[n] = (char*)malloc(strlen(line) + sizeof(char*));
        strcpy(lines[n], line);
        n++;
    }

    char *newstr = malloc(strlen(lines[n-1]+2));
    strcpy(newstr, lines[n-1]);
    lines[n-1] = strcat(newstr, "\n");

    if(fclose(archivo)) perror("No se pudo cerrar el file correctamente \n");
    
    if (sort == 1) //mergesort
    {
        merge_sort((void *) lines, 0, n-1, (int(*)(void*, void*)) (number ? numcmp : strcmp));
        printf("... sorting %s file with mergesort \n", fileName);
        printToFile(n, lines, producefile);
        return 0;
    }else if(sort == 2){ //quicksort
        quicksort((void *) lines, 0, n-1, (int(*)(void*, void*)) (number ? numcmp : strcmp));
        printf("... sorting %s file with quicksort \n", fileName);
        printToFile(n, lines, producefile);
        return 0;
    }else{
        printf("No se reconocio si desea hacer mergesort o quicksort \n");
    }
    


}