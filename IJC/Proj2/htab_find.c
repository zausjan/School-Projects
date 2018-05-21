// htab.h
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include"htab.h"


struct htab_listitem* htab_find(htab_t *t, const char *key){
    int k = htab_hash_function(key) % t->arr_size;
    struct htab_listitem *item = t->ptr_array[k];

    while(item != NULL){
        if(strcmp(item->key, key) == 0){
            return item;
        }
        item = item->next;
    }
    return NULL;
}
