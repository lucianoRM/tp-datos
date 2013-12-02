
#Project Builder
#General
G++ = g++ 
CC = gcc
FLAGS = -Wall -Wextra -g -pedantic -O3 
EXEC = TpGrupo6



all: stopwords.o porter.o parser.o vectorizador.o dir_seeker.o cluster.o kmeans.o hierarchical.o indice.o main.o 



dir_seeker.o: dir_seeker.cpp

	$(G++) $(FLAGS) -c dir_seeker.cpp -o dir_seeker.o 

parser.o: parser/parser.cpp
	
	$(G++) $(FLAGS) -c parser/parser.cpp -o parser/parser.o


stopwords.o: stopwords/stopwords.cpp 

	$(G++) $(FLAGS) -c stopwords/stopwords.cpp -o stopwords/stopwords.o

porter.o: porter/porter.c
	
	$(CC) $(FLAGS) -Wno-write-strings -c porter/porter.c -o porter/porter.o -lstdc++

cluster.o: cluster/cluster.cpp 

	$(G++) $(FLAGS) -c cluster/cluster.cpp -o cluster/cluster.o



vectorizador.o: parser/vectorizador.cpp

	$(G++) $(FLAGS) -c parser/vectorizador.cpp -o parser/vectorizador.o

indice.o: indice/indice.cpp

	$(G++) $(FLAGS) -c indice/indice.cpp -o indice/indice.o 

main.o: main.cpp

	$(G++) $(FLAGS) porter/porter.o parser/vectorizador.o stopwords/stopwords.o parser/parser.o cluster/cluster.o kmeans.o hierarchical.o dir_seeker.o indice/indice.o main.cpp -o $(EXEC)
	
kmeans.o: kmeans.cpp
		
	$(G++) $(FLAGS) -c	kmeans.cpp -o kmeans.o
	

hierarchical.o: hierarchical.cpp
	
	$(G++) $(FLAGS) -c hierarchical.cpp -o hierarchical.o
	
#Cleanup
clean:
	rm $(EXEC)
	rm *.o 
	rm stopwords/*.o
	rm porter/*.o
	rm cluster/*.o
	rm indice/*.o
	rm parser/*.o
