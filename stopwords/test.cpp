#include "stopwords.h"
#include "delimiters.h"
#include <map>
#include <string>
#include <iostream>

using std::string;
using std::map;
using std::cout;
using std::endl;

int main() {
	
	Stopwords* lala = Stopwords::getInstance();
	map<string, short> dict = lala->getMap();
	map<string, short>::iterator it;

	Delimiters* lelele = Delimiters::getInstance();
	map<string, short> dict2 = lelele->getMap();
	map<string, short>::iterator it2;
	
	for (it = dict.begin(); it != dict.end(); ++it) {
		cout << "Clave: " << it->first << " - Valor: " << it->second << endl;
	}

	cout << "Hasta aca stopwords" << endl;

	for (it2 = dict2.begin(); it2 != dict2.end(); ++it2) {
		cout << "Clave: " << it2->first << " - Valor: " << it2->second << endl;
	}
	return 0;
}
