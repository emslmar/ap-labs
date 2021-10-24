#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define REPORT_FILE "report.txt"

#define HASHSIZE 100000

struct nlist{
    struct nlist *next;
    char *name;
    char *defn;
};

static struct nlist *hashtab[HASHSIZE];
struct nlist* item;

unsigned hash(char *s){
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++){
        hashval = *s + 31 * hashval;
    }
    return hashval%HASHSIZE;
}

struct nlist *lookup(char *s){
    struct nlist *np;
    
    for (np = hashtab[hash(s)]; np != NULL; np = np->next){
        if(strcmp(s, np->name) == 0){
            return np;
        }
    }
    return NULL;
}

struct nlist *install(char *name, char *defn){
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL){
        np = (struct nlist *) malloc(sizeof(*np));
        if(np == NULL || (np->name = strdup(name))==NULL){
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }else{     //already there
        char *newSize;
        newSize = np->defn;
        np->defn = malloc(strlen(np->defn) + strlen(defn) + 50000);
        strcpy(np->defn, newSize);
        strcat(np->defn, "    ");
        strcat(np->defn, defn);
        return NULL;
    }

    if((np->defn = strdup(defn)) == NULL){
        return NULL;
    }

    return np;
    
}

void analizeLog(char *logFile, char *report);


int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Faltan argumentos\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    FILE * fp;
    char * line = NULL;
    char * copyLine;
    char * copyLine2;
    size_t len = 0;
    ssize_t read;

    int arrSize = 100000;
    char *arr[arrSize];
    int arrPos = 0;


    fp = fopen(logFile, "r");
    if (fp == NULL){
        printf("El nombre del archivo es incorrecto\n");
        exit(EXIT_FAILURE);
    }


    while ((read = getline(&line, &len, fp) != -1)){
        copyLine = strdup(line);
        copyLine2 = strdup(line);
        char *tkn;
        char *tkn2;
        char * hour;
        char * rest;
        char * newLine;
        //printf ("Splitting string \"%s\" into tokens:\n",copyLine);
        tkn = strtok(line, "]");
        hour = strdup(tkn);
        //printf ("Hour: %s\n",hour);
        tkn = strtok(NULL, ":");
        rest = strtok(NULL, "");
        if (rest == NULL)
        {
            rest = tkn;
        }
        
        //printf("Rest: %s",rest);
        //printf ("Token:  %s, line: %s",tkn, copyLine);
        tkn2 = strtok(copyLine2, "]");
        tkn2 = strtok(NULL, "");
        if(tkn != NULL){
            newLine = malloc(strlen(hour) + strlen(rest) +4);
            strcpy(newLine,hour);
            strcat(newLine,"]");
            strcat(newLine,rest);
            //printf ("New Line: %s\n",newLine);

            if(strcmp(tkn, tkn2) == 0){
                install("General", copyLine);
            }else{
                install(tkn, newLine);
            }
        }
        
    }
    
    fclose(fp);

    fp = fopen(REPORT_FILE, "w");

    if(fp == NULL){
        printf("Error creating a file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < HASHSIZE; i++){
        if(hashtab[i]!=NULL){
            fprintf(fp, "%s\n", hashtab[i]->name);
            fprintf(fp, "    %s\n", hashtab[i]->defn);
        }
    }

    printf("Report is generated at: [%s]\n", report);
    
    
}



