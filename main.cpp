#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <string>
#include "parser/parser.h"
#include <dirent.h>
#include <string.h>
#include <time.h>
#include "porter/porter.h"
#include "parser/vectorizador.h"
#include "dir_seeker.h"
#include "cluster/cluster.h"
#include "kmeans.h"
#include "hierarchical.h"
#include "indice/indice.h"
#include <cstdlib>

#define NOMBRE_TP "./TpGrupo1"

using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;




map<string,Cluster*>* convertir(vector<Cluster*>* clusters){
	
	vector<Cluster*>::iterator it;
	map<string,Cluster*>* retorno = new map<string,Cluster*>;
	for(it = clusters->begin(); it != clusters->end(); ++it){
		(*retorno)[(*it)->get_id()] = (*it);
		cout << clusters->size() <<endl;
	}
	delete clusters;
	return retorno;
} 
		

int vectorizar_documentos(string path, Parser* parser) {
	int retorno = parsear_archivos(path,parser);
	if (retorno != 0){
		cout << "No existe directorio\n";
		delete parser;
		return retorno;
	}
	
	guardar_frecuencias(parser->getFrecuenciasGlobales(), parser->getFrecuenciasLocales());	
	
	return 0;
}


void escribir_centroide_en_disco(Cluster* cluster){
	map<unsigned int,float>* centroide = cluster->get_centroide();
	map<unsigned int,float>::iterator it;
	ofstream archivo_centroide;
	archivo_centroide.open(("Centroides/"+cluster->get_id()).c_str());
	for(it = centroide ->begin(); it != centroide->end(); ++it){
		archivo_centroide << it->first << ";" << it->second << "\n";
	}
	archivo_centroide.close();
}

void escribir_clusters_en_disco(map<string,Cluster*>* clusters,string caracter){
	map<string,Cluster*>::iterator it;
	for(it = clusters->begin(); it!= clusters->end() ; ++it){
		it->second->escribir_a_disco(caracter);
		escribir_centroide_en_disco(it->second);
		delete it->second;
	}
	delete clusters;
}


void cargar_clusters(map<string,Cluster*>* clusters,unsigned int cant_terms){
	
	DIR* dir_pointer = opendir("Centroides");
	map<unsigned int,float> centroide;
	struct dirent* reg_buffer = readdir(dir_pointer);
	string nombre_archivo;
	ifstream archivo;
	
	while(reg_buffer != NULL){    
		nombre_archivo = (reg_buffer->d_name);
 		cout << nombre_archivo << endl;
		if(nombre_archivo == "." || nombre_archivo == ".."){
			reg_buffer = readdir(dir_pointer);
			continue;
		}
		archivo.open(("Centroides/" + nombre_archivo).c_str());		
		string posicion;
		string valor;
        while( getline(archivo, posicion, ';') ){
			getline(archivo, valor);
			centroide[atoi(posicion.c_str())] = atof(valor.c_str());
		}
        archivo.close();
		Cluster* cluster = new Cluster(&centroide,nombre_archivo,cant_terms);
		(*clusters)[nombre_archivo] = cluster;
        reg_buffer = readdir(dir_pointer);
	}

	closedir(dir_pointer);
}

void listar_documentos() {

    DIR* dir_pointer = opendir("Indices");
	//if (dir_pointer == NULL)
	struct dirent* reg_buffer = readdir(dir_pointer);
	
	string nombre_archivo;
	string nombre_cluster;
	string cantidad_terminos;
	ifstream archivo;
	
	while(reg_buffer != NULL){
        nombre_archivo = (reg_buffer->d_name);
        
		if(nombre_archivo == "." || nombre_archivo == ".."){
			reg_buffer = readdir(dir_pointer);
			continue;
		}
		
		archivo.open(("Indices" + nombre_archivo).c_str());		
		getline(archivo, nombre_cluster, ';');
        while( strcmp(nombre_cluster.c_str(),"") != 0){
			cout << nombre_cluster << ", ";
			getline(archivo, nombre_cluster, ';');
		}
		cout << "\n";
        
		archivo.close();
        
		reg_buffer = readdir(dir_pointer);
	}

	closedir(dir_pointer);
}

void listar_clusters(){

    DIR* dir_pointer = opendir("Clusters");
	//if (dir_pointer == NULL)
	struct dirent* reg_buffer = readdir(dir_pointer);
	
	string nombre_archivo;
	string nombre_cluster;
	string cantidad_terminos;
	ifstream archivo;
	
	while(reg_buffer != NULL){
        nombre_cluster = (reg_buffer->d_name);
        
		if(nombre_cluster == "." || nombre_cluster == ".."){
			reg_buffer = readdir(dir_pointer);
			continue;
		}
		
		archivo.open(("Clusters/" + nombre_cluster).c_str());		
		cout << nombre_cluster << ": \n";
		
		while(getline(archivo, nombre_archivo, ';')){
			cout << nombre_archivo <<endl;
		}
		
		cout << "\n\n";
        
		archivo.close();
        
		reg_buffer = readdir(dir_pointer);
	}

	closedir(dir_pointer);	
	
}

