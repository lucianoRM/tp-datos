#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include "cluster/cluster.h"
#include <cmath>
#include <sstream>


using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::string;
using std::ostringstream;











bool cambiaron_centroides(map<string,Cluster*>* clusters,float tolerancia){
	map<string,Cluster*>::iterator it;
	for(it = clusters->begin(); it != clusters->end() ;++it)
		if(it->second->cambio_centroide(tolerancia)) return true;
	return false;
}
		


	

map<unsigned int,float>* generar_centroide_aleatorio(unsigned int cant_terminos){
	unsigned int j = 0;
	float peso;
	int suma;
	srand(time(NULL));
	map<unsigned int,float>* centroide = new map<unsigned int,float>;
	while(j <= cant_terminos){		
		peso = (rand() % 27 + 7)/(float)7; //es primo, genera numeros float entre 1 y 4, 4 nunca.
		(*centroide)[j] = peso; //Numero entre 1 y 4 (es mayor a 1 porque van a ser todos asi, y menor a 4 porque es un peso normal para un termino.)
		suma = rand() % 10 + 1;		
		j += suma;	
	}
	
	return centroide;
}
		
/*Devuelve la clave dentro del map donde esta el cluster con distancia minima al vector*/
string distancia_minima(map<string,Cluster*>* clusters,map<unsigned int,float>* vector,float norma){
	string retorno = "";
	float min_distance;
	float act_distance;
	map<string,Cluster*>::iterator it;
	for(it = clusters->begin();it != clusters->end(); ++it ){
		if(retorno == "") {
			retorno = it->first;
			min_distance = calcular_distancia(it->second->get_centroide(),it->second->get_norma(),vector,norma);
			continue;
		}
		act_distance = calcular_distancia(it->second->get_centroide(),it->second->get_norma(),vector,norma);
		if (act_distance > min_distance){
			retorno = it->first;
			min_distance = act_distance;
		}
		//cout << "min_distance: " << min_distance << endl;
	}
	return retorno;
}
		
void resetear_clusters(map<string,Cluster*>* clusters){
	map<string,Cluster*>::iterator it;
	for(it = clusters->begin(); it != clusters->end() ;++it)
		it->second->resetear_cluster();
}





map<string,Cluster*>* k_means(map<string,map<unsigned int,float> >* vectores,unsigned int cant_clusters,unsigned int cant_terms,float tolerancia){
	
	unsigned int i;
	Cluster* cluster;
	map<string,Cluster*>* clusters = new map<string,Cluster*>;
	ostringstream numero;
	string mas_cercano;
	unsigned int agregados;
	map<string,map<unsigned int,float> >::iterator it_vectores;
	vector<map<unsigned int,float>* >* centroides = new vector<map<unsigned int,float>* >;// = generar_centroides_aleatorios(cant_clusters,cant_terms);
	for(it_vectores = vectores->begin(),agregados = 0;it_vectores != vectores->end() && agregados < cant_clusters;++it_vectores, agregados++)
		centroides->push_back(&it_vectores->second);
	 
	unsigned int size = centroides->size();
	cout << "size:" << centroides->size() << " Cant clusters: " << cant_clusters << endl;
	if(size < cant_clusters){
		cout << "diferencias: " << cant_clusters - size << endl;
		for (i= 0; i < (cant_clusters - size);i++)
			centroides->push_back(generar_centroide_aleatorio(cant_terms));
	}
	//Lleno clusters con los generados con centroides aleatorios
	for(i = 0; i < cant_clusters;i++){
		string nombre("cluster");
		numero << i;
		nombre += numero.str();
		
		cluster = new Cluster((*centroides)[i],nombre,cant_terms);
		if(i>=size)
			delete (*centroides)[i];
		(*clusters)[nombre] = cluster;
		numero.str("");
	
	}
	i = 0;
	while(cambiaron_centroides(clusters,tolerancia) != false){
		cout << i << endl;
		i++;
		resetear_clusters(clusters);
		for(it_vectores = vectores->begin();it_vectores != vectores->end();++it_vectores){
			mas_cercano = distancia_minima(clusters,&it_vectores->second,calcular_norma(it_vectores->second));
			(*clusters)[mas_cercano]->agregar_vector(&it_vectores->second,it_vectores->first);
		}
	}	
	delete centroides;
	return clusters;
}

	
