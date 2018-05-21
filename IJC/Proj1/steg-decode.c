// steg_decode.c
// Řešení IJC-DU1, příklad b), 20.3.2018
// Autor: Ján Zauška, FIT VUT
// Přeloženo: gcc 7.2.1

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include "ppm.h"
#include "error.h"
#include "bit_array.h"
#include "eratosthenes.c"

#define MAX_MESSEGE_LENGTH 10000

int main(int argc, char *argv[]){
    if(argc != 2){
        error_exit("Nespravne pouzitie argumentov.\n");
    }
    char *filename = argv[1];

    struct ppm *map = ppm_read(filename);
    if(map == NULL){
        error_exit("Chyba pri citani suboru \n");
    }
    int size = (map->xsize * map->ysize * 3);

    bit_array_create(p, 222000000);
    Eratosthenes(p);
    int count = 0;
    unsigned int temp = 0;
    unsigned char message[MAX_MESSEGE_LENGTH];
    int i = 11;
    while(i < size){
        if(!bit_array_getbit(p, i)){
            //rovno aj prevedie bity na char
            temp += (map->data[i] & 1)<<(count % CHAR_BIT);
            count ++;
            if(count% CHAR_BIT == 0){
                message[count/CHAR_BIT - 1] = temp;
                if(temp == 0) break;
                temp = 0;
            }
        }
        i++;
    }
    if(i == size){
        free(map);
        error_exit("Nespravne ukoncenie spravy. \n");
    }

    printf("%s\n", message);
    return 0;
}
