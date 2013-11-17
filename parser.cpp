#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "stopwords/stopwords.h"
#include "parser.h"

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



void agregar_termino_a_hash(string termino, map<string,unsigned int> *hash_frecuencias_globales,map<string,unsigned int> *hash_frecuencias_locales,map<string,short>* hash_stopwords,int* cant_terminos){
	

	if((*hash_stopwords)[termino] == 1) return; //No entra porque es un stopword
	unsigned int frecuencia_termino_globales = (*hash_frecuencias_globales)[termino];
	unsigned int frecuencia_termino_locales = (*hash_frecuencias_locales)[termino];
	if(frecuencia_termino_locales){
		(*hash_frecuencias_globales)[termino]++;
		(*hash_frecuencias_locales)[termino]++;
	}
	else if(frecuencia_termino_globales){
		(*hash_frecuencias_globales)[termino]++;
		(*hash_frecuencias_locales)[termino] = 1;
	}
	else{
		(*cant_terminos)++;
		(*hash_frecuencias_globales)[termino] = 1;
		(*hash_frecuencias_locales)[termino] = 1;
	}
}


/*Recibe una linea(string), la parsea y carga los terminos a los hashes.*/
void cargar_terminos(string linea, map<string,unsigned int> *hash_frecuencias_globales, map<string,unsigned int> *hash_frecuencias_locales, map<string,short>* hash_stopwords,int* cant_terminos){
	unsigned int pos_actual;
	unsigned int linea_size = linea.size();
	string termino = "";
	char letra_actual;
	for(pos_actual = 0;pos_actual < linea_size;pos_actual++){
		letra_actual = linea[pos_actual];//para optimizar la lectura, lee el caracter una vez, nada mas.
		if(is_letter(letra_actual) == 0){ 
			if(termino.size() > 1)
				agregar_termino_a_hash(termino,hash_frecuencias_globales,hash_frecuencias_locales,hash_stopwords,cant_terminos);
			termino = "";		
		}else{
			letra_actual = tolower(letra_actual);
			termino += letra_actual;	
		}
	}
	
}





