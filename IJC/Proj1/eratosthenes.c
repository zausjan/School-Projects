// eratosthenes.c
// Řešení IJC-DU1, příklad a), 20.3.2018
// Autor: Ján Zauška, FIT VUT
// Přeloženo: gcc 7.2.1

#include<stdio.h>
#include<math.h>
#include "eratosthenes.h"


void Eratosthenes(bit_array_t p){
    bit_array_setbit(p, 0, 1);
    bit_array_setbit(p, 1, 1);

    unsigned long SQRT_N  = sqrt(MAX_N);
    for(unsigned long i = 2; i <= SQRT_N; i++){
        if(bit_array_getbit(p, i))
            continue;
        for(unsigned long j = 2*i; j < MAX_N; j += i){
            bit_array_setbit(p, j, 1);
        }
    }

}
