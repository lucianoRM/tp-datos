#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "../stopwords/stopwords.h"
#include "../porter/porter.h"

class Parser{
	
	private:
		unsigned int cant_documentos;
		unsigned int cant_terminos;
		std::map<std::string,short> dicc_stopwords;
		std::map<std::string,unsigned int> hash_frecuencias_globales;
		std::map<std::string,std::map<std::string,unsigned int> > hash_frecuencias_locales;
		std::map<std::string,std::string> hash_apariciones_unicas;
	public:
		Parser();
		~Parser();

	public:
		void incrementarDocs();

	public:		
		/*Filtra el termino pasados por parametro y luego los agrega a los diccionarios*/
		void agregar_termino_a_hash(std::string termino,std::string nombre_archivo);
	public:
		/*Recibe una linea(string), la parsea y carga los terminos a los hashes.*/
		void cargar_terminos(std::string linea,std::string nombre_archivo);
	public:
		unsigned int getCantTerms();
	public:
		unsigned int getCantDocs();
	public:
		std::map<std::string,unsigned int>* getFrecuenciasGlobales();
	public:
		std::map<std::string,std::map<std::string,unsigned int> >* getFrecuenciasLocales();
	public:
		void filtrarAparicionesUnicas();
};

#endif //PARSER_H
