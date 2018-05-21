// ppm.c
// Řešení IJC-DU1, příklad b), 20.3.2018
// Autor: Ján Zauška, FIT VUT
// Přeloženo: gcc 7.2.1

#include<stdlib.h>
#include<stdio.h>
#include "ppm.h"
#include "error.h"


struct ppm *ppm_read(const char *filename){
    struct ppm *map = malloc(sizeof(struct ppm));

    FILE *f = fopen(filename, "rb");
    if(f == NULL){
        goto error;
    }

    int code = fscanf(f, "P6 %u %u 255 ", &map->xsize, &map->ysize);
    if(code != 2){
        goto error;
    }

    unsigned max_size = 3*map->xsize * map->ysize;
    unsigned i = 0;
    int c;
    while(( c = getc(f)) != EOF){
        if(i > max_size || c > 255){
            goto error;
        }
        map ->data[i] = c;
        i++;
    }

    return map;

    error:
        warning_msg("Chyba pri nacitani suboru.\n");
        free(map);
        if(f != NULL){
            fclose(f);
        }
        return NULL;
}

int ppm_write(struct ppm *p, const char * filename){
    FILE *f = fopen(filename, "wb");
    if(f == NULL){
        error_exit("Nepodarilo sa otvorit subor.\n");
    }

    int code;
    code = fprintf(f, "P6\n%u %u\n255\n", p->xsize, p->ysize);
    if(code < 1){
        error_exit("Problem pri zapisovani do suboru.\n");
    }

    int size = 3* p->xsize * p->ysize;
    code = fwrite(p->data, sizeof(char), size, f);

    if(code != size){
        warning_msg("Problem pri zapisovani dat.\n");
        return -1;
    }
    return 0;
}
