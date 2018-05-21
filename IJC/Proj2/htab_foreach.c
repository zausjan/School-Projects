// htab_foreach.c
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz

#include"htab.h"


void htab_foreach(htab_t *t, void (*func)(const char *, unsigned int)){
    if(t == NULL){
        fprintf(stderr, "Hashtable je prazdna.\n");
        return;
    }
    if(func == NULL){
        fprintf(stderr, "Zle zadana funkcia.\n");
        return;
    }
    struct htab_listitem *item = NULL;
    for(unsigned int k = 0; k < t->arr_size; k++){
        item = t->ptr_array[k];
        while(item != NULL){
            func(item->key, item->data);
            item = item->next;
        }
    }
}
