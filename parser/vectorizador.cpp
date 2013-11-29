#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <math.h> 
#include "vectorizador.h"
#include "parser.h"

using std::map;
using std::string;
using std::cout;
using std::endl;
using std::ofstream;

#define MAX 100
#define PATH "vectores/"

void cargar_a_disco(map<string,map<unsigned int,float> >* map_vectores){
	map<string,map<unsigned int,float> >::iterator it_docs;
	string nombre_archivo = PATH;
	map<unsigned int,float>::iterator it_terms;
	for(it_docs = map_vectores->begin(); it_docs != map_vectores->end(); ++it_docs){
		nombre_archivo += it_docs->first;
		ofstream archivo(nombre_archivo.c_str());
		for (it_terms = it_docs->second.begin(); it_terms != it_docs->second.end(); ++it_terms)
			archivo << it_terms->first << ":" << it_terms->second << endl;
		archivo.close();
		nombre_archivo = PATH;
	}
}
		
		
	



map<string,map<unsigned int,float> >* vectorizar(Parser* parser){

	unsigned int archivos_procesados = 0;
	map<string,unsigned int> hash_referencias; //Guarda las referencias de los terminos a los int que los representan
	map<string,map<unsigned int,float> >* vectores = new map<string,map<unsigned int,float> > ; //Vectores que representan a los archivos.
	map<string,float> pesos_globales; //Se guardan los pesos globales calculados;
	map<string,unsigned int>::iterator it_global;
	map<string,map<string,unsigned int> >::iterator it_local_ext;
	map<string,unsigned int>::iterator it_local_int;
	unsigned int i = 0;
	float peso;
	float cant_docs = (float)parser->getCantDocs();
	float division;
	for(it_global = parser->getFrecuenciasGlobales()->begin(); it_global !=  parser->getFrecuenciasGlobales()->end(); ++it_global){
		division = (cant_docs/(float)it_global->second);
		hash_referencias[it_global->first] = i;
		pesos_globales[it_global->first] = 1 + log10(division);
		i++;
	}
	
	for(it_local_ext = parser->getFrecuenciasLocales()->begin(); it_local_ext != parser->getFrecuenciasLocales()->end(); ++it_local_ext){
		cout << "Vectorizando: " << it_local_ext->first << endl;		
		for(it_local_int = it_local_ext->second.begin();it_local_int != it_local_ext->second.end();++it_local_int){
			peso = ((float)it_local_int->second) * pesos_globales[it_local_int->first];
			(*vectores)[it_local_ext->first][hash_referencias[it_local_int->first]] = peso;
		}
		archivos_procesados++;
		/*if(archivos_procesados > MAX){
				cout << "WRITING" << endl;
				cargar_a_disco(vectores);
				delete vectores;
				vectores = new map<string,map<unsigned int,float> >;
				archivos_procesados = 0;
		}*/
	}
	cargar_a_disco(vectores);
	return vectores;
}

	

