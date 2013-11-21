#ifndef PORTER_H
#define PORTER_H

#include <stdio.h>
#include <stdlib.h>      
#include <ctype.h> 


extern "C" {
   unsigned int stem(char * p, int i,unsigned int j);
}

#endif //PORTER_H
