// htab_move.c
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include<stdio.h>
#include"htab.h"


htab_t *htab_move(size_t newsize, htab_t *t2){
    htab_t *new_htab = malloc(sizeof(htab_t) + newsize * sizeof(struct htab_listitem*));
    if(new_htab == NULL){
        return NULL;
    }

    new_htab ->size = t2->size;
    new_htab->arr_size = newsize;
    for(size_t k = 0; k < newsize; k++){
        new_htab->ptr_array[k] = t2->ptr_array[k];
        t2->ptr_array[k] = NULL;
    }
    return new_htab;
}
