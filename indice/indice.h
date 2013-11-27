#ifndef INDICE_H
#define INDICE_H

#include <map>
#include <fstream>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class Indice
{
    private:
         map < string, vector<string> > indice;
         
    public:
        Indice(string nombre);
        vector<string> obtenerClustersDeTermino(string termino);
        virtual ~Indice();   
};

#endif // INDICE_H
