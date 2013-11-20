#ifndef CLUSTER_H
#define CLUSTER_H

#include <map>
#include <fstream>
#include <string>

class Cluster {
	private:
		std::string docs;
		float entropia_actual;
		float entropia_sig;
		float distancia; //auxiliar para el calculo de entropia
		std::map<unsigned int,float> centroide;
		unsigned int cant_docs;
		float norma; 
	public:
		Cluster(std::map<unsigned int,float>* centroid,std::string* doc);
		float calcular_distancia(std::map<unsigned int,float> centroide_aux,float norma_aux);
		float variacion_entropia();
		void recalcular(std::string doc,std::map<unsigned int,float> centroide_aux,unsigned int cant_docs_aux);
};


#endif //CLUSTER_H
