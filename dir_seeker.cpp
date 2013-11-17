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
int* parsear_archivos(const char* nombre_dir){
	
	DIR* dir_pointer = opendir(nombre_dir);
	struct dirent* reg_buffer = readdir(dir_pointer);
	string nombre_archivo;
	string nombre_directorio = nombre_dir;
	nombre_directorio += "/";
	string linea;
	ifstream archivo;
	map<string,map<string,unsigned int> > documentos;
	map<string,unsigned int> hash_frecuencias_globales;
	map<string,unsigned int> hash_frecuencias_locales;	
	map<string,short> hash_stopwords = Stopwords::getInstance()->getMap();
	int cant_archivos = 0;
	int cant_terminos = 0;
	map<string,unsigned int>::iterator it_local;
	map<string,map<string,unsigned int> >::iterator it_documentos;
	while(reg_buffer != NULL){
		nombre_archivo = (reg_buffer->d_name);
		cout << "Parsing: " << nombre_directorio + nombre_archivo << endl;
		archivo.open((nombre_directorio + nombre_archivo).c_str());
		while(getline(archivo,linea)){
			linea += '\n'; //Lo agrego para que no se coma los terminos del final.
			cargar_terminos(linea,&hash_frecuencias_globales,&documentos[nombre_archivo],&hash_stopwords,&cant_terminos);
		}
		archivo.close();
		reg_buffer = readdir(dir_pointer);
		cant_archivos++;
	}
	
	
	for(it_documentos = documentos.begin();it_documentos != documentos.end(); ++it_documentos){
		nombre_archivo = "terminos_locales/" + it_documentos->first;
		cout << "Writing: " << it_documentos->first << endl;
		ofstream terminos_locales(nombre_archivo.c_str());
		for(it_local = it_documentos->second.begin(); it_local !=it_documentos->second.end(); ++it_local)
			terminos_locales << it_local->first << ": " << it_local->second << endl;
		terminos_locales.close();
	}
	closedir(dir_pointer);
	ofstream terminos_globales("archivo_terminos_globales.txt");
	map<string,unsigned int>::iterator it;
	for(it = hash_frecuencias_globales.begin(); it != hash_frecuencias_globales.end(); ++it)
		terminos_globales << it->first << ": " << it->second << endl;
	terminos_globales.close();
	int* cantidades = (int*) malloc ( sizeof(int) * 2);
	cantidades[0] = cant_archivos;
	cantidades[1] = cant_terminos;
	return cantidades;
}

int main(){
	int t_inicio = time(NULL);
	int* cantidades = parsear_archivos("Libros");
	cout << "Cantidad de archivos: "<< cantidades[0] << endl;
	cout << "Cantidad de terminos: "<< cantidades[1] << endl;
	int t_fin = time(NULL);
	cout << "Tardo: " << t_fin - t_inicio << " segundos";
	free(cantidades);
	return 0;
}



