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
#include "cluster/cluster.h"

using std::pair;
using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;
using std::make_pair;


void setear_id_para_clusters(map<unsigned int,Cluster*>* hash_clusters){
	map<unsigned int,Cluster*>::iterator it;
	int count = 0;
	for(it = (*hash_clusters).begin();it != (*hash_clusters).end(); ++it){
		(*(it->second)).setear_id(count);
		count++;
	}
}


void calcular_distancias_iniciales(unsigned int cant_clusters_iniciales,map<pair<unsigned int,unsigned int>,float>* distancias_entre_clusters,map<unsigned int,Cluster*>* hash_clusters){
	unsigned int contador = 0;
	unsigned int count_aux = 0;
	while(contador < cant_clusters_iniciales){
		while(count_aux < cant_clusters_iniciales){
			if(count_aux != contador){
				if(contador > count_aux)
					(*distancias_entre_clusters)[make_pair(count_aux,contador)] = (*((*hash_clusters)[contador])).calcular_distancia((*((*hash_clusters)[count_aux])).get_centroide(),(*((*hash_clusters)[count_aux])).get_norma());
			}
			count_aux++;
		}
		count_aux = 0;
		contador++;
	}
}

//elimino del hash las distancias en las que interviene el cluster que se agrupó
void eliminar_distancias(unsigned int out,unsigned int cant_clusters_iniciales,map<unsigned int,int>* posicion_prohibida,map<pair<unsigned int,unsigned int>,float>* distancias_entre_clusters){
	unsigned int contador;	
	for(contador = 0;contador < cant_clusters_iniciales;contador++){
		if((*posicion_prohibida)[contador] != 1){
			if(out > contador)
				(*distancias_entre_clusters).erase(make_pair(contador,out));
			else
				(*distancias_entre_clusters).erase(make_pair(out,contador));
		}
	}
}

/*ORIGINAL*/
void agrupar_cluster(Cluster* cluster,map<unsigned int,Cluster*>* hash_clusters){
	int dist = -1;
	int dist_actual;
	unsigned int pareja;
	map<unsigned int,Cluster*>::iterator it;
	for(it = (*hash_clusters).begin();it!= (*hash_clusters).end();++it){
		dist_actual = (*(it->second)).calcular_distancia((*cluster).get_centroide(),(*cluster).get_norma());
		if(dist < dist_actual){
			dist = dist_actual;
			pareja = it->first;
		}
	}
	(*((*hash_clusters)[pareja])).recalcular((*cluster).get_docs(),(*cluster).get_centroide(),(*cluster).get_cant_docs());
	delete cluster;
}


/*ORIGINAL*/
void agrupar_clusters_iniciales(map<pair<unsigned int,unsigned int>,float>* distancias_entre_clusters,map<unsigned int,Cluster*>* hash_clusters,unsigned int cant_clusters_iniciales,unsigned int cant_clusters_actuales,unsigned int cant_clusters_finales,unsigned int ultimo_cluster_agrupado,map<unsigned int,int>* posicion_prohibida){
	if(cant_clusters_actuales > cant_clusters_finales){
		unsigned int count_aux = 0;
		if(cant_clusters_actuales == cant_clusters_iniciales)
			calcular_distancias_iniciales(cant_clusters_iniciales,distancias_entre_clusters,hash_clusters);
		else{
			while(count_aux < cant_clusters_iniciales){
				if(count_aux != ultimo_cluster_agrupado){
					if((*posicion_prohibida)[count_aux] != 1){
						if(ultimo_cluster_agrupado > count_aux)
							(*distancias_entre_clusters)[make_pair(count_aux,ultimo_cluster_agrupado)] = (*((*hash_clusters)[ultimo_cluster_agrupado])).calcular_distancia((*((*hash_clusters)[count_aux])).get_centroide(),(*((*hash_clusters)[count_aux])).get_norma());
						else
							(*distancias_entre_clusters)[make_pair(ultimo_cluster_agrupado,count_aux)] = (*((*hash_clusters)[ultimo_cluster_agrupado])).calcular_distancia((*((*hash_clusters)[count_aux])).get_centroide(),(*((*hash_clusters)[count_aux])).get_norma());
					}	
				}
				count_aux++;
			}
		}
		float dist_actual = -1;
		pair<unsigned int,unsigned int> prox_a_agruparse;
		map<pair<unsigned int, unsigned int>,float>::iterator it2; 
		for(it2 = (*distancias_entre_clusters).begin(); it2 != (*distancias_entre_clusters).end();++it2){
			if(dist_actual < it2->second){
				dist_actual = it2->second;
				prox_a_agruparse = it2->first;
			}    //ACA NO HAGO NADA DE LA ENTROPIA. SI QUEREMOS DESPUES LO METEMOS POR ACA
		}
		unsigned int prox,out;
		(*(*hash_clusters)[prox_a_agruparse.second]).recalcular((*(((*hash_clusters)[prox_a_agruparse.first]))).get_docs(),(*((*hash_clusters)[prox_a_agruparse.first])).get_centroide(),(*((*hash_clusters)[prox_a_agruparse.first])).get_cant_docs());
		prox = prox_a_agruparse.second;
		out = prox_a_agruparse.first;
		delete (*hash_clusters)[out];
		(*hash_clusters).erase(out);
		(*posicion_prohibida)[out] = 1;
		eliminar_distancias(out,cant_clusters_iniciales,posicion_prohibida,distancias_entre_clusters);
		agrupar_clusters_iniciales(distancias_entre_clusters,hash_clusters,cant_clusters_iniciales,cant_clusters_actuales - 1,cant_clusters_finales,prox,posicion_prohibida);				
	}
}

