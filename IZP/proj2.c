/*
 * Subor:   proj2.c
 * Datum:   19.11.2017
 * Autor:   1BIT - Jan Zauska, xzausk00@fit.vutbr.cz
 * Projekt: 1BIT Projekt 2: Iteracne vypocty
 * Popis:   https://wis.fit.vutbr.cz/FIT/st/cwk.php.cs
 *          ?title=IZP:Projekt2&csid=652975&id=12214
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>


#define MAX_ITERATIONS 13
#define ENOUGH_ITERATIONS 10 // na zaklade pozorovania
#define MAX_ANGLE 1.4
#define MAX_HEIGHT 100
#define DEFAULT_HEIGHT 1.5

enum ecodes{
    E_SUCCESS,
    E_WRONG_PARAM,
    E_WRONG_VALUE
};

enum states{
    S_HELP,
    S_TAN,
    S_DISTANCE,
    S_HEIGHT
};

const char *ECODE_MSG[] = {
    [E_SUCCESS] = "All OK",
    [E_WRONG_PARAM] = "Wrong use of parameters",
    [E_WRONG_VALUE] = "Wrong parameter value"
};

typedef struct params{
    unsigned int N;
    unsigned int M;
    double A;
    double B;
    double X;
    int ecode;
    int state;
} params_t;

void help();
double check_double(char *str);
unsigned int check_uint(char *str);
params_t get_params(int argc, char *argv[]);
void check_params(params_t *params);
double my_pow(double x, unsigned int n);
double my_fabs(double x);
double taylor_tan(double x, unsigned int n);
double cfrac_tan(double x, unsigned int n);
void print_tan(double A, unsigned int N, unsigned int M);
double calc_diS_TANce(double A, double c);
double calc_height(double B, double c, double distance);

/**
 * Vypise napovedu
 * @param   void
 * @return  void
 */
void help(){
    printf(
        "Usage: ./proj2 --help                                              \n"
        "       ./proj2 --tan A N M                                         \n"
        "       ./proj2 [-c X] -m A [B]                                     \n"
        "                                                                   \n"
        " \t--help    displays this help and exits                          \n"
        " \t--tan     calculates tan(A) for iterations in range N M         \n"
        " \t-c        sets observer height to X, default is 1.5             \n"
        " \t-m        calculates diS_TANce of object from A and X,          \n"
        " \t          calculates height too if B is entered                 \n"
        " \tA         angle (see description)                               \n"
        " \t          0 < A <= 1.4 < π/2                                    \n"
        " \t                                                                \n"
        " \tB         angle (see description)                               \n"
        " \t          0 < B <= 1.4 < π/2                                    \n"
        " \t                                                                \n"
        " \tX         height of measurement tool                            \n"
        " \t          0 < X <= 100                                          \n"
        " \t                                                                \n"
        " \tN         number of iteration to start calculate with           \n"
        " \t          0 < N <= M < 14                                       \n"
        " \t                                                                \n"
        " \tM         number of iteration to stop calculate with            \n"
        " \t          0 < N <= M < 14                                       \n"
        " \t                                                                \n"
        " \t                                                                \n"
    );
}

/**
 * Spracuje vstupne argumenty a overi ich hodnoty
 * @param   argc    pocet argumentov
 * @param   argv    pole argumentov
 * @return  result  struktura obsahujuca vsetky argumenty, stav a kod chyby
 */
params_t get_params(int argc, char *argv[]){
    params_t result =
    {
        .N = 0,
        .M = 0,
        .A = 0,
        .B = 0,
        .X = 1.5,
        .ecode = E_SUCCESS,
        .state = S_HELP
    };

    if(argc == 2 && (strcmp(argv[1], "--help") == 0)){
        result.state = S_HELP;
    }
    else if(argc == 5 && strcmp(argv[1], "--tan") == 0){
        result.state = S_TAN;
        result.A = check_double(argv[2]);
        result.N = check_uint(argv[3]);
        result.M = check_uint(argv[4]);
    }
    else if((argc == 5 || argc == 6) && (strcmp(argv[1], "-c") ==  0)
    && (strcmp(argv[3], "-m") == 0)){
        result.state = S_DISTANCE;
        result.X = check_double(argv[2]);
        result.A = check_double(argv[4]);
        if(argc == 6){
            result.state = S_HEIGHT;
            result.B = check_double(argv[5]);
        }
    }
    else if((argc == 3 || argc == 4) && (strcmp(argv[1], "-m") == 0)){
        result.state = S_DISTANCE;
        result.A = check_double(argv[2]);
        if(argc == 4){
            result.state = S_HEIGHT;
            result.B = check_double(argv[3]);
        }
    }
    else{
        result.ecode = E_WRONG_PARAM;
    }
    check_params(&result);
    return result;
}

/**
 * Skontroluje, ci je mozne string previest na double
 * @param   str string, ktory chceme previest
 * @return  d   double hodnota stringu, ak sa neda previest INFINITY
 */
double check_double(char *str){
    char *err;
    double d = strtod(str, &err);
    if(*err|| isnan(d)){
        return INFINITY;
    }
    return d;
}

/**
 * Skontroluje, ci je mozne string previest na unsigned int
 * @param   str string, ktory chceme previest
 * @return  ui   double hodnota stringu, ak sa neda previest INFINITY
 */
