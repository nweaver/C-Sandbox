#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "stringexamples_c.h"

char *toupper_str(const char *s){
    char *ret = (char *) calloc(sizeof(char), strlen(s)+1);
    size_t i;
    for(i = 0; i <strlen(s); ++i) {
        ret[i] = toupper(s[i]);
    }
    return ret;
}