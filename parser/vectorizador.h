#ifndef VECTORIZADOR_H
#define VECTORIZADOR_H

#include <iostream>
#include <map>
#include <string>
#include <math.h> 
#include "parser.h"


using std::map;
using std::string;
using std::cout;
using std::endl;



/*Toma un diccionario de frecuencias globales y lo transforma en uno de pesos globales*/
map<string,map<unsigned int,float> >* vectorizar(Parser* parser);

#endif //VECTORIZADOR_H
