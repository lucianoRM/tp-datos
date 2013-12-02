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
using std::ifstream;
using std::ofstream;

#define MAX 100
#define PATH "vectores/"
#define PORCENTAJE_VECTORES 0.3		
		
void guardar_pesos_globales(map<string,float> pesos_globales){
	map<string,float>::iterator it;
	ofstream archivo("globales.txt");
	for(it = pesos_globales.begin();it != pesos_globales.end();++it)
		archivo << it->second << endl << it->first << endl;
	archivo.close();
}

map<string,float> recuperar_pesos_globales(){
	map<string,float> pesos_globales = map<string,float>();
	ifstream archivo("globales.txt");
	string linea;
	string nombre_doc;
	float peso_global = 0;
	int count = 1;
	while(getline(archivo,linea)){
		if(count % 2 == 0){
			nombre_doc = linea;
			pesos_globales[nombre_doc] = peso_global;
		}
		else
			peso_global = atof(linea.c_str());
		count++;
	}
	return pesos_globales;
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
	
	string nombre_archivo = PATH;
	map<unsigned int,float>::iterator it_terms;
	
	for(it_local_ext = parser->getFrecuenciasLocales()->begin(); it_local_ext != parser->getFrecuenciasLocales()->end(); ++it_local_ext){
		cout << "Vectorizando: " << it_local_ext->first << endl;		
		for(it_local_int = it_local_ext->second.begin();it_local_int != it_local_ext->second.end();++it_local_int){
			peso = ((float)it_local_int->second) * pesos_globales[it_local_int->first];
			(*vectores)[it_local_ext->first][hash_referencias[it_local_int->first]] = peso;
		}
		archivos_procesados++;
		
		  ///////////////
		//Escritura en Disco
		
		nombre_archivo += it_local_ext->first;
		ofstream archivo(nombre_archivo.c_str());	

		for (it_terms = ((*vectores)[it_local_ext->first]).begin(); it_terms != ((*vectores)[it_local_ext->first]).end(); ++it_terms)
			archivo << it_terms->first << ":" << it_terms->second << endl;
		
		archivo.close();
		////////////////
		archivos_procesados++;
	}
	guardar_pesos_globales(pesos_globales);
	return vectores;
}

map<string, map<unsigned int, float> >* obtener_muestra_vectores(map<string, map<unsigned int, float> >* vectores){
	unsigned int archivos_procesados = 0;
	unsigned int distancia_docs = (int)(1/ PORCENTAJE_VECTORES);
	map<string, map<unsigned int, float> >* vectores_iniciales = new map<string,map<unsigned int,float> >;
	map<string, map<unsigned int, float> >::iterator it_local_ext;
	
	for(it_local_ext = vectores->begin(); it_local_ext != vectores->end(); ++it_local_ext){
		if (archivos_procesados % distancia_docs == 0){
			(*vectores_iniciales)[it_local_ext->first] = it_local_ext->second;
			cout << it_local_ext->first;
		}
		archivos_procesados++;
	}
	
	return vectores_iniciales;
}
	
