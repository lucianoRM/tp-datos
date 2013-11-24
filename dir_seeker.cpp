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
#include "dir_seeker.h"


using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;



/*Carga todo lo que se guarda en los hashes en disco*/
void guardar_terminos(map<string,unsigned int>* hash_frecuencias_globales,map<string,map<string,unsigned int> >* hash_frecuencias_locales){

	string nombre_archivo;	
	
	map<string,unsigned int>::iterator it_hash_frecuencias_globales;
	map<string,map<string,unsigned int> >::iterator	it_hash_frecuencias_locales;
	map<string,unsigned int>::iterator it_local;

	

	

	for(it_hash_frecuencias_locales = hash_frecuencias_locales->begin();it_hash_frecuencias_locales != hash_frecuencias_locales->end(); ++it_hash_frecuencias_locales){
		nombre_archivo = "terminos_locales/" + it_hash_frecuencias_locales->first;
		cout << "Writing: " << it_hash_frecuencias_locales->first << endl;
		ofstream terminos_locales(nombre_archivo.c_str());
		
		for(it_local = it_hash_frecuencias_locales->second.begin(); it_local != it_hash_frecuencias_locales->second.end(); ++it_local)
			terminos_locales << it_local->first << ": " << it_local->second << endl;
		terminos_locales.close();
	}

	ofstream terminos_globales("archivo_terminos_globales.txt");

	for(it_hash_frecuencias_globales = hash_frecuencias_globales->begin(); it_hash_frecuencias_globales!= hash_frecuencias_globales->end(); ++it_hash_frecuencias_globales)
		terminos_globales << it_hash_frecuencias_globales->first << ": " << it_hash_frecuencias_globales->second << endl;
	terminos_globales.close();
	
}





/*Funcion que recorre un directorio e imprime los nombres de los archivos que se encuentran dentro de el*/
int parsear_archivos(string nombre_dir,Parser* parser){
	
	DIR* dir_pointer = opendir(nombre_dir.c_str());
	if (dir_pointer == NULL) return 1;
	struct dirent* reg_buffer = readdir(dir_pointer);
	string nombre_archivo;
	string nombre_directorio = nombre_dir;
	nombre_directorio += "/";
	string linea;
	ifstream archivo;
	
	while(reg_buffer != NULL){
		nombre_archivo = (reg_buffer->d_name);
		if(nombre_archivo == "." || nombre_archivo == ".."){
			reg_buffer = readdir(dir_pointer);
			continue;
		}
		cout << "Parsing: " << nombre_directorio + nombre_archivo << endl;
		archivo.open((nombre_directorio + nombre_archivo).c_str());
		while(getline(archivo,linea)){
			linea += '\n'; //Lo agrego para que no se coma los terminos del final.
			parser->cargar_terminos(linea,nombre_archivo);
		}
		archivo.close();
		reg_buffer = readdir(dir_pointer);
		parser->incrementarDocs();
	}
	
	parser->filtrarAparicionesUnicas();
		

	closedir(dir_pointer);
	return 0;
}



