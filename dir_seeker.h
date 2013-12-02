#ifndef DIR_SEEKER_H
#define DIR_SEEKER_H

#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <string>
#include "parser/parser.h"
#include <dirent.h>
#include <string.h>
#include <time.h>
#include "porter/porter.h"
#include "parser/vectorizador.h"


/*Crea un directorio con el nombre pasado por parametro*/
void create_dir(std::string name);

/*Borra un directorio con el nombre pasado por parametro*/
void remove_dir(std::string name);

/*Carga todo lo que se guarda en los hashes en disco*/
void guardar_frecuencias(map<string,unsigned int>* hash_frecuencias_globales,map<string,map<string,unsigned int> >* hash_frecuencias_locales);


/*Funcion que recorre un directorio e imprime los nombres de los archivos que se encuentran dentro de el*/
int parsear_archivos(string nombre_dir,Parser* parser);

#endif //DIR_SEEKER_H
