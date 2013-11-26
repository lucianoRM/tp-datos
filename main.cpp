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
			cout << "Cantidad de archivos: "<< parser->getCantDocs() << endl;
			cout << "Cantidad de terminos: "<< parser->getCantTerms() << endl;
			int t_fin = time(NULL);
			cout << "Tardo: " << t_fin - t_inicio << " segundos" << endl;
			delete parser;
			delete vectores;
		}
	}
	return 0;
}
