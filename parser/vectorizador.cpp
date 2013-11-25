#include <iostream>
#include <map>
#include <string>
#include <math.h> 
#include "vectorizador.h"
#include "parser.h"

using std::map;
using std::string;
using std::cout;
using std::endl;


map<string,map<unsigned int,float> >* calcular_pesos_globales(Parser* parser){

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
		pesos_globales[it_global->first] = log10(division);
		i++;
	}
	
	for(it_local_ext = parser->getFrecuenciasLocales()->begin(); it_local_ext != parser->getFrecuenciasLocales()->end(); ++it_local_ext){
		cout << "ARCHIVO: " << it_local_ext->first << endl;		
		for(it_local_int = it_local_ext->second.begin();it_local_int != it_local_ext->second.end();++it_local_int){
			peso = ((float)it_local_int->second) * pesos_globales[it_local_int->first];
			(*vectores)[it_local_ext->first][hash_referencias[it_local_int->first]] = peso; 
		}
	}
	return vectores;
}

	