unsigned int check_uint(char *str){
    char *err;
    unsigned int ui = strtoul(str, &err, 10);
    if(*err){
        return (unsigned int) INFINITY;
    }
    return ui;
}

/**
 * Skontroluje hodnoty parametrov, ak nesedia nastavi kod chyby
 * @param   params  struktura obsahujuca parametre
 * @return  void
 */
void check_params(params_t *params){
    if(params->state == S_HELP || params->ecode != E_SUCCESS){
        return;
    }
    else if(params->state == S_TAN){
        if(isinf(params->A)){
            params->ecode = E_WRONG_VALUE;
        }
        if(params->N <= 0 || params->N > MAX_ITERATIONS
        || params->M <= 0 || params->M > MAX_ITERATIONS
        || params->N > params->M){
    		params->ecode = E_WRONG_VALUE;
    	}
    }
    else if(params->state == S_DISTANCE){
        if(params->A <= 0 || params->A > MAX_ANGLE){
            params->ecode = E_WRONG_VALUE;
        }
    }
    else if(params->state == S_HEIGHT){
        if(params->A <= 0 || params->A > MAX_ANGLE){
            params->ecode = E_WRONG_VALUE;
        }
        if(params->B <= 0 || params->B > MAX_ANGLE){
    		    params->ecode = E_WRONG_VALUE;
    	  }
    }
    if(params->X <= 0 || params->X > MAX_HEIGHT){
    	params->ecode = E_WRONG_VALUE;
    }
}

/**
 * Vyrata absolutnu hodnotu cisla
 * @param   x   cislo, ktoreho absolutnu hodnotu chceme zistit
 * @return      absolutna hodnota x
 */
double my_fabs(double x){
    return x < 0 ? -x : x;
}

/**
 * Implementacia Taylorovho polynomu pre vypocet funkcie tangens
 * @param   x       uhol v radianoch
 * @param   n       pocet iteracii
 * @return  tang    tangens uhla x
 */
double taylor_tan(double x, unsigned int n){
    unsigned long long int k[] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582,443861162, 18888466084, 113927491862, 58870668456604};
    unsigned long long int j[] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
    double tang = 0;
    double x2 = x*x;
    for(unsigned int i = 0; i < n; i++){
        tang += (k[i] * x) / j[i];
        x *= x2;
    }
    return tang;
}

/**
 * Implementacia zretazenych zlomkov pre vypocet funkcie tangens
 * @param   x       uhol v radianoch
 * @param   n       pocet iteracii
 * @return  tang    tangens uhla x
 */
double cfrac_tan(double x, unsigned int n){
    double tang = n * 2 - 1;
    double x2 = x * x;
    for(unsigned int i = n-1; i > 0; i--){
        tang = (i * 2 - 1)  - x2 / tang;
    }
    return x / tang;
}

/**
 * Vypise porovnanie vypoctu tangens funkciou tan z matematickej kniznice
 * a mnou implementovanych funkcii taylor_tan a cfrac_tan s poctom iteracii
 * v rozsahu <N, M>
 * @param   A       uhol v radianoch
 * @param   N       dolna hranica iteracii
 * @param   M       horna hranica iteracii
 * @return  void
 */
void print_tan(double A, unsigned int N, unsigned int M){
    double math_tan = tan(A);
    for(unsigned int i = N; i <= M; i++){
        double t_tan = taylor_tan(A, i);
        double c_tan = cfrac_tan(A, i);
        double t_eps = my_fabs(math_tan - t_tan);
        double c_eps = my_fabs(math_tan - c_tan);
        printf("%d %e %e %e %e %e\n",
                i, math_tan, t_tan, t_eps, c_tan, c_eps);
    }
}

/**
 * Vyrata vzdialenost objektu od meracieho pristroja
 * @param   A   uhol v radianoch
 * @param   X   vyska pristroja
 * @return      vzdialenost objektu od meracieho pristroja
 */
double calc_distance(double A, double X){
    double tang = cfrac_tan(A, ENOUGH_ITERATIONS);
    return X / tang;
}

/**
 * Vyrata vysku meraneho objektu
 * @param   B           uhol v radianoch
 * @param   X           vyska pristroja
 * @param   distance    vzdialenost objektu od meracieho pristroja
 * @return              vyska meraneho ubjektu
 */
double calc_height(double B, double X, double distance){
    double tang = cfrac_tan(B, ENOUGH_ITERATIONS);
    return tang * distance + X;
}

int main(int argc, char *argv[]){
    double distance;
    double height;
    params_t params = get_params(argc, argv);

    if(params.ecode != E_SUCCESS){
        fprintf(stderr, "%s\n", ECODE_MSG[params.ecode]);
        return params.ecode;
    }
    if(params.state == S_HELP){
        help();
        return params.ecode;
    }

    switch(params.state){
        case S_HELP:
            help();
            break;
        case S_TAN:
            print_tan(params.A, params.N, params.M);
            break;
        case S_DISTANCE:
            distance = calc_distance(params.A, params.X);
            printf("%.10e\n", distance);
            break;
        case S_HEIGHT:
            distance = calc_distance(params.A, params.X);
            height = calc_height(params.B, params.X, distance);
            printf("%.10e\n", distance);
            printf("%.10e\n", height);
            break;
    }
    return EXIT_SUCCESS;
}
