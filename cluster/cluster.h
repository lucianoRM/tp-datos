#ifndef CLUSTER_H
#define CLUSTER_H

#include <map>
#include <fstream>
#include <string>
#include <vector>

class Cluster {
	private:
		std::string docs;
		unsigned int cant_terms;
		float entropia_actual;
		float entropia_sig;
		float distancia; //auxiliar para el calculo de entropia
		std::map<unsigned int,float> centroide_actual;
		std::map<unsigned int,float> centroide_anterior;
		unsigned int cant_docs;
		float norma;
		std::string id; 
	public:
		Cluster(std::map<unsigned int,float>* centroid,std::string doc,unsigned int cantidad_terms);
		float variacion_entropia();
		std::map<unsigned int,float>* get_centroide();
		unsigned int get_cant_docs();
		void setear_id(int id2);
		float get_norma();
		std::string get_id();
		std::string get_docs();
		//void recalcular(std::string doc,std::map<unsigned int,float> centroide_aux,unsigned int cant_docs_aux);
		void agregar_vector(std::map<unsigned int,float>* vector,std::string nombre);
		void recalcular_centroide(std::map<unsigned int,float>* vector,unsigned int cant_terms);
		bool cambio_centroide(float tolerancia);
		void resetear_cluster();
		//~Cluster();
};

float calcular_norma(std::map<unsigned int,float> vector);

//Devuelve -1 si norma_aux es igual a 0.
float calcular_distancia(std::map<unsigned int,float>* centroide,float norma,std::map<unsigned int,float>* centroide_aux,float norma_aux);

#endif //CLUSTER_H
