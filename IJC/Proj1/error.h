// error.h
// Řešení IJC-DU1, příklad b), 20.3.2018
// Autor: Ján Zauška, FIT VUT
// Přeloženo: gcc 7.2.1

#ifndef _ERROR_H
#define _ERROR_H

#include<stdarg.h>
#include<stdlib.h>


void warning_msg(const char *fmt, ...);

void error_exit(const char *fmt, ...);

#endif /* _ERROR_H */