/*UN DOC EN MÁS DE UN CLUSTER*/
void agrupar_cluster_alternativo(Cluster* cluster,map<unsigned int,Cluster*>* hash_clusters,unsigned int cant_clusters_finales,float cota){
	unsigned int count;
	float dist;
	for(count = 0; count < cant_clusters_finales;count++){
		dist = (*((*hash_clusters)[count])).calcular_distancia((*cluster).get_centroide(),(*cluster).get_norma());
		if(dist > cota)
			(*((*hash_clusters)[count])).recalcular((*cluster).get_docs(),(*cluster).get_centroide(),(*cluster).get_cant_docs());
	}
	delete cluster;
}

/*UN DOC EN MÁS DE UN CLUSTER*/
void agrupar_clusters_iniciales_alternativo(map<unsigned int,Cluster*>* hash_clusters,unsigned int cant_clusters_actuales,unsigned int cant_clusters_finales,float* cota){
	if(cant_clusters_actuales > cant_clusters_finales){
		unsigned int contador = 0;
		unsigned int count_aux = 0;
		unsigned int pos = 0;
		float dist;
		unsigned int cant = cant_clusters_actuales;
		map<unsigned int,int> clusters_a_borrar = map<unsigned int,int>();
		map<unsigned int,Cluster*> new_hash_clusters = map<unsigned int,Cluster*>();
		string docs;
		map<unsigned int,float> centroide;
		if(cota != 0){
			while(contador < cant_clusters_actuales){
				while(count_aux < cant_clusters_actuales){
					if(cant + pos == cant_clusters_finales)
						break;
					if(count_aux != contador){
						if(contador > count_aux){
							dist = (*((*hash_clusters)[contador])).calcular_distancia((*((*hash_clusters)[count_aux])).get_centroide(),(*((*hash_clusters)[count_aux])).get_norma());
							if(dist > (*cota)){
								centroide = ((*((*hash_clusters)[contador])).get_centroide());
								docs = ((*((*hash_clusters)[contador])).get_docs());
								new_hash_clusters[pos] = new Cluster(&centroide,&docs);
								(*(new_hash_clusters[pos])).recalcular((*((*hash_clusters)[count_aux])).get_docs(),(*((*hash_clusters)[count_aux])).get_centroide(),(*((*hash_clusters)[count_aux])).get_cant_docs());
								if(clusters_a_borrar[contador] != 1){
									clusters_a_borrar[contador] = 1;
									cant -= 1;
								}
								if(clusters_a_borrar[count_aux] != 1){
									clusters_a_borrar[count_aux] = 1;
									cant-= 1;
								}
								pos++;
							}
						}
					}
					count_aux++;
				}
				count_aux = 0;
				contador++;
			}
		}
		else{
			map<pair<unsigned int,unsigned int>,float> distancias_entre_clusters = map<pair<unsigned int,unsigned int>,float>();
			map<unsigned int,int> posicion_prohibida = map<unsigned int,int>();
			agrupar_clusters_iniciales(&distancias_entre_clusters,hash_clusters,cant_clusters_actuales,cant_clusters_actuales,cant_clusters_finales,0,&posicion_prohibida);
		}
		if(pos!= 0){
			for(contador = 0;contador < cant_clusters_actuales ;contador++){
				if(clusters_a_borrar[contador] == 1)
					delete (*hash_clusters)[contador];
				else{
					new_hash_clusters[pos] = (*hash_clusters)[contador];
					pos++;
				}
			}
			(*hash_clusters) = new_hash_clusters;
		}
		else
			(*cota) = 0;
		agrupar_clusters_iniciales_alternativo(hash_clusters,pos,cant_clusters_finales,cota);
	}
}
	

