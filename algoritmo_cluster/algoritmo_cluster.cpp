#include <iostream>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <utility>
#include "../cluster/cluster.h"

using std::pair;
using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;
using std::make_pair;

#define PERTENECE 1
#define INF 100000
#define COTA_AUX 0.3
#define NEGATIVO -1
#define MULTICLUSTER -1
#define UNDEFINED 0
#define TAM 6
#define COTA 0.8

void setear_id_para_clusters(map<unsigned int,Cluster*>* hash_clusters){
	map<unsigned int,Cluster*>::iterator it;
	int count = 0;
	for(it = (*hash_clusters).begin();it != (*hash_clusters).end(); ++it){
		(*(it->second)).setear_id(count);
		count++;
	}
}


void calcular_distancias_iniciales(unsigned int cant_clusters_iniciales,map< pair<unsigned int,unsigned int>,pair<unsigned int,float> >* distancias_entre_clusters,map<unsigned int,Cluster*>* hash_clusters){
	unsigned int contador = 0;
	unsigned int count_aux = 0;
	unsigned int cant1,cant2;
	while(contador < cant_clusters_iniciales){
		cant1 = (*((*hash_clusters)[contador])).get_cant_docs();
		while(count_aux < cant_clusters_iniciales){
			if(count_aux != contador){
				if(contador > count_aux){
					cant2 = (*((*hash_clusters)[count_aux])).get_cant_docs();
					(*distancias_entre_clusters)[make_pair(count_aux,contador)] = make_pair(cant1 + cant2,(*((*hash_clusters)[contador])).calcular_distancia((*((*hash_clusters)[count_aux])).get_centroide(),(*((*hash_clusters)[count_aux])).get_norma()));
				}
			}
			count_aux++;
		}
		count_aux = 0;
		contador++;
	}
}

//elimino del hash las distancias en las que interviene el cluster que se agrupó
void eliminar_distancias(unsigned int out,unsigned int cant_clusters_iniciales,map<unsigned int,int>* posicion_prohibida,map< pair<unsigned int,unsigned int>,pair<unsigned int,float> >* distancias_entre_clusters){
	unsigned int contador;	
	for(contador = 0;contador < cant_clusters_iniciales;contador++){
		if((*posicion_prohibida)[contador] != PERTENECE){
			if(out > contador)
				(*distancias_entre_clusters).erase(make_pair(contador,out));
			else
				(*distancias_entre_clusters).erase(make_pair(out,contador));
		}
	}
}

/*ORIGINAL*/
string agrupar_cluster(Cluster* cluster,map<unsigned int,Cluster*>* hash_clusters){
	string clusters = ""; 
	float dist;
	float dist_actual = NEGATIVO;
	unsigned int pareja,cant1,cant2,cant_actual;
	cant1 = (*cluster).get_cant_docs();
	cant_actual = INF;
	map<unsigned int,Cluster*>::iterator it;
	for(it = (*hash_clusters).begin();it!= (*hash_clusters).end();++it){
		dist = (*(it->second)).calcular_distancia((*cluster).get_centroide(),(*cluster).get_norma());
		cant2 = (*(it->second)).get_cant_docs();
		if(dist_actual < dist){
			if(cant_actual > (cant1 + cant2) || (dist - dist_actual) > COTA_AUX){
				dist_actual = dist;
				pareja = it->first;
				cant_actual = cant1 + cant2;
			}
		}
	}
	clusters = ((*((*hash_clusters)[pareja])).get_id());
	(*((*hash_clusters)[pareja])).recalcular((*cluster).get_docs(),(*cluster).get_centroide(),(*cluster).get_cant_docs());
	delete cluster;
	return clusters;
}


