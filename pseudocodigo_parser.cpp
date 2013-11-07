#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

#define MINIMA_LONG_TERMINO 1; 

/*PARSEO(directorio,archivo de documentos)

Crear hash para terminos globales
Crear hash de stopwords
cantidad de documentos = 0
Para cada documento en el directorio:
		Crear hash para el documento actual
		cantidad_terminos = 0
		Para cada término del documento actual:
			Si el término no es un stopword:
				Si el término se encuentra en el hash del documento actual:
					Sumar uno a la frecuencia de ese termino dentro del hash del documento actual
				En cambio, si el término se encuentra en el hash para términos globales:
					Sumar uno a la frecuencia de ese termino dentro del hash para términos globales
					Agregar el término al hash del documento actual con una frecuencia igual a 1
				En cambio:
					Agregar el término al hash del documento actual con una frecuencia igual a 1
					Agregar el término al hash de términos globales con una frecuencia igual a 1
				cantidad_terminos += 1
				Si cantidad_terminos supera un numero N considerable:
					Pasar a siguiente documento
		Escribo documento actual con sus términos y frecuencias en "archivo de documentos"
		Destruyo hash para el documento actual
		cantidad de documentos += 1
Destruir hash de stopwords
Devolver hash de terminos globales y cantidad de documentos

---------------------------------------------------------------------------------------------

VECTORIZACIÓN(hash de frecuencias globales,archivo de documentos):

Crear nuevo hash que guarde la siguiente información de términos.
Para cada término del hash de frecuencia globales:
	frecuencia exacta = 1 + log(cantidad de documentos/frecuencia global del termino)
	Guardar en nuevo hash
Destruir hash de frecuencia globales
Para cada documento del archivo de documentos:
	Creamos counting filter del tamaño de cantidad de terminos con todos ceros
	Multiplicar las frecuencias de cada término por su frecuencia exacta que se encuentra en el nuevo hash
	Cada termino tendrá una posicion determinada por una funcion de hashing como para ubicar su frecuencia en el counting filter
	Guardamos el nombre del documento y su respectivo counting filter como una línea en el nuevo archivo de documentos
Destruir archivo de documentos viejo
Destruir hash de frecuencias globales

*/

void parsear_linea(string linea,map<string,unsigned int> *hash){
	int ultima_posicion = 0;
	int pos;
	int size = linea.size();
	string termino;
	int size_termino;
	int frecuencia_termino;
	int pos_actual = 0;
	int minima_long = MINIMA_LONG_TERMINO;
	for(pos = 0; pos<size; pos++){
		if(pos == size - 1) pos_actual = pos + 1;
		else pos_actual = pos;
		if(linea[pos] == ' ' || linea[pos] == '.' || pos_actual != pos){
			termino = linea.substr(ultima_posicion,(pos_actual - ultima_posicion));
			size_termino = termino.size();
			if(size_termino > minima_long){
				frecuencia_termino = (*hash)[termino];
				if(frecuencia_termino)
					(*hash)[termino]++;
				else (*hash)[termino] = 1;
			}	
			ultima_posicion = pos+1;
		}
	}
	
}

int main(void){
	char nombre_archivo[] = "ejemplo.txt";
	ifstream archivo;
	archivo.open(nombre_archivo);
	string linea;
	map<string,unsigned int>::iterator it;
	map<string,unsigned int> hash_archivo;
	char* line;
	while(getline(archivo,linea))
		parsear_linea(linea,&hash_archivo);
	for(it = hash_archivo.begin(); it!= hash_archivo.end(); it++)
			cout <<it->first << "," <<it->second << endl;	
	archivo.close();
	return 0;
}
