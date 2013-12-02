#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <string>
#include "parser.h"
#include <string.h>





using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;

Parser::Parser(){
	dicc_stopwords = Stopwords::getInstance()->getMap();
	cant_terminos = 0;
	cant_documentos = 0;

}

Parser::~Parser(){
	Stopwords::getInstance()->destroy();
}


/*Verifica si el caracter es una letra o no. Si no lo es devuelve 0*/
short is_letter(char character){
	if((int)character > 64 && (int)character < 123){
		if((int)character < 91 || (int)character > 96)
			return 1;
	}
	return 0;
}

void Parser::incrementarDocs(){
	cant_documentos++;
}
 
void Parser::agregar_termino_a_hash(string termino,string nombre_archivo){
	
	unsigned int size = termino.size();

	if(dicc_stopwords[termino] == 1) return; //No entra porque es un stopword
	
	unsigned int ultimo_char = stem((char*)termino.c_str(),0,size-1); 
	
	termino = termino.substr(0,ultimo_char+1);
	

	if(!hash_frecuencias_locales[nombre_archivo][termino]){
		hash_frecuencias_locales[nombre_archivo][termino] = 1;
		if(hash_frecuencias_globales[termino]){
			hash_apariciones_unicas.erase(termino);
			hash_frecuencias_globales[termino]++;
		}
		else{
			cant_terminos++;
			hash_frecuencias_globales[termino] = 1;
			hash_apariciones_unicas[termino] = nombre_archivo;
		}
	}else
		hash_frecuencias_locales[nombre_archivo][termino]++;
	
}
	


/*Recibe una linea(string), la parsea y carga los terminos a los hashes.*/
void Parser::cargar_terminos(std::string linea,std::string nombre_archivo){
	unsigned int pos_actual;
	unsigned int linea_size = linea.size();
	std::string termino = "";
	char letra_actual;
	for(pos_actual = 0;pos_actual < linea_size;pos_actual++){
		letra_actual = linea[pos_actual];//para optimizar la lectura, lee el caracter una vez, nada mas.
		if(is_letter(letra_actual) == 0){ 
			if(termino.size() > 3)
				agregar_termino_a_hash(termino,nombre_archivo);
			termino = "";		
		}else{
			letra_actual = tolower(letra_actual);
			termino += letra_actual;	
		}
	}
}	

unsigned int Parser::getCantTerms(){
	return cant_terminos;
}

unsigned int Parser::getCantDocs(){
	return cant_documentos;
}

void Parser::setFrecuenciasGlobales(map<string,float>  frecuencias){
	hash_frecuencias_globales = frecuencias;
	cant_terminos = frecuencias.size();
}
	
void Parser::agregar_archivo(map<string,unsigned int> frecuencias_locales,string nombre){
	hash_frecuencias_locales[nombre] = frecuencias_locales;
}

map<string,float>* Parser::getFrecuenciasGlobales(){
	return &hash_frecuencias_globales;
}


map<string,map<string,unsigned int> >* Parser::getFrecuenciasLocales(){
	return &hash_frecuencias_locales;
}


/*Borra de los terminos los que solo aprecen 1 vez entre todos los documentos*/
void Parser::filtrarAparicionesUnicas(){
	
	map<string,string>::iterator it_hash_apariciones_unicas;
	
	for(it_hash_apariciones_unicas = hash_apariciones_unicas.begin();it_hash_apariciones_unicas != hash_apariciones_unicas.end();++it_hash_apariciones_unicas){
		hash_frecuencias_locales[it_hash_apariciones_unicas->second].erase(it_hash_apariciones_unicas->first);
		hash_frecuencias_globales.erase(it_hash_apariciones_unicas->first);
		cant_terminos--;
	}
}
