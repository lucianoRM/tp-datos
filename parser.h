#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "stopwords/stopwords.h"
#include "porter/porter.h"


using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;





/*Filtra el termino pasados por parametro y luego los agrega a los diccionarios*/

void agregar_termino_a_hash(string termino, map<string,unsigned int> *hash_frecuencias_globales, map<string,unsigned int> *hash_frecuencias_locales,map<string,short>* hash_stopwords);
	


/*Recibe una linea(string), la parsea y carga los terminos a los hashes.*/

void cargar_terminos(string linea, map<string,unsigned int> *hash_frecuencias_globales, map<string,unsigned int> *hash_frecuencias_locales, map<string,short>* hash_stopwords,int* cant_terminos,map<string,string>* hash_apariciones_unicas,string nombre_archivo);

#endif //PARSER_H
