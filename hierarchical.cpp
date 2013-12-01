#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include "cluster/cluster.h"
#include <cmath>
#include <sstream>
#include "hierarchical.h"


using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::string;
using std::ostringstream;


void mergear_cercanos(vector<Cluster*>* clusters,float cota){
	
	
	unsigned int i,j,a,b; //a y b guardan las posiciones de los 2 clusters a mergear.
	float distancia;
	float min = 0.0;
	for(i = 0; i<clusters->size();i++){
		for(j = i+1; j < clusters->size(); j++){
			distancia = calcular_distancia((*clusters)[i]->get_centroide(),(*clusters)[i]->get_norma(),(*clusters)[j]->get_centroide(),(*clusters)[j]->get_norma()); 
			if(distancia > min) {
				a = i;
				b = j;
				min = distancia;
			}
		}
		if(min > cota){
			(*clusters)[a]->agregar_vector((*clusters)[b]->get_centroide(),(*clusters)[b]->get_docs());
			delete (*clusters)[b];
			clusters->erase(clusters->begin() + b );
		}
		min = 0.0;
	}
}
		


vector<Cluster*>* hierarchical(map<string,map<unsigned int,float> >* vectores,unsigned int cant_terms,float cota){
	
	unsigned int i;
	Cluster* cluster;
	vector<Cluster*>* clusters = new vector<Cluster*>;
	ostringstream numero;
	map<string,map<unsigned int,float> >::iterator it_vectores;
	for(it_vectores = vectores->begin();it_vectores != vectores->end();++it_vectores){
		string nombre("cluster");
		numero << i;
		nombre += numero.str();
		cluster = new Cluster(&it_vectores->second,nombre,cant_terms);
		cluster->agregar_vector(&it_vectores->second,it_vectores->first);
		clusters->push_back(cluster);
		numero.str("");
		i++;
	}
	i = 0;
	unsigned int anterior = 0;
	while(clusters->size() != anterior){
		cout << i << endl;
		i++;
		anterior = clusters->size();		
		mergear_cercanos(clusters,cota);
		cota += 0.1;
	}	
	return clusters;
}

