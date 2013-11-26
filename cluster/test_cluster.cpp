#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <cmath>
#include "cluster.h"

using std::map;
using std::ifstream;
using std::string;
using std::cout;
using std::endl;


int main(){
	map<unsigned int,float> doc1 = map<unsigned int,float>();
	doc1[98] = 232;
	doc1[121] = 1242;
	doc1[124] = 252;
	string nombre_doc1 = "doc1.txt";
	map<unsigned int,float> doc2 = map<unsigned int,float>();
	doc2[121] = 44;
	doc2[22] = 44;
	doc2[353] = 66;
	doc2[12] = 55;
	doc2[1] = 6;
	doc2[3] = 8;
	doc2[9] = 25;
	doc2[12]=11;
	doc2[53]=93;
	doc2[33]=32;
	string nombre_doc2 = "doc2.txt";
	Cluster cluster_doc1 = Cluster(&doc1,&nombre_doc1);
	cout << "DOCS DEL CLUSTER 1:" << cluster_doc1.get_docs() <<endl;
	Cluster cluster_doc2 = Cluster(&doc2,&nombre_doc2);
	cout << "DOCS DEL CLUSTER 2:" << cluster_doc2.get_docs() <<endl;
	cout << "NORMA DEL CLUSTER 1: " << cluster_doc1.get_norma() << endl;
	cout << "NORMA DEL CLUSTER 2: " << cluster_doc2.get_norma() << endl;
	cout << "CANTIDAD DE DOCS DEL CLUSTER 1: " << cluster_doc1.get_cant_docs() << endl;
	cout << "CANTIDAD DE DOCS DEL CLUSTER 2: " << cluster_doc2.get_cant_docs() << endl;
	map<unsigned int,float>::iterator it1;
	doc1 = cluster_doc1.get_centroide();
	for(it1 = doc1.begin(); it1 != doc1.end(); ++it1){
		cout << it1->first << "-" << it1->second << endl;
	}
	map<unsigned int,float>::iterator it2;
	cout << endl;
	doc1 = cluster_doc1.get_centroide();
	for(it2 = doc2.begin(); it2 != doc2.end(); ++it2){
		cout << it2->first << "-" << it2->second << endl;
	}
	cout << "DISTANCIA ENTRE CLUSTER1 Y CLUSTER2: " << cluster_doc1.calcular_distancia(cluster_doc2.get_centroide(),cluster_doc2.get_norma()) << endl;
	cout << "DISTANCIA ENTRE CLUSTER2 Y CLUSTER1: " << cluster_doc2.calcular_distancia(cluster_doc1.get_centroide(),cluster_doc1.get_norma()) << endl;
	cout << "VARIACION DE ENTROPIA CLUSTER1 SI LLEGO A AGRUPARLO CON CLUSTER2: " << cluster_doc1.variacion_entropia() << endl;
	cout << "VARIACION DE ENTROPIA CLUSTER2 SI LLEGO A AGRUPARLO CON CLUSTER1: " << cluster_doc2.variacion_entropia() << endl;
	nombre_doc2 = cluster_doc2.get_docs();
	cluster_doc1.recalcular(nombre_doc2,cluster_doc2.get_centroide(),cluster_doc2.get_cant_docs());
	cout << "DOCS DEL CLUSTER 1 LUEGO DE AGRUPARSE: " << cluster_doc1.get_docs() <<endl;
	cout << "NORMA DEL CLUSTER 1 AGRUPADO CON CLUSTER2: " << cluster_doc1.get_norma() << endl;
	cout << "CANTIDAD DE DOCS DEL CLUSTER 1 AGRUPADO CON CLUSTER2: " << cluster_doc1.get_cant_docs() << endl;
	doc1 = cluster_doc1.get_centroide();
	for(it1 = doc1.begin(); it1 != doc1.end(); ++it1){
		cout << it1->first << "-" << it1->second << endl;
	}
	return 0;
}
