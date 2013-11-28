#include <iostream>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <map>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <utility>
#include "../cluster/cluster.h"
#include "algoritmo_cluster.h"

#define UNDEFINED 0
#define MULTICLUSTER -1

using std::pair;
using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;
using std::make_pair;


void test_agrupar_cluster_a_la_vez(){
	map<unsigned int,Cluster*>* hash_clusters = new map<unsigned int,Cluster*>();
	int t_inicio = time(NULL);
	agrupar(hash_clusters,"../vectores",100,25,0.5,0);
	map<unsigned int,Cluster*>::iterator it;
	int t_fin = time(NULL);
	cout << "Tardo: " << t_fin - t_inicio << " segundos" << endl;
	ofstream clusters;
	for(it = (*hash_clusters).begin();it != (*hash_clusters).end();++it){
		cout << "DOCS: " << (*(it->second)).get_docs() << endl;
		cout << "CANT_DOCS: " << (*(it->second)).get_cant_docs() << endl;
		clusters.open((((*it->second)).get_id()).c_str());
		clusters << (*(it->second)).get_cant_docs() << ";" << (*(it->second)).get_docs() << endl;
		clusters.close();
	}
	while(!(*hash_clusters).empty()){
		it = (*hash_clusters).begin();
		delete it->second;
		(*hash_clusters).erase((*hash_clusters).begin());
	}
	delete hash_clusters;
}

void test_agrupar_muchos_clusters_a_la_vez(){
	map<unsigned int,Cluster*>* hash_clusters = new map<unsigned int,Cluster*>();
	int t_inicio = time(NULL);
	agrupar(hash_clusters,"../vectores",100,25,0.8,1);
	map<unsigned int,Cluster*>::iterator it;
	int t_fin = time(NULL);
	cout << "Tardo: " << t_fin - t_inicio << " segundos" << endl;
	ofstream clusters;
	for(it = (*hash_clusters).begin();it != (*hash_clusters).end();++it){
		cout << "DOCS: " << (*(it->second)).get_docs() << endl;
		cout << "CANT_DOCS: " << (*(it->second)).get_cant_docs() << endl;
		clusters.open((((*it->second)).get_id()).c_str());
		clusters << (*(it->second)).get_cant_docs() << ";" << (*(it->second)).get_docs() << endl;
		clusters.close();
	}
	while(!(*hash_clusters).empty()){
		it = (*hash_clusters).begin();
		delete it->second;
		(*hash_clusters).erase((*hash_clusters).begin());
	}
	delete hash_clusters;
}


int main(){
	map<unsigned int,Cluster*>* hash_clusters;
	cout << "ALGORITMO EN EL QUE SE AGRUPAN DOS CLUSTERS EN CADA PASADA SABIENDO CANTIDAD DE CLUSTERS FINALES" << endl;
	hash_clusters = generar_clusters(600,14,0);
	destruir_clusters(hash_clusters);
	/*cout << endl << "ALGORITMO EN EL QUE SE AGRUPAN DOS O MAS CLUSTERS EN CADA PASADA SABIENDO CANTIDAD DE CLUSTERS FINALES" << endl;
	hash_clusters = generar_clusters(600,14,MULTICLUSTER);
	destruir_clusters(hash_clusters);
	cout << "ALGORITMO EN EL QUE SE AGRUPAN DOS CLUSTERS EN CADA PASADA SIN SABER CANTIDAD DE CLUSTERS FINALES" << endl;
	hash_clusters = generar_clusters(600,UNDEFINED,0);
	destruir_clusters(hash_clusters);
	cout << endl << "ALGORITMO EN EL QUE SE AGRUPAN DOS O MAS CLUSTERS EN CADA PASADA SIN SABER CANTIDAD DE CLUSTERS FINALES" << endl;
	hash_clusters = generar_clusters(600,UNDEFINED,MULTICLUSTER);
	destruir_clusters(hash_clusters);*/	
	return 0;
}