void agrupar(map<unsigned int,Cluster*>* hash_clusters,const char* nombre_dir,int cant_docs_iniciales,int cant_clusters_finales,float cota,int multi){
	DIR* dir_pointer = opendir(nombre_dir);
	string nombre_directorio = nombre_dir;
	struct dirent* reg_buffer = readdir(dir_pointer);
	nombre_directorio+= '/';
	int count = 0;
	ifstream archivo_frecuencias;
	map<unsigned int,float> hash_frecuencias;
	map<pair<unsigned int,unsigned int>,float> distancias_entre_clusters = map<pair<unsigned int,unsigned int>,float>();
	string nombre_doc,parseo_actual,linea,ubicacion,frecuencia;
	char letra_actual;
	int pos_actual,linea_size;
	map<unsigned int,float> aux = map<unsigned int,float>();
	string string_aux = "";
	Cluster cluster = Cluster(&aux,&string_aux);
	int check_no_temp = 0;
	while(reg_buffer != NULL){
		nombre_doc = (reg_buffer->d_name);
		cout << "Parsing: " << nombre_directorio + nombre_doc << endl; 
		archivo_frecuencias.open((nombre_directorio + nombre_doc).c_str());
		hash_frecuencias = map<unsigned int, float>(); 
		while(getline(archivo_frecuencias,linea)){
			check_no_temp = 1;
			parseo_actual = "";
			linea_size = linea.size();
			for(pos_actual = 0;pos_actual < linea_size;pos_actual++){
				letra_actual = linea[pos_actual];
				if(letra_actual == ':'){ 
					ubicacion = parseo_actual;
					parseo_actual = "";		
				}
				else
					parseo_actual += letra_actual; 
			}
			frecuencia = parseo_actual;
			hash_frecuencias[atoi(ubicacion.c_str())] = atof(frecuencia.c_str());
		}
		archivo_frecuencias.close();
		if(check_no_temp != 0){
			if(count < cant_docs_iniciales - 1){
				(*hash_clusters)[count] = new Cluster(&hash_frecuencias,&nombre_doc);
				count++;
			}
			else if(count == cant_docs_iniciales - 1){
				(*hash_clusters)[count] = new Cluster(&hash_frecuencias,&nombre_doc);
				map<unsigned int,int> posicion_prohibida = map<unsigned int,int>();
				if(multi == 0)
					agrupar_clusters_iniciales(&distancias_entre_clusters,hash_clusters,cant_docs_iniciales,cant_docs_iniciales,cant_clusters_finales,0,&posicion_prohibida);
				else
					agrupar_clusters_iniciales_alternativo(hash_clusters,cant_docs_iniciales,cant_clusters_finales,&cota);
				setear_id_para_clusters(hash_clusters);
				count++;
			}else{
				if(multi == 0)
					agrupar_cluster(new Cluster(&hash_frecuencias,&nombre_doc),hash_clusters);
				else
					agrupar_cluster_alternativo(new Cluster(&hash_frecuencias,&nombre_doc),hash_clusters,cant_clusters_finales,cota);
			}
		}
		check_no_temp = 0;
		reg_buffer = readdir(dir_pointer);
	}
	closedir(dir_pointer);
}


int main(){
	map<unsigned int,Cluster*> hash_clusters = map<unsigned int,Cluster*>();
	int t_inicio = time(NULL);
	agrupar(&hash_clusters,"docs_clusters",5,2,0.2,1);
	map<unsigned int,Cluster*>::iterator it;
	int t_fin = time(NULL);
	cout << "Tardo: " << t_fin - t_inicio << " segundos";
	ofstream clusters;
	for(it = hash_clusters.begin();it != hash_clusters.end();++it){
		cout << "DOCS: " << (*(it->second)).get_docs() << endl;
		cout << "CANT_DOCS: " << (*(it->second)).get_cant_docs() << endl;
		clusters.open((((*it->second)).get_id()).c_str());
		clusters << (*(it->second)).get_cant_docs() << ";" << (*(it->second)).get_docs() << endl;
		clusters.close();
	}
	while(!hash_clusters.empty()){
		it = hash_clusters.begin();
		delete it->second;
		hash_clusters.erase(hash_clusters.begin());
	}
	return 0;
}
