#ifndef INDICE_H
#define INDICE_H

#include <map>
#include <fstream>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;


void agregar(string nombre_dir,string nombre);
    
void crearIndice(string nombre);
vector<string> obtenerClustersDeTermino(string termino);   


#endif // INDICE_H