void limpiar_directorio(string directorio){

	DIR* dir_pointer = opendir(directorio.c_str());
	
	struct dirent* reg_buffer = readdir(dir_pointer);
	
	string nombre_directorio = directorio;
	string nombre_archivo;
	nombre_directorio+="/";
	
	while(reg_buffer != NULL){
        nombre_archivo = (reg_buffer->d_name);
        
		if(nombre_archivo == "." || nombre_archivo == ".."){
			reg_buffer = readdir(dir_pointer);
			continue;
		}
		
		remove((nombre_directorio+nombre_archivo).c_str());
        
		reg_buffer = readdir(dir_pointer);
	}

	closedir(dir_pointer);
}

void help(){
	cout << "Uso:\n\nCreacion de Clusters:\n\n" << NOMBRE_TP << " -d <path> -c <#clusters> -o <Y/N>\n\n-d indica el path a donde están almacenados los documentos.\n-c indica la cantidad de categorias a crear, si no se indica se decide automaticamente automaticamente.\n-o indica si un documento puede estar en mas de un grupo.\n\nListado de resultados:\n\n" << NOMBRE_TP	 << " -l\nLista todos los documentos del repositorio y la categoría a la cual pertenece cada uno.\n\n" << NOMBRE_TP << " -g\nLista los grupos o categorías existentes y los documentos dentro de cada grupo o categoría.\n\nAgregado de archivos:\n\n" << NOMBRE_TP << " -a <path>\nAgrega y clasifica el texto pasado como parametro e indica a que grupo lo ha agregado."; 
}

int mensaje_error(){
	cout << "\nERROR FATAL: INGRESO DE DATOS INCORRECTO\n";
	help();
	return 1;
}

int main(int args,char* argv[]){
	if (args == 1){
		help();
		return mensaje_error();
	}
	
	
	
	int t_inicio = time(NULL);
	int t_fin;
	Parser *parser = new Parser();
	map<string, map<unsigned int, float> >* vectores;
	map<string, map<unsigned int, float> >* vectores_iniciales;
	unsigned int cant_terms;
	unsigned int cant_docs;
	int retorno;
	map<string,Cluster*>* clusters = new map<string,Cluster*>;
	switch(argv[1][1]) {
		case 'd':
			if(args < 5) return mensaje_error();
			if(args < 5) return mensaje_error();
			if( (strcmp(argv[3], "-c") == 0) ){ 
				if( (strcmp(argv[5], "-o") != 0) )
					return mensaje_error();
			}else{
				if( (strcmp(argv[3], "-o") != 0) )
					return mensaje_error();
			}
			
			retorno = vectorizar_documentos(argv[2], parser);
			if (retorno != 0)
				return retorno;
			
			limpiar_directorio("Clusters");
			limpiar_directorio("Centroides");
			limpiar_directorio("Indices");
			
			vectores = vectorizar(parser);
			vectores_iniciales = obtener_muestra_vectores(vectores);
			cant_terms = parser->getCantTerms();
			cant_docs = parser->getCantDocs();
			delete parser;
			
			
			
			if(strcmp(argv[3], "-c") == 0){
				
				if(args < 7) return mensaje_error();
				
				if(strcmp(argv[5], "-o") != 0) return mensaje_error();
				
				if(strcmp(argv[6], "Y") == 0){
					clusters = k_means(vectores_iniciales,atoi(argv[4]),cant_terms,0.99,0.8);
					delete vectores_iniciales;
					agregado_resto_de_vectores_KY(clusters, vectores);
				}
				else{
					clusters = k_means(vectores_iniciales,atoi(argv[4]),cant_terms,0.99,1);
					delete vectores_iniciales;
					agregado_resto_de_vectores_KN(clusters, vectores);
				}
				
				
			}else{
				vector<Cluster*>* clusters_aux;
				
				if(strcmp(argv[3], "-o") != 0) return mensaje_error();
				
				if(args < 5) return mensaje_error();
				clusters_aux = hierarchical(vectores_iniciales,cant_terms,0.55);
				
				delete vectores_iniciales;
				
				if(strcmp(argv[4], "Y") == 0){
					agregado_resto_de_vectores_HY(clusters_aux, vectores);
				}
				else {
					agregado_resto_de_vectores_HN(clusters_aux, vectores);
				}
				clusters = convertir(clusters_aux);
			}
			

			escribir_clusters_en_disco(clusters,"");
		
			cout << "Cantidad de archivos: "<< cant_docs << endl;
			cout << "Cantidad de terminos: "<< cant_terms << endl;
			t_fin = time(NULL);
			cout << "Tardo: " << t_fin - t_inicio << " segundos" << endl;
			crearIndice("Clusters");
			break;
			
		case 'l':
			listar_documentos();
			break;
			
		case 'g':
			listar_clusters();
			break;
			
		case 'a':
			map<string,float> pesos_globales = recuperar_pesos_globales();
			string nombrearchivo = argv[2];
			unsigned int pos = nombrearchivo.find_last_of("/");
			string path = nombrearchivo.substr(0,pos+1);
			nombrearchivo = nombrearchivo.substr(pos+1);
			parsear_archivo(path,nombrearchivo,parser);
			parser->setFrecuenciasGlobales(pesos_globales);
			cargar_clusters(clusters,parser->getCantTerms());
			vectores = vectorizar(parser);
			agregado_resto_de_vectores_KN(clusters, vectores);
			escribir_clusters_en_disco(clusters,";");
			crearIndice("Clusters");
	}
	
	
	return 0;
}
