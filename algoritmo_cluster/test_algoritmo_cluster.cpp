#include <iostream>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <map>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <utility>
#include "../cluster/cluster.h"
#include "algoritmo_cluster.h"
#include "agrupador.h"

#define UNDEFINED 0
#define MULTICLUSTER -1
#define COTA 0.8

using std::pair;
using std::map;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;
using std::make_pair;




int main(){
	cout << "ALGORITMO EN EL QUE SE AGRUPAN DOS CLUSTERS EN CADA PASADA SABIENDO CANTIDAD DE CLUSTERS FINALES" << endl;
	Agrupador* agrupador = new Agrupador(0,COTA,600,14);
	(*agrupador).generar_clusters();
	(*agrupador).destroy();
	delete agrupador;
	cout << endl << "ALGORITMO EN EL QUE SE AGRUPAN DOS O MAS CLUSTERS EN CADA PASADA SABIENDO CANTIDAD DE CLUSTERS FINALES" << endl;
	agrupador = new Agrupador(MULTICLUSTER,COTA,600,14);
	(*agrupador).generar_clusters();
	(*agrupador).destroy();
	delete agrupador;
	cout << "ALGORITMO EN EL QUE SE AGRUPAN DOS CLUSTERS EN CADA PASADA SIN SABER CANTIDAD DE CLUSTERS FINALES" << endl;
	agrupador = new Agrupador(0,COTA,600,UNDEFINED);
	(*agrupador).generar_clusters();
	(*agrupador).destroy();
	delete agrupador;
	cout << "ALGORITMO EN EL QUE SE AGRUPAN DOS O MÁS CLUSTERS EN CADA PASADA SIN SABER CANTIDAD DE CLUSTERS FINALES" << endl;
	agrupador = new Agrupador(MULTICLUSTER,COTA,600,UNDEFINED);
	(*agrupador).generar_clusters();
	(*agrupador).destroy();
	delete agrupador;	
	return 0;
}
