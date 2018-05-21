// error.c
// Řešení IJC-DU1, příklad b), 20.3.2018
// Autor: Ján Zauška, FIT VUT
// Přeloženo: gcc 7.2.1

#include<stdarg.h>
#include<stdlib.h>
#include<stdio.h>
#include "error.h"

void warning_msg(const char *fmt, ...){
    va_list args;

    va_start(args, fmt);
    fprintf(stderr,"CHYBA: ");
    vfprintf(stderr, fmt, args);
}

void error_exit(const char *fmt, ...){
    va_list args;

    va_start(args, fmt);
    fprintf(stderr,"CHYBA: ");
    vfprintf(stderr, fmt, args);
    exit(1);
}
