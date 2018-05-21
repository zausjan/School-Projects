// wordcount.c
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include<stdio.h>
#include"htab.h"
#include"io.h"

void print_item(const char* key, unsigned int data){
        printf("%s\t%d\n", key, data);
}

int main(){
    htab_t *hash_table = htab_init(HTAB_SIZE);
    if(hash_table == NULL){
        fprintf(stderr, "Chyba pri alokacii hashmapy.\n");
        return 1;
    }
    char word[MAX_WORD_LENGTH + 1];

    int word_length;

    struct htab_listitem* temp = NULL;
    while((word_length = get_word(word, MAX_WORD_LENGTH, stdin)) != EOF){
        temp = htab_lookup_add(hash_table, word);
        if(temp == NULL){
            break;
        }
        temp->data++;

    }

    htab_t *new_htable = htab_move(HTAB_SIZE, hash_table);
    htab_foreach(new_htable, print_item);
    htab_free(new_htable);
    htab_free(hash_table);
    return 0;
}
