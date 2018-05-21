// htab_free.c
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include"htab.h"


void htab_free(htab_t *t){
    htab_clear(t);
    free(t);
    t = NULL;
}
