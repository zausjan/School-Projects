// htab_lookup_add.c
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include<string.h>
#include"htab.h"


struct htab_listitem* htab_lookup_add(htab_t *t, const char *key){
    if(t == NULL){
        fprintf(stderr, "Hashtable je prazdna.\n");
        return NULL;
    }
    if(key == NULL){
        fprintf(stderr, "Kluc je pradny.\n");
        return NULL;
    }
    unsigned int k = htab_hash_function(key) % t->arr_size;
    struct htab_listitem *item = t->ptr_array[k];
    struct htab_listitem *prev = NULL;
    //skusime naist hladany item
    while(item != NULL){
        if(strcmp(item->key, key) == 0){
            return item;
        }
        prev = item;
        item = item->next;
    }

    //ak ho nenajdeme, vytvorime novy item
    struct htab_listitem *new_item = malloc(sizeof(struct htab_listitem) + strlen(key) + 1);
    //osetrime chyby pri alokacii
    if(new_item == NULL){
        return NULL;
    }
    new_item->key = malloc(strlen(key) + 1);
    if(new_item->key == NULL){
        fprintf(stderr, "Chyba pri alokacii.\n");
        free(new_item);
        return NULL;
    }
    //nastavime hodnoty key, data a next
    strcpy((char *)new_item->key, key);
    new_item->data = 0;
    new_item->next = NULL;

    //ak nema predchadzajuceho, je prvy - zapiseme ho do pola ukazovatelov
    if(prev == NULL){
        t->ptr_array[k] = new_item;
    }
    //inak ho zaradime na koniec zoznamu
    else{
        prev->next = new_item;
    }
    //zvacsime pocet poloziek
    t->size++;
    return new_item;
}
