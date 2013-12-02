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



unsigned int distancia_minima_vector(vector<Cluster*>* clusters,map<unsigned int,float>* centroide,unsigned int norma){
	unsigned int retorno;
	unsigned int i;
	float min_distance;
	float act_distance;
	
	vector<Cluster*>::iterator it;
	
	for(it = clusters->begin(), i = 0;it != clusters->end(); ++it,i++ ){
		if(i == 0) {
			retorno = i;
			min_distance = calcular_distancia((*it)->get_centroide(),(*it)->get_norma(),centroide,norma);
			continue;
		}
		act_distance = calcular_distancia((*it)->get_centroide(),(*it)->get_norma(),centroide,norma);
		if (act_distance > min_distance){
			retorno = i;
			min_distance = act_distance;
		}
	}
	return retorno;
}


void mergear_cercanos(vector<Cluster*>* clusters,float cota){
	
	
	unsigned int i,j;
	unsigned int a = 0;
	unsigned int b = 0; //a y b guardan las posiciones de los 2 clusters a mergear.
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
		
	}
	if(min > cota){
			(*clusters)[a]->agregar_vector((*clusters)[b]->get_centroide(),(*clusters)[b]->get_docs());
			delete (*clusters)[b];
			clusters->erase(clusters->begin() + b );
	}
}
	
void pasar_clusters(vector<Cluster*>* clusters,vector<Cluster*>* clusters_aux){
	unsigned int i = clusters_aux->size() - 1;
	while(clusters_aux->size() != 0){
		clusters->push_back((*clusters_aux)[i]);
		clusters_aux->pop_back();
		i--;
	}
}


void generar_clusters(vector<Cluster*>* clusters,map<string,map<unsigned int,float> >* vectores,unsigned int cant_terms,unsigned int &id){
	
	unsigned int j = 0;
	ostringstream numero;
	map<string,map<unsigned int,float> >::iterator it_vectores;
	unsigned int set = vectores->size()*0.3;
	for(it_vectores = vectores->begin();it_vectores != vectores->end();++it_vectores){
		string nombre("cluster");
		numero << id;
		nombre += numero.str();
		Cluster* cluster = new Cluster(&it_vectores->second,nombre,cant_terms);
		cluster->agregar_vector(&it_vectores->second,it_vectores->first);
		vectores->erase(it_vectores);
		clusters->push_back(cluster);
		numero.str("");
		id++;
		j++;
		if(j == set) break;
	}
}


vector<Cluster*>* hierarchical(map<string,map<unsigned int,float> >* vectores,unsigned int cant_terms,float cota){
	
	unsigned int i;
	unsigned int id = 0;
	vector<Cluster*>* clusters = new vector<Cluster*>;
	vector<Cluster*>* clusters_aux = new vector<Cluster*>;
	unsigned int anterior = 0;
	i = 0;
	generar_clusters(clusters_aux,vectores,cant_terms,id);
	while(clusters_aux->size() != 0){
		while(clusters_aux->size() != anterior){
			cout << i << endl;
			i++;
			anterior = clusters_aux->size();		
			mergear_cercanos(clusters_aux,cota);
		}	
		i = 0;
		pasar_clusters(clusters,clusters_aux);
		generar_clusters(clusters_aux,vectores,cant_terms,id);
	}
	i = 0;
	delete clusters_aux;
	while(clusters->size() != anterior){
		cout << i << endl;
		i++;
		anterior = clusters->size();		
		mergear_cercanos(clusters,cota);
	}	
	return clusters;
}

