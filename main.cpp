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


void help(){

	cout << "Uso:\n\nCreacion de Clusters:\n\n" << NOMBRE_TP << " -d <path> -c <#clusters> -o <Y/N>\n\n-d indica el path a donde están almacenados los documentos.\n-c indica la cantidad de categorias a crear, si no se indica se decide automaticamente automaticamente.\n-o indica si un documento puede estar en mas de un grupo.\n\nListado de resultados:\n\n" << NOMBRE_TP	 << " -l\nLista todos los documentos del repositorio y la categoría a la cual pertenece cada uno.\n\n" << NOMBRE_TP << " -g\nLista los grupos o categorías existentes y los documentos dentro de cada grupo o categoría.\n\nAgregado de archivos:\n\n" << NOMBRE_TP << " -a <path>\nAgrega y clasifica el texto pasado como parametro e indica a que grupo lo ha agregado."; 

}

int main(int args,char* argv[]){
	
	
	if (args == 1){
		help();
		return 0;
	}else{
		if(strcmp(argv[1],"-d") == 0){
			if(args < 3){
				help();
				return 1;
			}
			//int i;
			int t_inicio = time(NULL);
			Parser* parser =new Parser();
			int retorno = parsear_archivos(argv[2],parser);
			if (retorno != 0){
				cout << "No existe directorio\n";
				delete parser;
				return retorno;
			}
			guardar_frecuencias(parser->getFrecuenciasGlobales(), parser->getFrecuenciasLocales());	
			map<string, map<unsigned int, float> >* vectores = vectorizar(parser);
			map<string, map<unsigned int, float> >* vectores_iniciales = obtener_muestra_vectores(vectores);
			unsigned int cant_terms = parser->getCantTerms();
			unsigned int cant_docs = parser->getCantDocs();
			delete parser;
			
				
			map<string,Cluster*>* clusters = k_means(vectores_iniciales,atoi(argv[4]),cant_terms,0.99,0.8);
			delete vectores_iniciales;
			
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
				/*archivo_cluster.open(("Clusters/" + mas_cercano).c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
				archivo_cluster << ";" << it_vectores2->first;
				archivo_cluster.close();*/
			}
			delete vectores;
			map<string,Cluster*>::iterator it;
			for(it = clusters->begin(); it!= clusters->end() ; ++it){
				it->second->escribir_a_disco();
			}
			delete clusters;
			
			
			cout << "Cantidad de archivos: "<< cant_docs << endl;
			cout << "Cantidad de terminos: "<< cant_terms << endl;
			int t_fin = time(NULL);
			cout << "Tardo: " << t_fin - t_inicio << " segundos" << endl;
			
		}
	}
	return 0;
}
