/*
 * Subor:   proj1.c
 * Datum:   5.11.2017
 * Autor:   1BIT - Jan Zauska, xzausk00@fit.vutbr.cz
 * Projekt: 1BIT Projekt 1: Praca s textom
 * Popis:   https://wis.fit.vutbr.cz/FIT/st/cwk.php.cs
 *          ?title=IZP:Projekt1&csid=652975&id=12214
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>


#define MAX_WORDS 43
#define MAX_WORD_LENGHT 101

char* strToupper(char *s);
void sort(char arr[], int n);
int removeDuplicates(char arr[], int n);
int getInput(char addresses[][MAX_WORD_LENGHT]);
bool startsWith(char *s1, char *s2);
int findMatch(char *start, char addresses[][MAX_WORD_LENGHT],char *echars);
void printEchars(char echars[], int n);

/**
 * Prepise string do uppercase
 * @param s string chtory sa zmeni do uppercase
 * @return s vrati pointer na na string v uppercase
 */
char* strToupper(char *s){
    int i;
    for(i = 0; s[i] != '\0'; i++){
           s[i] = toupper(s[i]);
    }
    return s;
}

/**
 * Jednoduchy bubblesort, zoradi pole od najvacsej hodnoty po najmensiu.
 * @param arr pole charov, ktore chceme zoradit
 * @param n pocet prvkov pola
 */
void sort(char arr[], int n){
    int i, j, temp;
    for (i = 0 ; i < ( n - 1 ); i++){
        for (j = 0 ; j < (n - i - 1); j++){
            if (arr[j] > arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

/**
 * Odstrani z pola prvky, ktore sa tam nachadzaju viac krat.
 * @param arr pole charov, kde chceme odstranit duplicity
 * @param n pocet prvkov pola
 * @return count pocet prvkov upraveneho pola
 */
int removeDuplicates(char arr[], int n){
    int count = 0;
    for(int i = 1; i < n; i++){
        if(arr[i] != arr[count]){
            arr[++count] = arr[i];
        }
    }
    arr[++count] = '\0';
    return  count;
}

/**
 * Nacita mesta zo stdin
 * @param addresses pole, do ktoreho sa ulozia adresy
 * @return EXIT_SUCCESS ak vstup prebehol v poriadku
 *        EXIT_FAILURE ak nastala chyba
 */
int getInput(char addresses[][MAX_WORD_LENGHT]){
    int c;
    int i = 0;
    int j = 0;
    while((c = getchar()) != EOF){
        if(c == '\n'){
            if(j > 0){
                addresses[i][j] = '\0';
                i++;
            }
            j = 0;
            continue;
        }
        if(j >= MAX_WORD_LENGHT - 1){
            fprintf(stderr, "Too long input.\n");
            return EXIT_FAILURE;
        }
        if(i >= MAX_WORDS -1){
            fprintf(stderr, "Too many addresses.\n");
            return EXIT_FAILURE;
        }
        addresses[i][j++] = c;
    }
    addresses[i++][j] = '\0';
    addresses[i][0] = '\0';
    return EXIT_SUCCESS;
}

/**
 * Zisti ci string zacina prefixom
 * @param s1 zaciatok stringu
 * @param s2 string
 * @return true ak string str je zaciatok stringu text alebo sa rovnaju
 *         false ak string str nie je zaciatok stringu text alebo je dlhsi
 */
bool startsWith(char *s1, char *s2){
    if(strlen(s1) > strlen(s2)){
        return false;
    }
    int  i = 0;
    while(s1[i] != '\0'){
        if(toupper(s1[i]) != toupper(s2[i])){
            return false;
        }
        i++;
    }
    return true;
}

/**
 * Najde vsetky znaky, ktore mozu nasledovat
 * @param start zaciatok adresy
 * @param addresses pole, kde su ulozene adresy
 * @param echars pole, do ktoreho sa ulozia enabled znaky
 * @return pos pozicia adresy, ktora zacina stringom start
 */
int findMatch(char *start, char addresses[][MAX_WORD_LENGHT],char *echars){
    int n = strlen(start);
    int j = 0;
    int pos = -1;
    for(int i = 0; addresses[i][0]; i++){
        if(strcmp(addresses[i], start) == 0){
            // aby nevypisalo not found a neprepisalo predosle
            if(pos == -1){
                pos = i;
            }
            printf("Found: %s\n", strToupper(addresses[i]));

        }
        else if(startsWith(start, addresses[i])){
            pos = i;
            echars[j++] = addresses[i][n];
        }
    }
    echars[j] = '\0';
    strToupper(echars);
    return pos;
}

/**
 * Zoradi, vymaze duplicity a vypise enabled chars
 * @param echars pole, v ktorom su ulozene enabled chars
 * @param n dlzka pola echars
 */
void printEchars(char echars[], int n){
    sort(echars, n);
    removeDuplicates(echars, n);
    printf("Enable: %s\n", echars);
}

int main(int argc, char *argv[]){
    char echars[MAX_WORDS];
    char addresses[MAX_WORDS][MAX_WORD_LENGHT];
    char prefix[MAX_WORD_LENGHT];

    // Skontroluje vstupne argumenty
    if(argc < 2){
        prefix[0] = '\0';
    }
    else if(argc > 2){
        fprintf(stderr, "Wrong number of parameters.\n");
        return EXIT_FAILURE;
    }
    else if(argc == 2){
        if(strlen(argv[1]) > MAX_WORD_LENGHT - 1){
            fprintf(stderr, "Argument is too long.\n");
            return EXIT_FAILURE;
        }
        strncpy(prefix, argv[1], MAX_WORD_LENGHT);
    }

    // Nacita mesta a overi, ci vstup prebehol bez problemov
    int error = getInput(addresses);
    if(error){
        return error;
    }

    // Prehlada adresy a najde zhody
    int found = findMatch(prefix, addresses, echars);

    // Vypise vysledok na zaklade poctu enabled chars a premennej found
    int nletters = strlen(echars);
    if(nletters > 1){
        printEchars(echars, nletters);
    }
    else if(nletters == 1){
        printf("Found: %s\n", strToupper(addresses[found]));
    }
    else if(nletters == 0 && found == -1){
        printf("Not found\n");
    }
    return EXIT_SUCCESS;
}
