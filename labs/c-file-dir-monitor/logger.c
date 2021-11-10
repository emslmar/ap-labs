#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <syslog.h>
#include "logger.h"

//Emiliano Sloth

int logN = 0; 

int initLogger(char *logType) {
    printf("Log mode changed to %s\n", logType);
    if (strcmp(logType,"stdout") == 0){
        logN = 0;
    } else {
        if (strcmp(logType,"syslog") == 0){
            logN = 1;
        }
        else{
            printf("Error: logtype is invalid");
            return -1;
        }   
    }
    return 0;
}


int infof ( const char * format, ... )
{
    va_list args;
    va_start (args, format);
    if(logN == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_EMERG,format, args);
        closelog();

    } else {
        vprintf (format, args);
        printf("\n");
    }
    va_end (args);
    return 1;
}

int warnf ( const char * format, ... )
{
    va_list args;
    va_start (args, format);
    if(logN == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_EMERG,format, args);
        closelog();
    } else {
        printf("WARN: ");
        vprintf (format, args);
        printf("\n");
    }
    va_end (args);
    printf("\x1b[0m");
    return 1;
}

int errorf ( const char * format, ... )
{
    va_list args;
    va_start (args, format);
    if(logN == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_EMERG,format, args);
        closelog();

    } else {
        printf("ERROR: ");
        vprintf (format, args);
        printf("\n");
    }
    va_end (args);
    return 1;
}

int panicf ( const char * format, ... )
{
    va_list args;
    va_start (args, format);
    if(logN == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_EMERG,format, args);
        closelog();
    } else {
        printf("PANIC: ");
        vprintf (format, args);
        printf("\n");
    }
    va_end (args);
    return 1;
}

