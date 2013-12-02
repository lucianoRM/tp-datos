#ifndef HIERARCHICAL_H
#define HIERARCHICAL_H

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include "cluster/cluster.h"
#include <cmath>
#include <sstream>


std::vector<Cluster*>* hierarchical(std::map<std::string,std::map<unsigned int,float> >* vectores,unsigned int cant_terms,float cota);

void agregado_resto_de_vectores_HN(std::vector<Cluster*>* clusters, std::map<std::string, std::map<unsigned int, float> >* vectores);

void agregado_resto_de_vectores_HY(std::vector<Cluster*>* clusters, std::map<std::string, std::map<unsigned int, float> >* vectores);

#endif //HIERARCHICAL_H
