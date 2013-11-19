#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "stopwords/stopwords.h"
#include "parser.h"
#include "porter/porter.h"

using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;



/*Verifica si el caracter es una letra o no. Si no lo es devuelve 0*/
short is_letter(char character){
	if((int)character > 64 && (int)character < 123){
		if((int)character < 91 || (int)character > 96)
			return 1;
	}
	return 0;
}


 
void agregar_termino_a_hash(string termino, map<string,unsigned int> *hash_frecuencias_globales,map<string,unsigned int> *hash_frecuencias_locales,map<string,short>* hash_stopwords,int* cant_terminos,map<string,string>* hash_apariciones_unicas,string nombre_archivo){
	
	unsigned int size = termino.size();

	if((*hash_stopwords)[termino] == 1) return; //No entra porque es un stopword
	
	unsigned int ultimo_char = stem((char*)termino.c_str(),0,size-1); 
	
	termino = termino.substr(0,ultimo_char+1);
	

	if(!((*hash_frecuencias_locales)[termino])){
		(*hash_frecuencias_locales)[termino] = 1;
		if((*hash_frecuencias_globales)[termino]){
			(*hash_apariciones_unicas).erase(termino);
			(*hash_frecuencias_globales)[termino]++;
		}
		else{
			(*cant_terminos)++;
			(*hash_frecuencias_globales)[termino] = 1;
			(*hash_apariciones_unicas)[termino] = nombre_archivo;
		}
	}else
		(*hash_frecuencias_locales)[termino]++;
	
}
	


/*Recibe una linea(string), la parsea y carga los terminos a los hashes.*/
void cargar_terminos(string linea, map<string,unsigned int> *hash_frecuencias_globales, map<string,unsigned int> *hash_frecuencias_locales, map<string,short>* hash_stopwords,int* cant_terminos,map<string,string>* hash_apariciones_unicas,string nombre_archivo){
	unsigned int pos_actual;
	unsigned int linea_size = linea.size();
	string termino = "";
	char letra_actual;
	for(pos_actual = 0;pos_actual < linea_size;pos_actual++){
		letra_actual = linea[pos_actual];//para optimizar la lectura, lee el caracter una vez, nada mas.
		if(is_letter(letra_actual) == 0){ 
			if(termino.size() > 3)
				agregar_termino_a_hash(termino,hash_frecuencias_globales,hash_frecuencias_locales,hash_stopwords,cant_terminos,hash_apariciones_unicas,nombre_archivo);
			termino = "";		
		}else{
			letra_actual = tolower(letra_actual);
			termino += letra_actual;	
		}
	}
	
}






