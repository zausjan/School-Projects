/**
 * @mainpage Projekt 3 - Jednoducha zhlukova analyza
 * @link
 * proj3.h
 * @endlink
 *
 * @file proj3.h
 * @brief Projekt 3 - Jednoducha shlukova analyza (Complete linkage)
 * @author Jan Zauska <xzausk00@stud.fit.vutbr.cz>
 * @date 10.12.2017
 * @version 1.0
 * @see http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 *.
 */

/** hodnota pre realokaciu zhluku */
extern const int CLUSTER_CHUNK;

/**
 * @struct obj_t
 * @brief Struktura reprezentujuca objekt
 */
struct obj_t {
    /** indentifikator */
    int id;
    /** suradnica x */
    float x;
    /** suradnica y */
    float y;
};

/**
 * @struct cluster_t
 * @brief Struktura reprezentujuca zhluk objektov
 */
struct cluster_t {
    /** pocet objektov */
    int size;
    /** kapacita */
    int capacity;
    /** pole objektov */
    struct obj_t *obj;
};

/**
 * @defgroup clusters Praca so zhlukmi
 * @{
 */

/**
 * Inicializacia zhluku 'c'. Alokuje pamet pre cap objektu (kapacitu).
 * Ukazatel NULL u pola objektov znamena kapacitu 0.
 *
 * @pre
 * zhluk 'c' != NULL
 *
 * @pre
 * kapacita 'c' je vacsia ako 0
 *
 * @post
 * kapacita sa rovna 'cap' ak nenastane chyba
 *
 * @post
 * size sa rovna 0
 *
 * @param c     ukazovatel na zhluk, ktory chceme inicializovat
 * @param cap   kapacita, ktoru chceme alokovat
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * Odstranenie vsetkych objektov zhluku a inicializovanie na prazdny zhluk.
 *
 * @pre
 * zhluk 'c' != NULL
 *
 * @post
 * kapacita sa rovna 'cap' ak nenastane chyba
 *
 * @post
 * size sa rovna 0
 *
 * @param c     ukazovatel na zhluk, ktory chceme vycistit
 * @param cap   kapacita zhluku c
 */
void clear_cluster(struct cluster_t *c);

/**
 * Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 *
 * @pre
 * kapacita zhluku >= 0
 *
 * @post
 * kapacita sa rovna  'new_cap' ak nenastane chyba
 *
 * @param   c         ukazovatel na zhluk, ktoreho kapacitu chceme zmenit
 * @param   new_cap   nova kapacita zhluku
 * @return  c         ukazovatel na zhluk c
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * Prida objekt 'obj' na koniec zhluku 'c'. Rozsiri zhluk, ak se do neho objekt
 * nevojde.
 *
 * @pre
 * zhluk != NULL
 *
 * @post
 * zhluk obsahuje objekt 'obj'
 *
 * @param   c   ukazovatel na zhluk, ktory chceme rozsirit
 * @param   obj objekt, ktory chceme pridat do zhluku.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * Do zhluku 'c1' prida objekty 'c2'. Zhluk 'c1' bude v pripade nutnosti rozsireny.
 * Objekty v zhluku 'c1' budu zoradene vzostupne podla identifikacneho cisla.
 * Zhluk 'c2' bude nezmenen.
 *
 * @pre
 * zhluk 'c1'!= NULL
 *
 * @pre
 * zhluk 'c2'!= NULL
 *
 * @post
 * zhluk 'c1'  obsahuje vsetky objekty zhluku 'c2'
 *
 * @param c1    ukazovatel na zhluk, ktory chceme rozsirit
 * @param c2    ukazovatel na zhluk, ktoreho objekty chceme pridat do 'c1'
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Odstrani zhluk z pola zhlukov 'carr'. Pole zhlukov obsahuje 'narr' poloziek
 * (zhlukuov). Zhluk pre odstranenie se nachadza na indexe 'idx'. Funkcia vracia
 * novy pocet shluku v poli.
 *
 * @pre
 * index je mensi ako narr
 *
 * @pre
 * narr je vacsi ako 0
 *
 * @post
 * pole zhlukov 'carr' neobsahuje prvok ktory mal index idx
 *
 * @param   carr  ukazovatel na prvy prvok pola struktur
 * @param   narr  pocet prvkov v poli
 * @param   idx   index prvku, ktory chceme zmazat z pola
 * @return  narr  novy pocet prvkov v poli
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * Zoradi objekty v zhluku  'c' vzostupne podla ich identifikacneho cisla.
 *
 * @post
 * zhluk 'c' je zoradeny
 *
 * @param c     ukazovatel na zhluk, ktory chceme zoradit
 */
void sort_cluster(struct cluster_t *c);

/**
 * Tisk zhluku 'c' na stdout.
 *
 * @pre
 * velkost zhluku je viac ako 0
 *
 * @param c ukazovatel na zhluk c
 */
void print_cluster(struct cluster_t *c);

/**
 * @}
 */

/**
* @defgroup cluster_array Praca s polom zhlukov
* @{
*/

/**
 * Zo suboru 'filename' nacita objekty. Pre kazdy objekt vytvori zhluk a ulozi
 * ho do pola zhlukov. Alokuje priestor pre pole vsetkych zhlukov a ukazatel
 * na prvu polozku pola (ukalazatel na prvy zhluk v alokovanom poli) ulozi
 * do pamati, kam odkazuje parameter 'arr'. Funkcia vracia pocet nacitanych
 * objektov (zhlukov). V pripade chyby uklada do pamati, kam odkazuje 'arr',
 * hodnotu NULL.
 *
 * @pre
 * pole 'arr' != NULL
 *
 * @post
 * pole 'arr' obsahuje zhluky nacitane zo souboru
 *
 * @param   filename    nazov suboru
 * @param   arr         pole zhlukov
 * @return  narr        pocet nacitanych zhlukov,
 *                      -1 ak nastane chyba
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 * hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 * 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 * adresu 'c1' resp. 'c2'.
 *
 * @pre
 * pocet zhlukov v poli je viac ako 0
 *
 * @param carr ukazovatel na prvy prvok pola struktur
 * @param narr pocet prvkov pola
 * @param c1 ukazovatel na int, kde ulozime index clustra 'c1'
 * @param c2 ukazovatel na int, kde ulozime index clustra 'c2'
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * Tisk pola zhlukov. Parametr 'carr' je ukazovatel na prvu polozku (zhluk).
 * Tiskne sa prvych 'narr' zhlukov.
 *
 * @param carr  pole zhlukov
 * @param narr  pocet prvkov pola
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @}
 */
 /**

 * @defgroup distance Pocitanie vzdialensti
 * @{
 */

/**
 * Pocita Euklidovsku vzdialenost medzi dvomi objektami.
 *
 * @pre
 * objekt 'o1' != NULL
 *
 * @pre
 * objekt 'o2' != NULL
 *
 * @param   o1    ukazovatel na prvy objekt
 * @param   o2    ukazovatel na druhy objekt
 * @return  dist  vzdialenost objektov 'o1' a 'o2'
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * Pocita vzdialenost dvoch zhlukou.
 *
 * @pre
 * zhluk 'c1' != NULL
 *
 * @pre
 * zhluk 'c2' != NULL
 *
 * @pre
 * velkost zhluku 'c1' je vacsia ako nula
 *
 * @pre
 * velkost zhluku 'c2' je vacsia ako nula
 *
 * @param   c1    ukazovatel na prvy zhluk
 * @param   c2    ukazovatel na druhy zhluk
 * @return  dist  vzdialenost zhlukov 'c1' a 'c2'
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @}
 */
 /**