/*ORIGINAL*/
void agrupar_clusters_iniciales(map< pair<unsigned int,unsigned int>,pair<unsigned int,float> >* distancias_entre_clusters,map<unsigned int,Cluster*>* hash_clusters,unsigned int cant_clusters_iniciales,unsigned int cant_clusters_actuales,unsigned int cant_clusters_finales,int ultimo_cluster_agrup,map<unsigned int,int>* posicion_prohibida){
	if(cant_clusters_actuales > cant_clusters_finales){
		unsigned int ultimo_cluster_agrupado = ultimo_cluster_agrup;
		unsigned int count_aux = 0;
		unsigned int cant1,cant2;
		if(cant_clusters_actuales == cant_clusters_iniciales || ultimo_cluster_agrup == MULTICLUSTER)
			calcular_distancias_iniciales(cant_clusters_iniciales,distancias_entre_clusters,hash_clusters);
		else{
			cant1 = (*((*hash_clusters)[ultimo_cluster_agrupado])).get_cant_docs();
			while(count_aux < cant_clusters_iniciales){
				if(count_aux != ultimo_cluster_agrupado){
					if((*posicion_prohibida)[count_aux] != PERTENECE){
						cant2 = (*((*hash_clusters)[count_aux])).get_cant_docs();
						if(ultimo_cluster_agrupado > count_aux)
							(*distancias_entre_clusters)[make_pair(count_aux,ultimo_cluster_agrupado)] = make_pair(cant1 + cant2,(*((*hash_clusters)[ultimo_cluster_agrupado])).calcular_distancia((*((*hash_clusters)[count_aux])).get_centroide(),(*((*hash_clusters)[count_aux])).get_norma()));
						else
							(*distancias_entre_clusters)[make_pair(ultimo_cluster_agrupado,count_aux)] = make_pair(cant1 + cant2,(*((*hash_clusters)[ultimo_cluster_agrupado])).calcular_distancia((*((*hash_clusters)[count_aux])).get_centroide(),(*((*hash_clusters)[count_aux])).get_norma()));
					}	
				}
				count_aux++;
			}
		}
		cant1 = INF;
		float dist_actual = NEGATIVO;
		float dist_aux;
		pair<unsigned int,unsigned int> prox_a_agruparse;
		map<pair<unsigned int, unsigned int>,pair<unsigned int,float> >::iterator it2; 
		for(it2 = (*distancias_entre_clusters).begin(); it2 != (*distancias_entre_clusters).end();++it2){
			cant2 = (it2->second).first;
			dist_aux = (it2->second).second;
			if(dist_actual < dist_aux){
				if(cant2 < cant1 || (dist_aux - dist_actual) > COTA_AUX){
					dist_actual = dist_aux;
					prox_a_agruparse = it2->first;
				}
			}    
			//ACA NO HAGO NADA DE LA ENTROPIA. SI QUEREMOS DESPUES LO METEMOS POR ACA
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
string agrupar_cluster_alternativo(Cluster* cluster,map<unsigned int,Cluster*>* hash_clusters,float cota){
	float dist;
	int count = 0;
	string clusters = "";
	map<unsigned int,Cluster*>::iterator it;
	for(it = (*hash_clusters).begin(); it != (*hash_clusters).end();++it){
		dist = (*(it->second)).calcular_distancia((*cluster).get_centroide(),(*cluster).get_norma());
		if(dist > cota){
			(*(it->second)).recalcular((*cluster).get_docs(),(*cluster).get_centroide(),(*cluster).get_cant_docs());
			clusters += (*(it->second)).get_id();
			count++;
		}
	}
	if(count == 0)
		return agrupar_cluster(cluster,hash_clusters);
	else{
		return clusters;
		delete cluster;
	}
}

/*UN DOC EN MÁS DE UN CLUSTER*/
void agrupar_clusters_iniciales_alternativo(map<unsigned int,Cluster*>* hash_clusters,unsigned int cant_clusters_actuales,unsigned int cant_clusters_finales,float cota){
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
		if(cota > 0){
			while(contador < cant_clusters_actuales){
				while(count_aux < cant_clusters_actuales){
					if(cant + pos == cant_clusters_finales)
						break;
					if(count_aux != contador){
						if(contador > count_aux){
							dist = (*((*hash_clusters)[contador])).calcular_distancia((*((*hash_clusters)[count_aux])).get_centroide(),(*((*hash_clusters)[count_aux])).get_norma());
							if(dist > cota){
								centroide = ((*((*hash_clusters)[contador])).get_centroide());
								docs = ((*((*hash_clusters)[contador])).get_docs());
								new_hash_clusters[pos] = new Cluster(&centroide,&docs);
								(*(new_hash_clusters[pos])).recalcular((*((*hash_clusters)[count_aux])).get_docs(),(*((*hash_clusters)[count_aux])).get_centroide(),(*((*hash_clusters)[count_aux])).get_cant_docs());
								if(clusters_a_borrar[contador] != PERTENECE){
									clusters_a_borrar[contador] = PERTENECE;
									cant -= 1;
								}
								if(clusters_a_borrar[count_aux] != PERTENECE){
									clusters_a_borrar[count_aux] = PERTENECE;
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
			if(pos!= 0){
				for(contador = 0;contador < cant_clusters_actuales ;contador++){
					if(clusters_a_borrar[contador] == PERTENECE)
						delete (*hash_clusters)[contador];
					else{
						new_hash_clusters[pos] = (*hash_clusters)[contador];
						pos++;
					}
				}
				(*hash_clusters) = new_hash_clusters;
				agrupar_clusters_iniciales_alternativo(hash_clusters,pos,cant_clusters_finales,cota);
			}
			else{
				map< pair<unsigned int,unsigned int>, pair<unsigned int,float> >* distancias_entre_clusters = new map< pair<unsigned int,unsigned int>,pair<unsigned int,float> >();
				map<unsigned int,int>* posicion_prohibida = new map<unsigned int,int>();
				agrupar_clusters_iniciales(distancias_entre_clusters,hash_clusters,cant_clusters_actuales,cant_clusters_actuales,cant_clusters_finales,MULTICLUSTER,posicion_prohibida);
				delete distancias_entre_clusters;
				delete posicion_prohibida;	
			}
		}
	}
}


map<unsigned int,float>* generar_hash_frecuencias(string doc){
	map<unsigned int,float>* hash_frecuencias = new map<unsigned int,float>();
	char letra_actual;
	int linea_size,pos_actual;
	string ubicacion,frecuencia,parseo_actual,linea;
	ifstream archivo_frecuencias(doc.c_str());
	while(getline(archivo_frecuencias,linea)){
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
		(*hash_frecuencias)[atoi(ubicacion.c_str())] = atof(frecuencia.c_str());
	}
	archivo_frecuencias.close();
	return hash_frecuencias;
}

void agrupar(map<unsigned int,Cluster*>* hash_clusters,const char* nombre_dir,int cant_docs_iniciales,int cant_clusters_finales,float cota,int multi){
	DIR* dir_pointer = opendir(nombre_dir);
	string nombre_directorio = nombre_dir;
	struct dirent* reg_buffer = readdir(dir_pointer);
	nombre_directorio+= '/';
	int count = 0;
	map<unsigned int,float>* hash_frecuencias;
	map< pair<unsigned int,unsigned int>,pair<unsigned int,float> >* distancias_entre_clusters = new map< pair<unsigned int,unsigned int>,pair<unsigned int,float> >();
	string* nombre_doc;
	while(reg_buffer != NULL){
		nombre_doc = new string(reg_buffer->d_name);
		hash_frecuencias = generar_hash_frecuencias((nombre_directorio + (*nombre_doc)));
		if(count < cant_docs_iniciales - 1){
			(*hash_clusters)[count] = new Cluster(hash_frecuencias,nombre_doc);
			count++;
		}
		else if(count == cant_docs_iniciales - 1){
			(*hash_clusters)[count] = new Cluster(hash_frecuencias,nombre_doc);
			map<unsigned int,int>* posicion_prohibida = new map<unsigned int,int>();
			if(multi == 0)
				agrupar_clusters_iniciales(distancias_entre_clusters,hash_clusters,cant_docs_iniciales,cant_docs_iniciales,cant_clusters_finales,0,posicion_prohibida);
			else
				agrupar_clusters_iniciales_alternativo(hash_clusters,cant_docs_iniciales,cant_clusters_finales,cota);
			setear_id_para_clusters(hash_clusters);
			delete posicion_prohibida;
			count++;
		}else{
			if(multi == 0)
				agrupar_cluster(new Cluster(hash_frecuencias,nombre_doc),hash_clusters);
			else
				agrupar_cluster_alternativo(new Cluster(hash_frecuencias,nombre_doc),hash_clusters,cota);
		}
		reg_buffer = readdir(dir_pointer);
		delete hash_frecuencias;
		delete nombre_doc;
	}
	delete distancias_entre_clusters;
	closedir(dir_pointer);
}



