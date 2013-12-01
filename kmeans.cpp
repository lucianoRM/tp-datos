#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include "cluster/cluster.h"
#include <cmath>
#include <sstream>
#include "kmeans.h"

#define COTA 0.8



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
		suma = rand() % 100 + 1;		
		j += suma;	
	}
	
	return centroide;
}

float distancia_minima(vector<map<unsigned int,float>* >* centroides,map<unsigned int,float>* archivo,float norma){
	float min_distance = -1.0;
	float act_distance = 0.0;
	vector<map<unsigned int,float>* >::iterator it;
	
	for(it = centroides->begin();it != centroides->end(); ++it ){
		if(min_distance == -1.0) {
			min_distance = calcular_distancia((*it),calcular_norma(*(*it)),archivo,norma);
			continue;
		}
		act_distance = calcular_distancia((*it),calcular_norma(*(*it)),archivo,norma);
		if (act_distance > min_distance){
			min_distance = act_distance;
		}
	}
	return min_distance;
}
		
/*Devuelve la clave dentro del map donde esta el cluster con distancia minima al vector*/
string distancia_minima_key(map<string,Cluster*>* clusters,map<unsigned int,float>* vector,float norma){
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
	}
	return retorno;
}
		
void resetear_clusters(map<string,Cluster*>* clusters){
	map<string,Cluster*>::iterator it;
	for(it = clusters->begin(); it != clusters->end() ;++it)
		it->second->resetear_cluster();
}

void agregar_centroides_aleatorios(vector<map<unsigned int,float>* >* centroides,unsigned int cant_centroides,unsigned int cant_terms){
	unsigned int i;	
	for (i= 0; i < cant_centroides;i++)
			centroides->push_back(generar_centroide_aleatorio(cant_terms));
}
	
vector<string> min_distances(map<string,Cluster*>* clusters,map<unsigned int,float>* archivo,float norma,float cota){
	vector<string> retorno;
	float act_distance;
	map<string,Cluster*>::iterator it;
	for(it = clusters->begin();it != clusters->end(); ++it ){
		act_distance = calcular_distancia(it->second->get_centroide(),it->second->get_norma(),archivo,norma);
		if (act_distance > cota)
			retorno.push_back(it->first);
	}
	return retorno;
}


map<string,Cluster*>* k_means(map<string,map<unsigned int,float> >* vectores,unsigned int cant_clusters,unsigned int cant_terms,float tolerancia,float cota){
	
	unsigned int i,j;
	Cluster* cluster;
	map<string,Cluster*>* clusters = new map<string,Cluster*>;
	ostringstream numero;
	string mas_cercano;
	vector<string> mas_cercanos;
	float min_distance;
	unsigned int agregados;
	map<string,map<unsigned int,float> >::iterator it_vectores;
	vector<map<unsigned int,float>* >* centroides = new vector<map<unsigned int,float>* >;
	unsigned int size_vectores = vectores->size();
	unsigned int salto = size_vectores/cant_clusters;//para no agarrar entre los primeros y agarrar sobre toda la lista.
	if(salto == 0) salto = 1;
	for(it_vectores = vectores->begin(),agregados = 0;it_vectores != vectores->end() && agregados < cant_clusters;agregados++){
		min_distance = distancia_minima(centroides,&it_vectores->second,calcular_norma(it_vectores->second));
		if(min_distance > COTA) ++it_vectores;
		else centroides->push_back(&it_vectores->second);
		for(j=0;j<salto;j++) ++it_vectores;
	}
	unsigned int size = centroides->size();
	//si faltan centroides para completar los necesarios para la cantidad de clusters		
	if(size < cant_clusters)
		agregar_centroides_aleatorios(centroides,(cant_clusters - size),cant_terms);
	//Lleno clusters con los generados con centroides aleatorios
	for(i = 0; i < cant_clusters;i++){
		string nombre("cluster");
		numero << i;
		nombre += numero.str();
	
		cluster = new Cluster((*centroides)[i],nombre,cant_terms);
		//libera los centroides generados aleatoriamente		
		if(i>=size)
			delete (*centroides)[i];
		(*clusters)[nombre] = cluster;
		numero.str("");
	}
	delete centroides;
	i = 0;
	while(cambiaron_centroides(clusters,tolerancia) != false){
		cout << i << endl;
		i++;
		resetear_clusters(clusters);
		if(cota == 1){
			for(it_vectores = vectores->begin();it_vectores != vectores->end();++it_vectores){
				mas_cercano = distancia_minima_key(clusters,&it_vectores->second,calcular_norma(it_vectores->second));
				(*clusters)[mas_cercano]->agregar_vector(&it_vectores->second,it_vectores->first);
			}
		}else{
			for(it_vectores = vectores->begin();it_vectores != vectores->end();++it_vectores){
				mas_cercanos = min_distances(clusters,&it_vectores->second,calcular_norma(it_vectores->second),cota);
				if(mas_cercanos.size() == 0) mas_cercanos.push_back(distancia_minima_key(clusters,&it_vectores->second,calcular_norma(it_vectores->second)));
				for(unsigned int h = 0; h<mas_cercanos.size();h++){
					(*clusters)[mas_cercanos[h]]->agregar_vector(&it_vectores->second,it_vectores->first);
					cout << mas_cercanos[h] << endl;
				}
			}
		}	
	}	
	return clusters;
}




	
