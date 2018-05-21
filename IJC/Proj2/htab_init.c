// htab_init.c
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include"htab.h"


htab_t *htab_init(size_t size){
    htab_t *new_htab = malloc(sizeof(htab_t) + size * sizeof(struct htab_listitem*));
    if(new_htab == NULL){
        return NULL;
    }

    new_htab->size = 0;
    new_htab->arr_size = size;
    for(unsigned int k = 0; k < size; k++){
        new_htab->ptr_array[k] = NULL;
    }
    return new_htab;

}
