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
#define PORCENTAJE_VECTORES 0.3		
		

map<string,map<unsigned int,float> >* vectorizar(Parser* parser){

	unsigned int archivos_procesados = 0;
	map<string,unsigned int> hash_referencias; //Guarda las referencias de los terminos a los int que los representan
	map<string,map<unsigned int,float> >* vectores = new map<string,map<unsigned int,float> > ; //Vectores que representan a los archivos.
	map<unsigned int, float> vector_actual;
	map<string,float> pesos_globales; //Se guardan los pesos globales calculados;
	map<string,unsigned int>::iterator it_global;
	map<string,map<string,unsigned int> >::iterator it_local_ext;
	map<string,unsigned int>::iterator it_local_int;
	unsigned int i = 0;
	float peso;
	float cant_docs = (float)parser->getCantDocs();
	float division;
	unsigned int distancia_docs = (int)(1/ PORCENTAJE_VECTORES);
	
	for(it_global = parser->getFrecuenciasGlobales()->begin(); it_global !=  parser->getFrecuenciasGlobales()->end(); ++it_global){
		division = (cant_docs/(float)it_global->second);
		hash_referencias[it_global->first] = i;
		pesos_globales[it_global->first] = 1 + log10(division);
		i++;
	}
	string nombre_archivo = PATH;
	map<unsigned int,float>::iterator it_terms;
	for(it_local_ext = parser->getFrecuenciasLocales()->begin(); it_local_ext != parser->getFrecuenciasLocales()->end(); ++it_local_ext){
		cout << "Vectorizando: " << it_local_ext->first << endl;		
		for(it_local_int = it_local_ext->second.begin();it_local_int != it_local_ext->second.end();++it_local_int){
			peso = ((float)it_local_int->second) * pesos_globales[it_local_int->first];
			vector_actual[hash_referencias[it_local_int->first]] = peso;
		}

		if (archivos_procesados % distancia_docs == 0)
			(*vectores)[it_local_ext->first] = vector_actual;
		
		  ///////////////
		//Escritura en Disco
		nombre_archivo += it_local_ext->first;
		ofstream archivo(nombre_archivo.c_str());	

		for (it_terms = vector_actual.begin(); it_terms != vector_actual.end(); ++it_terms)
			archivo << it_terms->first << ":" << it_terms->second << endl;
		
		archivo.close();
		////////////////
		archivos_procesados++;
	}
	
	return vectores;
}

