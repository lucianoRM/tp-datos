#include <map>
#include <fstream>
#include <string>

#include "delimiters.h"

using std::map;
using std::ifstream;
using std::string;



/*Usa patron singleton. Crea es medio bardero de explicar, la ventaja es que podemos acceder a la misma instancia de diccionario de delimitadores desde todo el programa. Lo unico que hay que hacer es map<string, short> dict2 = Delimiters::getInstance()->getMap(). Revisar test.cpp en la carpeta*/

Delimiters* Delimiters::instance = NULL;

Delimiters::Delimiters() {
	diccionario = map<string,short>();
	char nombre_archivo[] = "stopwords/delimiters.txt";
	ifstream archivo;
	archivo.open(nombre_archivo);
	string delimiter;
	while(getline(archivo,delimiter,',')) {
		diccionario[delimiter] = 0;
	}
}
 
Delimiters* Delimiters::getInstance() {
	if (instance == NULL) {
		instance = new Delimiters ();
      	}
	return instance;
}
		
map<string,short> Delimiters::getMap(){
	return diccionario;
}
