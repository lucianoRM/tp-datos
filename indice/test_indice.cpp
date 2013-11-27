#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include "indice.h"

using std::vector;
using std::cout;

int main(){
	Indice* indice = new Indice("Clusters");
	vector<string> cluster_A = indice->obtenerClustersDeTermino("A.txt");
	cout << "clusters de A: \n";
	for(int i = 0; i < cluster_A.size(); i++){
		cout << cluster_A[i] << "\n";
	}
	
	return 0;
}
