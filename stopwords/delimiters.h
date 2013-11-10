#include <map>
#include <fstream>
#include <string>

class Delimiters {	
	private:
		static Delimiters* instance;
		std::map<std::string,short> diccionario;
 
	private: 
		Delimiters();
 
	public:
		static Delimiters* getInstance();
		std::map<std::string,short> getMap();
};
