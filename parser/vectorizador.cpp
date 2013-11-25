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


void calcular_pesos_globales(Parser* parser){

	map<string,unsigned int>::iterator it;
	unsigned int i = 0;
	float cant_docs = (float)parser->getCantDocs();
	float division;
	for(it = parser->getFrecuenciasGlobales()->begin(); it !=  parser->getFrecuenciasGlobales()->end(); ++it){
		division = (cant_docs/(float)it->second);
		cout << "Termino: " << it->first << ",Numero: " << i << ",Peso: " << log10(division) << endl;
		i++;
	}


}
	

