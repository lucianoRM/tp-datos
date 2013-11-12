#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <string>
#include "parser.h"
#include <dirent.h>
#include <string.h>
#include <time.h>

using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;




		

/*Funcion que recorre un directorio e imprime los nombres de los archivos que se encuentran dentro de el*/
void parsear_archivos(const char* nombre_dir){
	
	DIR* dir_pointer = opendir(nombre_dir);
	struct dirent* reg_buffer = readdir(dir_pointer);
	string nombre_archivo;
	string nombre_directorio = nombre_dir;
	nombre_directorio += "/";
	string linea;
	ifstream archivo;
	map<string,unsigned int> hash_frecuencias;	
	map<string,short> hash_stopwords = Stopwords::getInstance()->getMap();
	
	while(reg_buffer != NULL){
		nombre_archivo = (reg_buffer->d_name);
		nombre_archivo = nombre_directorio + nombre_archivo;
		cout << "Parsing: " << nombre_archivo << endl;
		archivo.open(nombre_archivo.c_str());
		while(getline(archivo,linea)){
			linea += '\n'; //Lo agrego para que no se coma los terminos del final.
			cargar_terminos(linea,&hash_frecuencias,&hash_stopwords);
		}
		archivo.close();
		reg_buffer = readdir(dir_pointer);
	}
	closedir(dir_pointer);
	ofstream terminos  ("archivo_terminos.txt");
	map<string,unsigned int>::iterator it;
	for(it = hash_frecuencias.begin(); it != hash_frecuencias.end(); ++it)
		terminos << it->first << ": " << it->second << endl;
	
	terminos.close();
}

int main(){
	int t_inicio = time(NULL);
	parsear_archivos("Libros");
	int t_fin = time(NULL);
	cout << "Tardo: " << t_fin - t_inicio << " segundos";
	return 0;
}



