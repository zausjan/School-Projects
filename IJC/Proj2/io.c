// io.c
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include"io.h"


int get_word(char *s, int max, FILE *f){
    int c;
    while((c = fgetc(f)) != EOF){
        if(! isspace(c)){
            ungetc(c, f);
            break;
        }
    }
    if(c == EOF){
        return EOF;
    }
    int i = 0;
    int overflow = 0;
    while((c = fgetc(f)) != EOF){
        if(i == (MAX_WORD_LENGTH - 2) && overflow == 0){
            overflow = 1;
            fprintf(stderr, "Prekrocena dlza slova.\n");
            break;
        }
        if(! isspace(c) && overflow == 0){
            s[i++] = c;
        }
        else{
            ungetc(c, f);
            break;
        }
    }
    s[i++] = '\0';
    if(c == EOF){
        return EOF;
    }
    return i;
}
