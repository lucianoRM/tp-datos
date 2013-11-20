#include <map>
#include <fstream>
#include <string>
#include <cmath>
#include "cluster.h"

using std::map;
using std::ifstream;
using std::string;


Cluster::Cluster(map<unsigned int,float>* centroid,string* doc){
	centroide = (*centroid);
	docs = (*doc);
	entropia_actual = 0;
	entropia_sig = 0;
	norma = 0;
	cant_docs = 1;
	map<unsigned int,float>::iterator it;
	float valor;
	for(it = centroide.begin(); it != centroide.end(); ++it){
		valor = it->second;
		norma += (valor*valor);
	}
	norma = sqrt(norma);
	distancia = 0;
}

//Devuelve -1 si norma_aux es igual a 0.
float Cluster::calcular_distancia(map<unsigned int,float> centroide_aux,float norma_aux){
	distancia = 0;
	entropia_sig = entropia_actual;
	if(norma_aux == 0) return -1;
	map<unsigned int,float>::iterator it1;
	map<unsigned int,float>::iterator it2;
	unsigned int pos1,pos2;
	for(it1 = centroide.begin(),it2 = centroide_aux.begin(); it1 != centroide.end() && it2 != centroide_aux.end();){
		 pos1 = it1->first;
		 pos2 = it2->first;
		 if(pos1 < pos2) it1++;
		 else if(pos2 == pos2){
			distancia += (it1->second)*(it2->second); 
			it1++;
			it2++;
		}
		else it2++;
	}
	distancia /= (norma*norma_aux);
	return distancia;
}

float Cluster::variacion_entropia(){
	float aux = 1 - distancia;
	float logaux = log10(aux);
	float logdistance = log10(distancia);
	entropia_sig = (distancia*logdistance)+(aux*logaux);
	return entropia_sig - entropia_actual;
}

void Cluster::recalcular(string doc,map<unsigned int,float> centroide_aux,unsigned int cant_docs_aux){
	docs += doc;
	map<unsigned int,float>::iterator it;
	unsigned int pos;
	float valor;
	norma = 0;
	cant_docs += cant_docs_aux;
	for(it = centroide_aux.begin(); it != centroide_aux.end();it++){
		 pos = it->first;
		 valor = centroide[pos];
		 if(valor){
			 valor *= (cant_docs - cant_docs_aux);
			 valor += (it->second);
		}
		else valor = (it->second);
		valor /= (cant_docs);
		norma += (valor*valor);
		centroide[pos] = valor; 
	}
	norma = sqrt(norma);
}
