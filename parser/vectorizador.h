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
void guardar_pesos_globales(std::map<std::string,float> pesos_globales);
std::map<std::string,float> recuperar_pesos_globales();
map<string,map<unsigned int,float> >* vectorizar(Parser* parser);
map<string, map<unsigned int, float> >* obtener_muestra_vectores(map<string, map<unsigned int, float> >* vectores);

#endif //VECTORIZADOR_H
