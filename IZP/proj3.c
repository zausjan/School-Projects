/**
 * Subor:   proj3.c
 * Datum:   10.12.2017
 * Autor:   1BIT - Jan Zauska, xzausk00@fit.vutbr.cz
 * Projekt: 1BIT Projekt 3: Jednoducha shlukova analyza
 * Popis: https://wis.fit.vutbr.cz/FIT/st/cwk.php.cs?title=IZP:Projekt3
 *        &csid=652975&id=12214
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h> // strcmp

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

enum{
    M_AVG,
    M_MIN,
    M_MAX
};

int premium_case = M_AVG;
/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

 /**
  * Inicializacia zhluku 'c'. Alokuje pamet pre cap objektu (kapacitu).
  * Ukazatel NULL u pola objektov znamena kapacitu 0.
  * @param c     ukazovatel na zhluk, ktory chceme inicializovat
  * @param cap   kapacita, ktoru chceme alokovat
 */
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->obj = malloc(sizeof(struct obj_t) * cap);
    if(c->obj)
        c->capacity = cap;
    else
        c->capacity = 0;
    c->size = 0;
}

/**
 * Odstranenie vsetkych objektov zhluku a inicializovanie na prazdny zhluk.
 * @param c     ukazovatel na zhluk, ktory chceme vycistit
 * @param cap   kapacita zhluku c
 */
