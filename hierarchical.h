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
#include "hierarchical.h"

std::vector<Cluster*>* hierarchical(std::map<std::string,std::map<unsigned int,float> >* vectores,unsigned int cant_terms,float cota);




#endif //HIERARCHICAL_H
