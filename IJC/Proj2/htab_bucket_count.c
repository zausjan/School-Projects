// htab_bucket_count.c
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz

#include"htab.h"


size_t htab_bucket_count(htab_t *t){
    return t->arr_size;
}
