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
	agrupar(hash_clusters,"../vectores",100,15,0.5,0);
	map<unsigned int,Cluster*>::iterator it;
	int t_fin = time(NULL);
	cout << "Tardo: " << t_fin - t_inicio << " segundos" << endl;
	for(it = (*hash_clusters).begin();it != (*hash_clusters).end();++it){
		cout << "DOCS: " << (*(it->second)).get_docs() << endl;
		cout << "CANT_DOCS: " << (*(it->second)).get_cant_docs() << endl;
		cout << (*it->second).get_id() << endl;
		ofstream clusters((((*it->second)).get_id()).c_str());
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
	for(it = (*hash_clusters).begin();it != (*hash_clusters).end();++it){
		cout << "DOCS: " << (*(it->second)).get_docs() << endl;
		cout << "CANT_DOCS: " << (*(it->second)).get_cant_docs() << endl;
		ofstream clusters((((*it->second)).get_id()).c_str());
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
	//cout << "ALGORITMO EN EL QUE SE AGRUPAN DOS CLUSTERS EN CADA PASADA" << endl;
	//test_agrupar_cluster_a_la_vez();
	cout << endl << "ALGORITMO EN EL QUE SE AGRUPAN DOS O MAS CLUSTERS EN CADA PASADA" << endl;
	test_agrupar_muchos_clusters_a_la_vez();
	return 0;
}
