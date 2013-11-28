#ifndef AGRUPADOR_H
#define AGRUPADOR_H

#include <map>
#include <fstream>
#include <string>
#include "../cluster/cluster.h"
#include "algoritmo_cluster.h"

class Agrupador{
	private:
		int multicluster;
		std::map<unsigned int,Cluster*>* hash_clusters;
		float cota;
		unsigned int cant_clusters,cant_docs;
	public:
		Agrupador(int multi,float cota_aux,unsigned int cant_docs_aux,unsigned int cant_clusters_aux);
		void generar_clusters();
		void agregar_cluster(std::string* doc);
		void destroy();
};

#endif //AGRUPADOR_H
