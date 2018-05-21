#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/mman.h>
#include<stdarg.h>


#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

#define MUTEX_SEMAPHORE_NAME "/xzausk00.mutex.sem"
#define BUS_SEMAPHORE_NAME "/xzausk00.bus.sem"
#define BOARDED_SEMAPHORE_NAME "/xzausk00.boarded.sem"
#define WRITE_SEMAPHORE_NAME "/xzausk00.write.sem"
#define DRIVING_SEMAPHORE_NAME "/xzausk00.driving.sem"
#define LOCKED 0
#define UNLOCKED 1


typedef struct{
    unsigned  R;
    unsigned  C;
    unsigned  ART;
    unsigned  ABT;
} params_t;

unsigned *waiting;
unsigned *transported;
unsigned *action_id;

sem_t *mutex = NULL;
sem_t *bus = NULL;
sem_t *boarded = NULL;
sem_t *write_s = NULL;
sem_t *bus_driving = NULL;

FILE *output = NULL;

void init();
void clear_all();
void print_action(const char *fmt, ...);
unsigned transport(unsigned c);
void take_ride(unsigned id);
void create_bus(unsigned c, unsigned r, unsigned abt);
void create_rider(unsigned n, unsigned art);

void print_action(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);

    vfprintf(output, fmt, args);
    fflush(output);
    (*action_id)++;
    va_end(args);
}

