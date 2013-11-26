
#Project Builder
#General
G++ = g++ 
CC = gcc
FLAGS = -Wall -Wextra -g -pedantic 
EXEC = TpGrupo6



all: stopwords.o porter.o parser.o vectorizador.o dir_seeker.o cluster.o test_cluster.o main.o algoritmo_cluster.o test_algoritmo_cluster.o



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

test_cluster.o: cluster/test_cluster.cpp

	$(G++) $(FLAGS) cluster/cluster.o cluster/test_cluster.cpp -o cluster/test_cluster.o 
	
algoritmo_cluster.o: algoritmo_cluster/algoritmo_cluster.cpp
	$(G++) $(FLAGS) cluster/cluster.o -c algoritmo_cluster/algoritmo_cluster.cpp -o algoritmo_cluster/algoritmo_cluster.o

test_algoritmo_cluster.o: algoritmo_cluster/test_algoritmo_cluster.cpp

	$(G++) $(FLAGS) cluster/cluster.o algoritmo_cluster/algoritmo_cluster.o algoritmo_cluster/test_algoritmo_cluster.cpp -o algoritmo_cluster/test_algoritmo_cluster.o

vectorizador.o: parser/vectorizador.cpp

	$(G++) $(FLAGS) -c parser/vectorizador.cpp -o parser/vectorizador.o

main.o: main.cpp

	$(G++) $(FLAGS) porter/porter.o parser/vectorizador.o stopwords/stopwords.o parser/parser.o dir_seeker.o main.cpp -o $(EXEC)
	
	
#Cleanup
clean:
	rm $(EXEC)
	rm *.o 
	rm stopwords/*.o
	rm porter/*.o
	rm cluster/*.o
	rm algoritmo_cluster/*.o
	rm parser/*.o
