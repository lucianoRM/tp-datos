#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <string>
#include "parser.h"
#include <dirent.h>
#include <string.h>
#include <time.h>
#include "porter/porter.h"


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



/*Borra de los terminos los que solo aprecen 1 vez entre todos los documentos*/
void filtrar_apariciones_unicas(map<string,unsigned int>* hash_frecuencias_globales,map<string,map<string,unsigned int> >* hash_frecuencias_locales,map<string,string>* hash_apariciones_unicas,int* cant_terminos){
	
	map<string,string>::iterator it_hash_apariciones_unicas;
	
	for(it_hash_apariciones_unicas = hash_apariciones_unicas->begin();it_hash_apariciones_unicas != hash_apariciones_unicas->end();++it_hash_apariciones_unicas){
		(*hash_frecuencias_locales)[it_hash_apariciones_unicas->second].erase(it_hash_apariciones_unicas->first);
		(*hash_frecuencias_globales).erase(it_hash_apariciones_unicas->first);
		(*cant_terminos)--;
	}
}


/*Funcion que recorre un directorio e imprime los nombres de los archivos que se encuentran dentro de el*/
int* parsear_archivos(const char* nombre_dir){
	
	DIR* dir_pointer = opendir(nombre_dir);
	struct dirent* reg_buffer = readdir(dir_pointer);
	string nombre_archivo;
	string nombre_directorio = nombre_dir;
	nombre_directorio += "/";
	string linea;
	ifstream archivo;
	map<string,string> hash_apariciones_unicas; //Necesario para filtrar los terminos que aparecen solo en 1 documento.
	map<string,map<string,unsigned int> > hash_frecuencias_locales; //Cada uno es un hash con clave: nombre_documento y dato: hash con frecuencias locales.
	map<string,unsigned int> hash_frecuencias_globales;
	map<string,short> hash_stopwords = Stopwords::getInstance()->getMap();
	int cant_archivos = 0;
	int cant_terminos = 0;
	
	while(reg_buffer != NULL){
		nombre_archivo = (reg_buffer->d_name);
		cout << "Parsing: " << nombre_directorio + nombre_archivo << endl;
		archivo.open((nombre_directorio + nombre_archivo).c_str());
		while(getline(archivo,linea)){
			linea += '\n'; //Lo agrego para que no se coma los terminos del final.
			cargar_terminos(linea,&hash_frecuencias_globales,&hash_frecuencias_locales[nombre_archivo],&hash_stopwords,&cant_terminos,&hash_apariciones_unicas,nombre_archivo);
		}
		archivo.close();
		reg_buffer = readdir(dir_pointer);
		cant_archivos++;
	}
	
	filtrar_apariciones_unicas(&hash_frecuencias_globales,&hash_frecuencias_locales,&hash_apariciones_unicas,&cant_terminos);
	guardar_terminos(&hash_frecuencias_globales,&hash_frecuencias_locales);
	
	closedir(dir_pointer);
	
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



