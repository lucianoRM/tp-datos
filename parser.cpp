#include <iostream>
#include <fstream>
#include <map>
#include <string>

using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;


#define MINIMA_LONG_TERMINO 2



void agregar_termino_a_hash(string termino, map<string,unsigned int> *hash){
	unsigned int frecuencia_termino = (*hash)[termino];
	if(frecuencia_termino)
		(*hash)[termino]++;
	else (*hash)[termino] = 1;
}


/*Recibe una linea(string), la parsea y carga los terminos a los hashes.*/
void cargar_terminos(string linea,map<string,unsigned int> *hash){
	int pos_actual;
	int size_linea = linea.size();
	string termino;
	char letra_actual;
	for(pos_actual = 0;pos_actual < size_linea;pos_actual++){
		letra_actual = linea[pos_actual]; //para optimizar la lectura, lee el caracter una vez, nada mas.
		if(letra_actual == ' ' || letra_actual == '.' || letra_actual == ',' || letra_actual == '\r'){
			if(termino.size() > MINIMA_LONG_TERMINO)
				agregar_termino_a_hash(termino,hash);
			termino = "";			
		}else
			termino +=letra_actual;	
	}
	if(termino.size() > MINIMA_LONG_TERMINO)
		agregar_termino_a_hash(termino,hash); //para la ultima palabra, porque sino no se agrega
	
}




int main(void){
	char nombre_archivo[] = "Libros/passing_of_the_third_floor_back.txt";
	ifstream archivo;
	archivo.open(nombre_archivo);
	string linea;
	map<string,unsigned int>::iterator it;
	map<string,unsigned int> hash_archivo;
	while(getline(archivo,linea))
		cargar_terminos(linea,&hash_archivo);
	for(it = hash_archivo.begin(); it!= hash_archivo.end(); it++)
			cout << it->first << "," << it->second << endl;	
	archivo.close();
	return 0;
}	




