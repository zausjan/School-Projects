// ppm.h
// Řešení IJC-DU1, příklad b), 20.3.2018
// Autor: Ján Zauška, FIT VUT
// Přeloženo: gcc 7.2.1

#ifndef _PPM_H
#define _PPM_H

#define MAX_X 1000
#define MAX_Y 1000

struct ppm {
   unsigned xsize;
   unsigned ysize;
   char data[3*MAX_X*MAX_Y];    // RGB bajty, celkem 3*xsize*ysize
};

struct ppm *ppm_read(const char *filename);
int ppm_write(struct ppm *p, const char * filename);
#endif /*_PPM_H*/