void clear_cluster(struct cluster_t *c)
{
    assert(c);
    free(c->obj);
    init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/**
 * Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 * @param   c         ukazovatel na zhluk, ktoreho kapacitu chceme zmenit
 * @param   new_cap   nova kapacita zhluku
 * @return  c         ukazovatel na zhluk c
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 * Prida objekt 'obj' na koneic zhluku 'c'. Rozsiri zhluk, ak se do neho objekt
 * nevojde.
 * @param   c   ukazovatel na zhluk, ktory chceme rozsirit
 * @param   obj objekt, ktory chceme pridat do zhluku.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    assert(c);
    if(c->capacity  <= c->size)
    {
        c = resize_cluster(c, c->size + CLUSTER_CHUNK);
    }
    c->obj[c->size] = obj;
    c->size++;
}

/*
 * Zoradi objekty v zhluku  'c' vzostupne podla ich identifikacneho cisla.
 * @param c     ukazovatel na zhluk, ktory chceme zoradit
 */
void sort_cluster(struct cluster_t *c);

/*
 * Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 * Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 * Shluk 'c2' bude nezmenen.
 * @param c1    ukazovatel na zhluk, ktory chceme rozsirit
 * @param c2    ukazovatel na zhluk, ktoreho objekty chceme pridat do 'c1'
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);
    int size_needed = c1->size + c2-> size;
    if(c1->capacity < size_needed)
        c1 = resize_cluster(c1, size_needed);

    for(int i = 0; i < c2->size; i++)
    {
        append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 * Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 * (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 * pocet shluku v poli.
 * @param   carr  ukazovatel na prvy prvok pola struktur
 * @param   narr  pocet prvkov v poli
 * @param   idx   index prvku, ktory chceme zmazat z pola
 * @return  narr  novy pocet prvkov v poli
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    free(carr[idx].obj);
    carr[idx] = carr[narr - 1];
    return narr - 1;
}

/*
 Uvolni pamat na konci programu
 Parameter 'carr' je ukazovatel na prvu polozku (zhluk),
 ktory chceme uvolnit.
 Parameter narr je pocet zhlukov.
 */
void free_all(struct cluster_t * carr, int narr){
    while(narr > 0)
    {
        narr = remove_cluster(carr, narr, narr - 1);
    }
    free(carr);
    carr = NULL;
}

/*
 * Pocita Euklidovsku vzdialenost medzi dvomi objektami.
 * @param   o1    ukazovatel na prvy objekt
 * @param   o2    ukazovatel na druhy objekt
 * @return  dist  vzdialenost objektov 'o1' a 'o2'
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float x, y;
    x = o1->x - o2->x;
    y = o1->y - o2->y;
    return sqrtf(x*x + y*y);
}

/*
 * Pocita vzdialenost dvoch zhlukou.
 * @param   c1    ukazovatel na prvy zhluk
 * @param   c2    ukazovatel na druhy zhluk
 * @return  dist  vzdialenost zhlukov 'c1' a 'c2'
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float distance_min = INFINITY;
    float distance_max = 0;
    float distance_sum = 0;

    for(int i = 0; i < c1->size; i++)
    {
        for(int j = 0; j < c2->size; j++)
        {
            float temp  = obj_distance(&c1->obj[i], &c2->obj[j]);
            distance_sum += temp;
            distance_max = fmax(temp, distance_max);
            distance_min = fmin(temp, distance_min);
        }
    }

    if(premium_case == M_MIN)
        return distance_min;
    else if(premium_case == M_MAX)
        return distance_max;
    else
        return distance_sum / (c1->size * c2->size);
}

/*
 * Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 * hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 * 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 * adresu 'c1' resp. 'c2'.
 * @param carr ukazovatel na prvy prvok pola struktur
 * @param narr pocet prvkov pola
 * @param c1 ukazovatel na int, kde ulozime index clustra 'c1'
 * @param c2 ukazovatel na int, kde ulozime index clustra 'c2'
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    float min = INFINITY;
    for(int i = 0; i < narr; i++)
    {
        for(int j = i+1; j < narr; j++)
        {
            float temp = cluster_distance(&carr[i], &carr[j]);
            if(temp < min)
            {
                *c1 = i;
                *c2 = j;
                min = temp;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

static int cluster_sort_compar(const void *a, const void *b)
{
    const struct cluster_t *c1 = (const struct cluster_t *)a;
    const struct cluster_t *c2 = (const struct cluster_t *)b;
    if (c1->obj[0].id < c2->obj[0].id) return -1;
    if (c1->obj[0].id > c2->obj[0].id) return 1;
    return 0;
}

void sort_clusters(struct cluster_t *c, int N){
    qsort(c, N, sizeof(struct cluster_t), &cluster_sort_compar);
}

/**
 * Tisk zhluku 'c' na stdout.
 * @param c ukazovatel na zhluk c
 */
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    assert(c->size);
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 * Zo suboru 'filename' nacita objekty. Pre kazdy objekt vytvori zhluk a ulozi
 * ho do pola zhlukov. Alokuje priestor pre pole vsetkych zhlukov a ukazatel
 * na prvu polozku pola (ukalazatel na prvy zhluk v alokovanom poli) ulozi
 * do pamati, kam odkazuje parameter 'arr'. Funkcia vracia pocet nacitanych
 * objektov (zhlukov). V pripade chyby uklada do pamati, kam odkazuje 'arr',
 * hodnotu NULL.
 * @param   filename    nazov suboru
 * @param   arr         pole zhlukov
 * @return  narr        pocet nacitanych zhlukov,
 *                      -1 ak nastane chyba
 */
int load_clusters(char *filename, struct cluster_t **arr)
{
	assert(arr != NULL);
	FILE *file;
    file = fopen(filename, "r");
    if(file == NULL)
    {
        fprintf(stderr, "Error reading the file.\n");
        return -1;
    }

    int narr;
	if(fscanf(file, "count=%d", &narr) != 1 || narr < 1)
    {
        fprintf(stderr, "Count not found at first line.\n");
        fclose(file);
        return -1;
    }

	*arr = malloc(narr * sizeof(struct cluster_t));
    if(*arr == NULL)
    {
        fprintf(stderr, "Error while allocating space.\n");
        fclose(file);
        return -1;
    }

    int i = 0;
    struct obj_t obj;
	while (fscanf(file ,"%d %f %f", &obj.id, &obj.x, &obj.y)==3)
    {
        if(obj.x < 0 || obj.x > 1000 || obj.y < 0 || obj.y > 1000) break;
		init_cluster(&(*arr)[i], 1);
        append_cluster(&(*arr)[i], obj);
        i++;
	}

	if(fclose(file) == EOF){
        fprintf(stderr, "Couldn't close file.\n");
        free_all(*arr, i);
        return -1;
    }
    if(i != narr)
    {
        fprintf(stderr, "Number of clusters doesn't match count.\n");
        free_all(*arr, i);
        return -1;
    }

	return narr;
}

/*
 * Tisk pola zhlukov. Parametr 'carr' je ukazovatel na prvu polozku (zhluk).
 * Tiskne sa prvych 'narr' zhlukov.
 * @param carr  pole zhlukov
 * @param narr  pocet prvkov pola
 */
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

/*
 Spaja zhluky, pokial ich pocet je vacsi ako N.
 Parameter 'carr' je ukazovatel na prvu polozku (zhluk).
 Paramater narr je pocet zhlukov na zaciatku, N pocet zhlukov ktory chceme
 */
void reduce_clusters(struct cluster_t *carr, int *narr, int N){
    while(*narr > N)
    {
        int c1, c2;
        find_neighbours(carr, *narr , &c1, &c2);
        merge_clusters(carr + c1, carr + c2);
        *narr = remove_cluster(carr, *narr, c2);
    }
    sort_clusters(carr, N);
}

int parse_args(int argc, char *argv[], int *N){
    if(argc < 2 || argc > 4)
    {
        return -1;
    }
    if(argc > 2)
    {
        *N = strtoul(argv[2], NULL, 10);
    }
    if(*N < 1)
    {
        fprintf(stderr, "Number of clusters out of range.\n");
        return -1;
    }

    if(argc == 4){
        if(strcmp(argv[3], "--avg") == 0){
            premium_case = M_AVG;
        }
        else if(strcmp(argv[3], "--min") == 0){
            premium_case = M_MIN;
        }
        else if(strcmp(argv[3], "--max") == 0){
            premium_case = M_MAX;
        }
        else{
            return -1;
        }
    }
    return 0;
}
int main(int argc, char *argv[])
{
    int N = 1;
    int err = parse_args(argc, argv, &N);
    if(err){
        fprintf(stderr, "Wrong use of params.\n");
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    struct cluster_t *clusters;
    int narr = load_clusters(filename, &clusters);

    if(narr == -1 || narr < N)
    {
        fprintf(stderr, "Error occured while loading clusters.\n");
        free_all(clusters, narr);
        return EXIT_FAILURE;
    }
    reduce_clusters(clusters, &narr, N);
    print_clusters(clusters, narr);
    free_all(clusters, narr);

    return EXIT_SUCCESS;
}