void init(){
    if ((waiting = mmap(NULL, sizeof(unsigned), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED){
        fprintf(stderr, "Chyba zdielanej pamate.\n");
        clear_all();
        exit(1);
    }
    *waiting = 0;

    if ((action_id= mmap(NULL, sizeof(unsigned), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED){
        fprintf(stderr, "Chyba zdielanej pamate.\n");
        clear_all();
        exit(1);
    }
    *action_id = 1;

    // create  locked semaphores
    if ((mutex = sem_open(MUTEX_SEMAPHORE_NAME, O_CREAT | O_EXCL, 0666, UNLOCKED)) == SEM_FAILED)
    {
        fprintf(stderr, "Chyba pri vytvarani semaforu.\n");
        clear_all();
        exit(1);
    }

    if ((boarded = sem_open(BOARDED_SEMAPHORE_NAME, O_CREAT | O_EXCL, 0666, LOCKED)) == SEM_FAILED)
    {
        fprintf(stderr, "Chyba pri vytvarani semaforu.\n");
        clear_all();
        exit(1);
    }

    if ((bus = sem_open(BUS_SEMAPHORE_NAME, O_CREAT | O_EXCL, 0666, LOCKED)) == SEM_FAILED)
    {
        fprintf(stderr, "Chyba pri vytvarani semaforu.\n");
        clear_all();
        exit(1);
    }
    if ((write_s = sem_open(WRITE_SEMAPHORE_NAME, O_CREAT | O_EXCL, 0666, UNLOCKED)) == SEM_FAILED)
    {
        fprintf(stderr, "Chyba pri vytvarani semaforu.\n");
        clear_all();
        exit(1);
    }
    if ((bus_driving = sem_open(DRIVING_SEMAPHORE_NAME, O_CREAT | O_EXCL, 0666, LOCKED)) == SEM_FAILED)
    {
        fprintf(stderr, "Chyba pri vytvarani semaforu.\n");
        clear_all();
        exit(1);
    }
}

unsigned transport(unsigned c){
    if(*waiting == 0) {
        sem_post(mutex);
        return 0;
    }

    sem_wait(write_s);
    print_action("%u:\tBUS:\tstart boarding:%u\n",*action_id, *waiting);
    sem_post(write_s);
    unsigned n = min(*waiting, c);
    for(unsigned i = 0; i < n; i++){
        sem_post(bus);
        sem_wait(boarded);
    }
    *waiting = (*waiting>c)?(*waiting-c):0; //TODO

    sem_wait(write_s);
    print_action("%u:\tBUS:\tend boarding:%u\n",*action_id,  *waiting);
    sem_post(write_s);
    sem_post(mutex);
    return n;
}

void take_ride(unsigned id){
    sem_wait(mutex);
        (*waiting)++;
    sem_post(mutex);

    sem_wait(write_s);
    print_action("%u:\tRID %u:\tenter:%u\n",*action_id, id, *waiting);
    sem_post(write_s);
    sem_wait(bus);

    sem_wait(write_s);
    print_action("%u:\tRID %u:\tboarding\n",*action_id, id);
    sem_post(write_s);
    sem_post(boarded);

    sem_wait(bus_driving);
    sem_wait(write_s);
    print_action("%u:\tRID %u:\tfinish\n",*action_id, id);
    sem_post(write_s);
    exit(0);
}

void create_bus(unsigned c, unsigned r, unsigned abt){
    sem_wait(write_s);
    print_action("%u:\tBUS:\tstart\n",*action_id);
    sem_post(write_s);
    unsigned transported = 0;
    while(transported < r || *waiting  > 0){
        sem_wait(mutex);

        sem_wait(write_s);
        print_action("%u:\tBUS:\tarrival\n",*action_id);
        sem_post(write_s);
        int n =transport(c);
        transported += n;

        sem_wait(write_s);
        print_action("%u:\tBUS:\tdepart\n",*action_id);
        sem_post(write_s);
        if(abt != 0){
            unsigned delay = rand() % abt;
            usleep(delay*1000);
        }
        sem_wait(write_s);
        print_action("%u:\tBUS:\tend\n",*action_id);
        sem_post(write_s);
        for(int i = 0; i < n; i++){
            sem_post(bus_driving);
        }
    }
    sem_wait(write_s);
    print_action("%u:\tBUS:\tfinish\n",*action_id);
    sem_post(write_s);
    exit(0);
}

void create_rider(unsigned n, unsigned art){
    unsigned i;
    pid_t pid;
    unsigned delay = 0;
    pid_t *children = malloc(n*sizeof(pid_t));
    for(i = 1; i < n+1; i++){
        if(art != 0){
            delay = rand() % art;
            usleep(delay * 1000);
        }
        pid = fork();
        if(pid < 0){
            fprintf(stderr, "Chyba pri vytvarani potomka.\n");
            free(children);
            clear_all();
            kill(0, SIGKILL);
        }
        else if(pid > 0){
              children[i-1] = pid;
        }
        else{ //PID == 0
             sem_wait(write_s);
             print_action("%u:\tRID %u:\tstart\n",*action_id, i);
             sem_post(write_s);
             take_ride(i);
             exit(0);
        }

    }
    for (unsigned i = 0; i < n; i++){
        if(waitpid(children[i], NULL, 0) == -1){
           fprintf(stderr,"%s","Nepodarilo sa pockat na dokoncenie potomka\n");
        }
    }
    free(children);
    exit (0);
}

void clear_all(){
        // close semaphores
        sem_close(mutex);
        sem_close(bus);
        sem_close(boarded);
        sem_close(write_s);
        sem_close(bus_driving);

        // remove semaphores
        sem_unlink(MUTEX_SEMAPHORE_NAME);
        sem_unlink(BUS_SEMAPHORE_NAME);
        sem_unlink(BOARDED_SEMAPHORE_NAME);
        sem_unlink(WRITE_SEMAPHORE_NAME);
        sem_unlink(DRIVING_SEMAPHORE_NAME);

        //unmap shared variables
        munmap(waiting, sizeof(unsigned));
        munmap(action_id,sizeof(unsigned));

        //close output file
        if(output != NULL){
            fclose(output);
        }
}


int main(int argc, char *argv[]){

    if(argc != 5){
        fprintf(stderr, "Nespravny pocet argumentov\n");
        return 1;
    }

    char *endptr = '\0';
    params_t params;

    params.R = strtoul((argv[1]),&endptr,10);
    if (*endptr != '\0'){
      fprintf(stderr,"%s","Nespravny parameter\n");
      return 1;
    }
    params.C = strtoul((argv[2]),&endptr,10);
    if (*endptr != '\0'){
      fprintf(stderr,"%s","Nespravny parameter\n");
      return 1;
    }
    params.ART = strtoul((argv[3]),&endptr,10);
    if (*endptr != '\0'){
      fprintf(stderr,"%s","Nespravny parameter\n");
      return 1;
    }
    params.ABT = strtoul((argv[4]),&endptr,10);
    if (*endptr != '\0'){
      fprintf(stderr,"%s","Nespravny parameter\n");
      return 1;
    }

    output = fopen("proj2.out", "w");
    if(output == NULL){
        fprintf(stderr, "Chyba pri otvarani suboru.\n");
        return 1;
    }
    // prevent buffering of the output
    setbuf(output, NULL);
    init();
    srand(getpid());
    pid_t pid = fork();

    if(pid == 0){
        create_bus(params.C, params.R, params.ABT);
    }
    else if(pid>0){
        pid_t r_pid = fork();
        if(r_pid == 0){
            create_rider(params.R, params.ART);
        }
        else if (r_pid < 0){
            fprintf(stderr, "Chyba pri forkovani.\n");
            exit(1);
        }
        if(waitpid(r_pid, NULL, 0) == -1){
           fprintf(stderr,"%s","Nepodarilo sa pockat na dokoncenie potomka\n");
        }
    }
    else{
        clear_all();
        fprintf(stderr, "PID error.\n");
        return 1;
    }

    // wait for child to end
    waitpid(pid, NULL, 0);
    clear_all();

    return 0;
}
