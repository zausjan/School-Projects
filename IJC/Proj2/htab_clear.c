// htab_clear.c
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include"htab.h"


void htab_clear(htab_t *t){
    if(t == NULL){
        return;
    }
    struct htab_listitem *item = NULL;
    struct htab_listitem *temp = NULL;
    t->size = 0;
    for(unsigned int k = 0; k < t->arr_size; k++){
        item = t->ptr_array[k];
        t->ptr_array[k] = NULL;

        while(item != NULL){
            temp = item;
            item = item->next;
            free((char *) temp->key);
            free(temp);
        }
    }
}
