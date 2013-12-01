#include <map>
#include <fstream>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include "cluster.h"
#include <vector>

using std::map;
using std::ifstream;
using std::string;
using std::ostringstream;
using std::vector;




//Devuelve -1 si norma_aux es igual a 0.
float calcular_distancia(map<unsigned int,float>* centroide,float norma,map<unsigned int,float>* centroide_aux,float norma_aux){
	float distancia = 0;
	map<unsigned int,float>::iterator it;
	for(it = centroide->begin(); it != centroide->end();++it){
		if((*centroide_aux)[it->first] == 0) {	
			continue;
		}
		distancia += ((*centroide)[it->first]) * ((*centroide_aux)[it->first]);
	}
	
	distancia = distancia/(norma*norma_aux);
	
	return distancia;
}

float calcular_norma(map<unsigned int,float> vector){
	float norma = 0;
	map<unsigned int,float>::iterator it;
	for(it = vector.begin(); it != vector.end();++it)
		norma += vector[it->first] * vector[it->first];
	norma  = sqrt(norma);
	if (norma == 0) return 0.00001;
	return norma;
}


string Cluster::get_id(){
	return id;
}

string Cluster::get_docs(){
	return docs;
}

map<unsigned int,float>* Cluster::get_centroide(){
	return &centroide_actual;
}

float Cluster::get_norma(){
	return norma;
}

unsigned int Cluster::get_cant_docs(){
	return cant_docs;
}

Cluster::Cluster(map<unsigned int,float>* centroid,string nombre,unsigned int cantidad_terms){
	centroide_actual = (*centroid);
	cant_terms = cantidad_terms;
	docs = "";
	entropia_actual = 0;
	entropia_sig = 0;
	norma = calcular_norma(centroide_actual);
	cant_docs = 0;
	id = nombre;
	distancia = 0;
}

bool Cluster::cambio_centroide(float tolerancia){
	float distancia = calcular_distancia(&centroide_actual,calcular_norma(centroide_actual),&centroide_anterior,calcular_norma(centroide_anterior));
	if(distancia > tolerancia) return false;
	return true;
}

void Cluster::recalcular_centroide(map<unsigned int,float>* vector,unsigned int cant_terms){
	map<unsigned int, float>* nuevo_centroide = new map<unsigned int,float>;
	unsigned int i;
	float valor;
	unsigned int cant_docs = get_cant_docs();
	for(i=0;i<cant_terms;i++){
		if((*vector)[i] == 0){
			if(centroide_actual[i] == 0) continue;
			(*nuevo_centroide)[i] = (centroide_actual[i] * (cant_docs + 1)/(cant_docs + 2)); // 1 porque si el cluster esta vacio tiene 0 docs pero no se debe multiplicar por 0, 2 es por el nuevo doc;
		}else{
			valor = centroide_actual[i];
			(*nuevo_centroide)[i] = (valor * (cant_docs + 1) + (*vector)[i])/(cant_docs + 2);
		}
	}
	centroide_actual = (*nuevo_centroide);
	delete nuevo_centroide;
}
	


void Cluster::agregar_vector(map<unsigned int,float>* vector,string nombre){
	if (docs != "")	
		docs+=";";
	docs += nombre;
	recalcular_centroide(vector,cant_terms);
	cant_docs++;
	norma = calcular_norma(centroide_actual);
}



/*Borra todos los documentos, pero mantiene el centroide*/
void Cluster::resetear_cluster(){
	docs = "";
	cant_docs = 0;
	centroide_anterior = centroide_actual;

	
}
