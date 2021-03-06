#include <map>
#include <fstream>
#include <string>

#include "stopwords.h"

using std::map;
using std::ifstream;
using std::string;


/*Usa patron singleton. Crea es medio bardero de explicar, la ventaja es que podemos acceder a la misma instancia de diccionario de stopwords desde todo el programa. Lo unico que hay que hacer es map<string, short> dict2 = Stopwords::getInstance()->getMap(). Revisar test.cpp en la carpeta*/

Stopwords* Stopwords::instance = NULL;

Stopwords::Stopwords() {
	diccionario = map<string,short>();
	char nombre_archivo[] = "stopwords/stopwords.txt";
	ifstream archivo;
	archivo.open(nombre_archivo);
	string stopword;
	while(getline(archivo,stopword,',')) {
		diccionario[stopword] = 1;
	}
	archivo.close();
}

void Stopwords::destroy() {
	delete instance;

}

 
Stopwords* Stopwords::getInstance() {
	if (instance == NULL) {
		instance = new Stopwords ();
      	}
	return instance;
}
		
map<string,short> Stopwords::getMap(){
	return diccionario;
}
