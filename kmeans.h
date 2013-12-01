#ifndef K_MEANS_H
#define K_MEANS_H



#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include "cluster/cluster.h"
#include <cmath>
#include <sstream>

#define CANT_TERMS 3

using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::string;
using std::ostringstream;



	





	



bool cambiaron_centroides(map<string,Cluster*>* clusters,float tolerancia);
		


map<unsigned int,float>* generar_centroide_aleatorio(unsigned int cant_terminos);
		
/*Devuelve la clave dentro del map donde esta el cluster con distancia minima al vector*/
string distancia_minima_key(map<string,Cluster*>* clusters,map<unsigned int,float>* vector,float norma);
	
/*Devuelve la distancia minima entre vector y los centroides*/
float distancia_minima(vector<map<unsigned int,float>* >* centroides,map<unsigned int,float>* vector,float norma);
		
void resetear_clusters(map<string,Cluster*>* clusters);




map<string,Cluster*>* k_means(map<string,map<unsigned int,float> >* vectores,unsigned int cant_clusters,unsigned int cant_terms,float tolerancia);


#endif //K_MEANS_H
