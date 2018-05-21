// primes.c
// Řešení IJC-DU1, příklad a), 20.3.2018
// Autor: Ján Zauška, FIT VUT
// Přeloženo: gcc 7.2.1

#include<stdio.h>
#include "error.h"
#include "eratosthenes.h"
#include "bit_array.h"

int main(){
    bit_array_create(p, MAX_N);
    Eratosthenes(p);

    unsigned long last10[10];
    unsigned  count = 10;
    for(unsigned long i = MAX_N-1; i > 0; i--){
        if(!bit_array_getbit(p, i)){
            last10[--count] = i;
        }
        if(count == 0) break;
    }

    for(unsigned long i = 0; i < 10; i++){
        printf("%lu\n", last10[i]);
    }
    return 0;
}
