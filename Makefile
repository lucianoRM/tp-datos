
#Project Builder
#General
G++ = g++ 
CC = gcc
FLAGS = -Wall -Wextra -g -pedantic 


all: stopwords.o porter.o parser.o vectorizador.o dir_seeker.o


dir_seeker.o: dir_seeker.cpp

	$(G++) $(FLAGS) porter/porter.o parser/vectorizador.o stopwords/stopwords.o parser/parser.o dir_seeker.cpp -o dir_seeker.o 


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
	
	
	
#Cleanup
clean:
	rm *.o 
	rm stopwords/*.o
	rm porter/*.o
	rm cluster/*.o
	rm parser/*.o
