// bit_array.h
// Řešení IJC-DU1, příklad a), 20.3.2018
// Autor: Ján Zauška, FIT VUT
// Přeloženo: gcc 7.2.1

#ifndef _BIT_ARRAY_H
#define _BIT_ARRAY_H

#include<limits.h>
#include "error.h"

#define ULONG_BITS (sizeof(unsigned long) * CHAR_BIT)
#define POSITION(index) (1 + (index) / ULONG_BITS)
#define MASK(index) (1UL <<(ULONG_BITS - ((index) % ULONG_BITS) - 1))

typedef unsigned long * bit_array_t;

#define bit_array_create(name, size) \
    unsigned long name[POSITION(size) + ((size % ULONG_BITS) != 0)] = {size, };


#ifndef USE_INLINE

#   define bit_array_size(name) name[0]

#   define bit_array_setbit(name, index, expr) do{\
        if(index >= name[0])\
            error_exit("Index out of range");\
        else if(expr == 1)\
            name[POSITION(index)] |= MASK(index);\
        else\
            name[POSITION(index)] &= ~(MASK(index));\
        } while(0)

#   define bit_array_getbit(name, index)(\
        ((unsigned long) index >= name[0]) \
        ? (error_exit("Index out of range"), 0)\
        : ((name[POSITION(index)] & (MASK(index))) != 0))


#else
    static inline size_t bit_array_size(bit_array_t name){
            return name[0];
    }

    static inline void bit_array_setbit(bit_array_t name, unsigned index, unsigned expr){
        if(index >= name[0])
            error_exit("Index out of range");
        else if(expr == 1)\
            name[POSITION(index)] |= MASK(index);
        else
            name[POSITION(index)] &= ~(MASK(index));
    }

    static inline int bit_array_getbit(bit_array_t name, unsigned index){
        if(index >= name[0])
            error_exit("Index out of range");
        return (name[POSITION(index)] & MASK(index)) != 0;
    }

#endif /* USE_INLINE */
#endif /* _BITARRAY_H */
