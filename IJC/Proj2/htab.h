// htab.h
// Řešení IJC-DU2, příklad 2), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#ifndef _HTAB_H
#define _HTAB_H
#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

//zistil som, ze ako velkost najlepsie funguju velke prvocisla - najlepsie rozlozenie
//zvolil som rovnake cislo ako C++ program pre testovaci vstup (/usr/share/dict/words)
#define HTAB_SIZE 902483

struct htab_listitem {
    const char *key;
    unsigned int data;
    struct htab_listitem *next;
};

typedef struct{
    size_t size;
    size_t arr_size;
    struct htab_listitem *ptr_array[];
} htab_t;


unsigned int htab_hash_function(const char *str);
htab_t *htab_init(size_t size);
htab_t *htab_move(size_t newsize, htab_t *t2);
size_t htab_size(htab_t *t);
size_t htab_bucket_count(htab_t *t);
struct htab_listitem* htab_lookup_add(htab_t *t, const char *key);
struct htab_listitem* htab_find(htab_t *t, const char *key);
void htab_foreach(htab_t *t, void (*func)(const char *, unsigned int));
bool htab_remove(htab_t *t, const char *key);
void htab_clear(htab_t *t);
void htab_free(htab_t *t);
#endif
