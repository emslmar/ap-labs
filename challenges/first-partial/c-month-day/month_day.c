#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static char daytab[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/* month_name:  return name of n-th month */
char *month_name(int n) {

   static char *name[] = {
       "Illegal month",
       "Jan", "Feb", "Mar",
       "Apr", "May", "Jun",
       "Jul", "Aug", "Sep",
       "Oct", "Nov", "Dec"
   };

   return (n < 1 || n > 12) ? name[0] : name[n];
}

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){
    int numMonth;
    int copyYearDay = yearday;
    int leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    for(int i = 0; i <= 12; i++){
        if(yearday <= daytab[leap][i]){
            *pday = yearday;
            *pmonth = i;
            break;
        }else{
            yearday = yearday - daytab[leap][i];
        }
    }

}

int main(int argc, char** argv) {
    //printf("%d\n", argc);
    if(argc < 3){
        printf("No se ingresaron los parametros suficientes\n");
        return -1;
    }else if(argc > 3){
        printf("Se ingresaron demasiados parametros\n");
        return -1;
    }
    

    int year;
    int yearday;
    int leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    long lnum;
    char *end;  
    lnum = strtol(argv[1], &end, 10);
    if(*end != '\0'){
        printf("ERROR: el parametro del año no es numerico\n");
        return 0;
    }else{
        year = atoi(argv[1]);
    }

    lnum = strtol(argv[2], &end, 10);
    if(*end != '\0'){
        printf("ERROR: el parametro del dia no es numerico\n");
        return 0;
    }else{
        yearday = atoi(argv[2]);
    }

    if(year < 1){
        printf("ERROR: el año es invalido\n");
        return 0;
    }else if(yearday > 365 && leap == 0){
        printf("ERROR: dia excede los 365 dias del año en un año no bisiesto\n");
        return 0;
    }else if(yearday > 366 && leap == 1){
        printf("ERROR: dia excede los 366 dias del año en un año bisiesto\n");
        return 0;
    }

    
    int month;
    int day;
    
    month_day(year, yearday, &month, &day);

    char *monthName = month_name(month);
    printf("%s %02d, %d\n", monthName, day, year);

    return 0;
}








