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
			unsigned int cant_terms = parser->getCantTerms();
			unsigned int cant_docs = parser->getCantDocs();
			delete parser;
			map<string, map<unsigned int, float> >* vectores_aux = new map<string, map<unsigned int, float> >;
			map<string, map<unsigned int, float> >::iterator it2;
			unsigned int l = 0;
			unsigned int muestra = cant_docs * 0.3;
			cout << "size" << vectores->size()  << " muestra "<< muestra <<endl;
			for(it2 = vectores->begin(); it2 != vectores->end();++it2){ 
				if(l > muestra) break;
				(*vectores_aux)[it2->first] = it2->second;
				vectores->erase(it2);
				l++;
			}
			
			string mas_cercano;
			vector<string> mas_cercanos;
			/*Para que pueda entrar el mismo archivo en varios clusters poner el ultimo parametro en 1*/
			map<string,Cluster*>* clusters = k_means(vectores_aux,atoi(argv[3]),cant_terms,0.99,0.8);
			delete vectores_aux;
			//vector<Cluster*>* clusters = hierarchical(vectores,cant_terms,0.5);
			
			//SE USA SI EL PARAMETRO ES 1(COTA)
			/*for(it2 = vectores->begin();it2 != vectores->end();++it2){
				mas_cercano = distancia_minima_key(clusters,&it2->second,calcular_norma(it2->second));
				(*clusters)[mas_cercano]->agregar_vector(&it2->second,it2->first);
				vectores->erase(it2);
			}*/
			//SE USA SI EL PARAMETRO ES MENOR A 1 (COTA)
			for(it2 = vectores->begin();it2 != vectores->end();++it2){
				mas_cercanos = min_distances(clusters,&it2->second,calcular_norma(it2->second),0.8);
				if(mas_cercanos.size() == 0) mas_cercanos.push_back(distancia_minima_key(clusters,&it2->second,calcular_norma(it2->second)));
				for(unsigned int h = 0; h<mas_cercanos.size();h++){
					(*clusters)[mas_cercanos[h]]->agregar_vector(&it2->second,it2->first);
					cout << mas_cercanos[h] << endl;
				}
				vectores->erase(it2);
			}
			/*vector<Cluster*>::iterator it;
			for(it = clusters->begin(); it!= clusters->end() ; ++it){
					cout <<(*it)->get_id() << endl << endl << (*it)->get_docs() << endl;
					cout << "--------------------------------------------------------------" << endl;
					delete (*it);
			}	*/	
					
			map<string,Cluster*>::iterator it;
			for(it = clusters->begin();it != clusters->end();++it){
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
