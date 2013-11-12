#ifndef STOPWORDS_H
#define STOPWORDS_H

#include <map>
#include <fstream>
#include <string>

class Stopwords {	
	private:
		static Stopwords* instance;
		std::map<std::string,short> diccionario;
 
	private: 
		Stopwords();
 
	public:
		static Stopwords* getInstance();
		std::map<std::string,short> getMap();
};

#endif // STOPWORDS_H 
