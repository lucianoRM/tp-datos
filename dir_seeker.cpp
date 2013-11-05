#include <iostream>
#include <fstream>
#include <dirent.h>



using namespace std;




/*Funcion que recorre un directorio e imprime los nombres de los archivos que se encuentran dentro de el*/
void imprimir_nombre_archivos(char* nombre_dir){
	DIR* dir_pointer = opendir(nombre_dir);
	struct dirent* reg_buffer = readdir(dir_pointer);
	while(reg_buffer != NULL){
		printf("%s\n",reg_buffer->d_name);
		reg_buffer = readdir(dir_pointer);
	}
	closedir(dir_pointer);
}

int main(){
	imprimir_nombre_archivos((char*)"Libros");
	return 0;
}



