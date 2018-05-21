// tail.c
// Řešení IJC-DU2, příklad 1 a), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


#define MAX_LINE_LENGTH 1024
#define DEFAULT_LINES 10

typedef struct line{
    char data[MAX_LINE_LENGTH];
    struct line *next;
} line_t;

typedef struct{
    line_t *head;
    line_t *tail;
    unsigned int size;
} queue_t;

queue_t *queue_init(){
    queue_t *q = malloc(sizeof(queue_t));
    if(q == NULL){
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

line_t  *queue_push(queue_t *q, char *data){
    //vytvori novy riadok
    line_t * line = malloc(sizeof(line_t));
    if(line == NULL){
        return NULL;
    }
    line->next = NULL;
    strcpy(line->data, data);

    if(q->head == NULL){
        q->head = line;
        q->tail = line;
        q->size++;
        return line;
    }
    line_t * current = q->tail;
    current->next = line;
    q->tail = line;
    q->size++;
    return line;
}

char * queue_front(queue_t *q){
    return q->head->data;
}

bool queue_pop(queue_t *q){
    if(q->head == NULL){
        return false;
    }
    line_t * temp = q->head->next;
    free(q->head);
    q->head = temp;
    q->size--;
    return true;
}

void queue_delete(queue_t *q){
    line_t *temp = NULL;
    line_t * current = q->head;
    while(current != NULL){
        temp = current;
        current = current->next;
        free(temp);
    }
    free(q);
    q = NULL;
}
void remove_end_of_line(FILE *f){
    int c;
    while (((c = fgetc(f)) != '\n') && (c != EOF));
}

void  read_file(FILE *f, queue_t * q, unsigned int n){
    int overflow = 0;
    char buffer[MAX_LINE_LENGTH];
    while(fgets(buffer, MAX_LINE_LENGTH, f))
    {

        if(strlen(buffer) == MAX_LINE_LENGTH - 1){
            if(overflow == 0){
                fprintf(stderr, "Prekrocena dlzka riadku.\n");
                overflow = 1;
            }
            //skipne zvysok riadku
            remove_end_of_line(f);
            buffer[MAX_LINE_LENGTH - 2] = '\n';
            buffer[MAX_LINE_LENGTH - 1] = '\0';
        }
        if(queue_push(q, buffer) == NULL){
            return;
        }
        if(q->size > n){
            queue_pop(q);
        }
    }

}

void print_lines(queue_t *q){
    while(q->size != 0){
        printf("%s", queue_front(q));
        queue_pop(q);
    }
}

int main(int argc, char *argv[]){
    FILE *f;
    int n = DEFAULT_LINES;

    char *endptr = '\0';
    if(argc == 1){
        f = stdin;
    }
    else if(argc == 2){
        f = fopen(argv[1], "r");
    }
    else if(argc == 3 && strcmp("-n", argv[2])){
        n = strtoul(argv[2], &endptr, 10);
        f = stdin;
    }
    else if(argc == 4 && strcmp("-n", argv[2])){
        n = strtoul(argv[2], &endptr, 10);
        f = fopen(argv[3], "r");
    }
    else{
        fprintf(stderr, "Zle zadane argumenty.\n");
        return -1;
    }
    if(f == NULL){
        fprintf(stderr, "Chyba pri otvarani suboru.\n");
        return -1;
    }
    if(*endptr != '\0' || n <= 0){
        fprintf(stderr, "Nespravny pocet riadkov.\n");
        fclose(f);

        return -1;
    }


    queue_t *q = queue_init();
    read_file(f, q, n);

    print_lines(q);
    queue_delete(q);
    fclose(f);
    return 0;
}
