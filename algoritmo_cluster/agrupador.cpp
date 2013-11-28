#include <iostream>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <utility>
#include "agrupador.h"
#include "../cluster/cluster.h"
#include "algoritmo_cluster.h"

#define UNDEFINED 0
#define FRACCION 6
#define INACTIVO 0

using std::pair;
using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;
using std::make_pair;

Agrupador::Agrupador(int multi,float cota_aux,unsigned int cant_docs_aux,unsigned int cant_clusters_aux){
	multicluster = multi;
	cota = cota_aux;
	hash_clusters = new map<unsigned int,Cluster*>();
	cant_docs = cant_docs_aux;
	if(cant_clusters_aux == UNDEFINED)
		cant_clusters = sqrt(cant_docs);
	else
		cant_clusters = cant_clusters_aux;
}

void Agrupador::generar_clusters(){
	int t_inicio = time(NULL);
	unsigned int muestra_datos = cant_docs/FRACCION;
 	agrupar(hash_clusters,"../vectores",muestra_datos,cant_clusters,cota,multicluster);
 	int t_fin = time(NULL);
	cout << "Tardo: " << t_fin - t_inicio << " segundos" << endl;
	ofstream clusters;
	map<unsigned int,Cluster*>::iterator it;
	for(it = (*hash_clusters).begin();it != (*hash_clusters).end();++it){
		cout << "DOCS: " << (*(it->second)).get_docs() << endl;
		cout << "CANT_DOCS: " << (*(it->second)).get_cant_docs() << endl;
		clusters.open((((*it->second)).get_id()).c_str());
		clusters << (*(it->second)).get_cant_docs() << ";" << (*(it->second)).get_docs() << endl;
		clusters.close();
	}
}

void Agrupador::agregar_cluster(string* doc){
	map<unsigned int,float>* hash_frecuencias = generar_hash_frecuencias((*doc));
	string clusters_intervinientes;
	if(multicluster == INACTIVO)
		clusters_intervinientes = agrupar_cluster(new Cluster(hash_frecuencias,doc),hash_clusters);
	else
		clusters_intervinientes = agrupar_cluster_alternativo(new Cluster(hash_frecuencias,doc),hash_clusters,cota);
	cant_docs++; //sumo uno a la cantidad total de documentos. Todavía no sé si servirá para algo.
	delete hash_frecuencias;
	//Hay que agregar en disco al nuevo doc en los clusters a los que se agrupó.
}

void Agrupador::destroy(){
	map<unsigned int,Cluster*>::iterator it;
	while(!(*hash_clusters).empty()){
		it = (*hash_clusters).begin();
		delete it->second;
		(*hash_clusters).erase((*hash_clusters).begin());
	}
	delete hash_clusters;
}		
