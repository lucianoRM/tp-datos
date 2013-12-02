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
#include <cstdlib>

#define NOMBRE_TP "./TpGrupo6"

using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;

int vectorizar_documentos(string path, Parser* parser, map<string, map<unsigned int, float> >* vectores) {
	int t_inicio = time(NULL);
	parser =new Parser();
	int retorno = parsear_archivos(argv[2],parser);
	if (retorno != 0){
		cout << "No existe directorio\n";
		delete parser;
		return retorno;
	}
	
	guardar_frecuencias(parser->getFrecuenciasGlobales(), parser->getFrecuenciasLocales());	
	vectores = vectorizar(parser);
	
	return 0;
}
void agregado_resto_de_vectores(map<string,Cluster*>* clusters, map<string, map<unsigned int, float> >* vectores){
	
	string mas_cercano;
	map<string,map<unsigned int,float> >::iterator it_vectores2;
				
	unsigned int distancia_docs = (int)(1/ 0.3);
	int i = 0;
	
	for(it_vectores2 = vectores->begin();it_vectores2 != vectores->end();++it_vectores2){
		if (i % distancia_docs != 0){
			mas_cercano = distancia_minima_key(clusters,&it_vectores2->second,calcular_norma(it_vectores2->second));
			(*clusters)[mas_cercano]->agregar_vector(&it_vectores2->second,it_vectores2->first);
		}
		i++;
	}
	delete vectores;
}

void escribir_clusters_en_disco(map<string,Cluster*>* clusters){
	map<string,Cluster*>::iterator it;
	for(it = clusters->begin(); it!= clusters->end() ; ++it){
		it->second->escribir_a_disco();
		delete it->second;
	}
	delete clusters;
}

void listar_documentos() {

    DIR* dir_pointer = opendir("Indices\ Archivos/");
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
		
		archivo.open(("Indices\ Archivos/" + nombre_archivo).c_str());		
		cout << "Documento " << nombre_archivo << ": ";
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
		cout << "Cluster " << nombre_cluster << ": ";

		getline(archivo, cantidad_terminos, ';');
        for(int i = 0; i < atoi( cantidad_terminos.c_str() ) - 1; i++){
			getline(archivo, nombre_archivo, ';');
			cout << nombre_archivo <<", ";
		}
		
		cout << "\n";
        
		archivo.close();
        
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
		return 0;
	}
	
	switch(argv[1][1]) {
		case 'd':
			if(args < 3) return mensaje_error();
			
			Parser *parser;
			map<string, map<unsigned int, float> >* vectores;
			int retorno = vectorizar_documentos(argv[2], parser, vectores);
			if (retorno != 0)
				return retorno;
			
			map<string, map<unsigned int, float> >* vectores_iniciales = obtener_muestra_vectores(vectores);
			unsigned int cant_terms = parser->getCantTerms();
			unsigned int cant_docs = parser->getCantDocs();
			delete parser;
			
			map<string,Cluster*>* clusters
			
			if(strcmp(argv[3], "-c") == 0){
				if(args < 7) return mensaje_error();
				
				if(strcmp(argv[5], "-o") != 0) return mensaje_error();
				
				if(strcmp(argv[6], "Y") == 0)
					clusters = k_means(vectores_iniciales,atoi(argv[4]),cant_terms,0.99,0.8);
				else 
					clusters = k_means(vectores_iniciales,atoi(argv[4]),cant_terms,0.99,1);
					
				
				delete vectores_iniciales;
			
				agregado_resto_de_vectores(clusters, vectores)				
				escribir_clusters_en_disco(clusters);
				
				cout << "Cantidad de archivos: "<< cant_docs << endl;
				cout << "Cantidad de terminos: "<< cant_terms << endl;
				int t_fin = time(NULL);
				cout << "Tardo: " << t_fin - t_inicio << " segundos" << endl;
				vectorizar_documentos(argv[2]);	
				cout << "Vectorizo los documentos\n";
				
			}else{
				if(strcmp(argv[3], "-o") != 0) return mensaje_error();
				
				if(args < 5) return mensaje_error();
				
				if(strcmp(argv[6], "Y") == 0)
					//Jerarquico
				else 
					//Jerarquico
			}
			Indice* indice = new Indice("Clusters");
			delete(indice);
			
			break;
			
		case 'l':
			listar_documentos();
			break;
			
		case 'g':
			listar_clusters();
			break;
			
		//case: "-a"
	}
	
	
	return 0;
}
