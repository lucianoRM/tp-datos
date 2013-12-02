#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include "indice.h"
#include <sstream>
#include <cstdlib>

# define PATH_INDICE "../Indices\ Archivos/"

using std::map;
using std::vector;
using std::string;
using std::ifstream;
using std::istringstream;
using std::ios;
using std::cout;
using std::endl;
using std::fstream;


void crearIndice(string nombre_dir)
{
    map < string,vector<string> > indice = map < string,vector<string> >();

    DIR* dir_pointer = opendir(nombre_dir.c_str());
	//if (dir_pointer == NULL)
	struct dirent* reg_buffer = readdir(dir_pointer);

	string path = "Indices\ Archivos/";
	string nombre_archivo;
	string nombre_directorio = nombre_dir;
	nombre_directorio+="/";
	string nombre_cluster;
	string cantidad_terminos;
	ifstream archivo;
	
	while(reg_buffer != NULL){
        nombre_archivo = (reg_buffer->d_name);
        
		if(nombre_archivo == "." || nombre_archivo == ".."){
			reg_buffer = readdir(dir_pointer);
			continue;
		}
		
		archivo.open((nombre_directorio + nombre_archivo).c_str());		
		
		string termino_actual;
		
        while(getline(archivo, termino_actual, ';')){

            indice[termino_actual].push_back(nombre_cluster);
            
            fstream archivo_indice;
			archivo_indice.open((path+termino_actual).c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
			archivo_indice << nombre_archivo << ";";
			archivo_indice.close();
		}
        
        //Levanto el ultimo texto
        /*string termino_actual;
        getline(archivo, termino_actual);
        indice[termino_actual].push_back(nombre_cluster);
		
		fstream archivo_indice;
		archivo_indice.open ((path+termino_actual).c_str(), std::fstream::in | std::fstream::out | std::fstream::app); 
		archivo_indice << nombre_cluster << ";";
		archivo_indice.close();
		*/
		archivo.close();
		reg_buffer = readdir(dir_pointer);
	}

	closedir(dir_pointer);
}

vector<string> obtenerClustersDeTermino(string termino){
	ifstream archivoIndices;
	archivoIndices.open(("../Indices\ Archivos/"+termino).c_str());
	string nombre_cluster;
	vector<string> clusters;
	while(getline(archivoIndices, nombre_cluster, ';'))
		clusters.push_back(nombre_cluster);
		
	return clusters;
}
