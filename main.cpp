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
			/*map<string, map<unsigned int, float> >* vectores_iniciales = new map<string, map<unsigned int, float> >;
			map<string,map<unsigned int,float> >::iterator it_vectores;		
			for(it_vectores = vectores->begin(),i = 0;it_vectores != vectores->end() && i<100;++it_vectores,i++){
				(*vectores_iniciales)[it_vectores->first] = it_vectores->second;
			}*/
			unsigned int cant_terms = parser->getCantTerms();
			unsigned int cant_docs = parser->getCantDocs();
			delete parser;
			map<string,Cluster*>* clusters = k_means(vectores,atoi(argv[3]),cant_terms,0.99);
			delete vectores;
			/*string mas_cercano;
			map<string,map<unsigned int,float> >::iterator it_vectores2;
			for(it_vectores2 = vectores->begin();it_vectores2 != vectores->end();++it_vectores2){
				mas_cercano = distancia_minima(clusters,&it_vectores->second,calcular_norma(it_vectores->second));
				(*clusters)[mas_cercano]->agregar_vector(&it_vectores2->second,it_vectores2->first);
			}*/
			map<string,Cluster*>::iterator it;
			for(it = clusters->begin(); it!= clusters->end() ; ++it){
				if(it->second->get_cant_docs() != 0){
					ofstream salida(("Clusters/" + it->first).c_str());
					cout << it->first << endl << it->second->get_docs() << endl;
					salida << it->second->get_docs() << endl;
					salida.close();
				}
				delete it->second;
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
