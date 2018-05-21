// htab_remove.c
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include"htab.h"


bool htab_remove(htab_t *t, const char *key){
    if(t == NULL){
        fprintf(stderr, "Hashtable je prazdna.\n");
        return NULL;
    }
    if(key == NULL){
        fprintf(stderr, "Kluc je prazdny.\n");
        return NULL;
    }

    unsigned int k = htab_hash_function(key) % t->arr_size;
    struct htab_listitem *item = t->ptr_array[k];
    struct htab_listitem *prev = NULL;
    //hladame item na odstranenie
    while(item != NULL){
        if(strcmp(item->key, key) == 0){
            //nasli sme ho, ak je prvy, v poli ukazovatelov ho nahradime nasledujucim
            if(prev == NULL){
                t->ptr_array[k] = item->next;
            }
            //inak na jeho miesto dame nasledujuci
            else{
                prev->next = item->next;
            }
            free((char *) item->key);
            free(item);
            return true;
        }
        prev = item;
        item = item->next;
    }
    return false;
}
