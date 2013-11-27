#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include "indice.h"
#include <sstream>

using std::map;
using std::vector;
using std::string;
using std::ifstream;
using std::istringstream;
using std::cout;
using std::endl;

Indice::Indice(string nombre_dir)
{
    indice = map < string,vector<string> >();

    DIR* dir_pointer = opendir(nombre_dir.c_str());
	//if (dir_pointer == NULL)
	struct dirent* reg_buffer = readdir(dir_pointer);

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
		
		nombre_cluster = nombre_archivo.substr(0, nombre_archivo.length()- 4);
		archivo.open((nombre_directorio + nombre_archivo).c_str());
		
		getline(archivo, cantidad_terminos, ';');
        for(int i = 0; i < atoi( cantidad_terminos.c_str() ) - 1; i++){
			string termino_actual;
            getline(archivo, termino_actual, ';');

            indice[termino_actual].push_back(nombre_cluster);
        }
        
        //Levanto el ultimo texto
        string termino_actual;
        getline(archivo, termino_actual);
        indice[termino_actual].push_back(nombre_cluster);

		archivo.close();
		reg_buffer = readdir(dir_pointer);
	}

	closedir(dir_pointer);
}

vector<string> Indice::obtenerClustersDeTermino(string termino){
		return indice[termino];
}

Indice::~Indice()
{
    //dtor
}
