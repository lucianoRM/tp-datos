#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "stopwords/stopwords.h"
#include "stopwords/delimiters.h"

using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;





/*Filtra el termino pasados por parametro y luego los agrega a los diccionarios*/

void agregar_termino_a_hash(string termino, map<string,unsigned int> *hash);
	


/*Recibe una linea(string), la parsea y carga los terminos a los hashes.*/

void cargar_terminos(string linea,map<string,unsigned int> *hash);

#endif //PARSER_H
