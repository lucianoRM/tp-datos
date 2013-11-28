#ifndef ALGORITMO_CLUSTER_H
#define ALGORITMO_CLUSTER_H

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

void setear_id_para_clusters(std::map<unsigned int,Cluster*>* hash_clusters);

void calcular_distancias_iniciales(unsigned int cant_clusters_iniciales,std::map<std::pair<unsigned int,unsigned int>,std::pair<unsigned int,float> >* distancias_entre_clusters,std::map<unsigned int,Cluster*>* hash_clusters);

void eliminar_distancias(unsigned int out,unsigned int cant_clusters_iniciales,std::map<unsigned int,int>* posicion_prohibida,std::map<std::pair<unsigned int,unsigned int>,std::pair<unsigned int,float> >* distancias_entre_clusters);

void agrupar_cluster(Cluster* cluster,std::map<unsigned int,Cluster*>* hash_clusters);

void agrupar_clusters_iniciales(std::map<std::pair<unsigned int,unsigned int>,std::pair<unsigned int,float> >* distancias_entre_clusters,std::map<unsigned int,Cluster*>* hash_clusters,unsigned int cant_clusters_iniciales,unsigned int cant_clusters_actuales,unsigned int cant_clusters_finales,unsigned int ultimo_cluster_agrup,std::map<unsigned int,int>* posicion_prohibida);

void agrupar_cluster_alternativo(Cluster* cluster,std::map<unsigned int,Cluster*>* hash_clusters,float cota);

void agrupar_clusters_iniciales_alternativo(std::map<unsigned int,Cluster*>* hash_clusters,unsigned int cant_clusters_actuales,unsigned int cant_clusters_finales,float cota);

void agrupar(std::map<unsigned int,Cluster*>* hash_clusters,const char* nombre_dir,int cant_docs_iniciales,int cant_clusters_finales,float cota,int multi);

std::map<unsigned int,Cluster*>* generar_clusters(unsigned int cant_docs,unsigned int cant_clusters,unsigned int multicluster);

void destruir_clusters(std::map<unsigned int,Cluster*>* hash_clusters);

#endif //ALGORITMO_CLUSTER_H

