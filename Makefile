
#Project Builder
#General
G++ = g++ 
CC = gcc
FLAGS = -Wall -Wextra -g -pedantic 


all: stopwords.o porter.o parser.o dir_seeker.o


dir_seeker.o: dir_seeker.cpp

	$(G++) $(FLAGS) porter/porter.o stopwords/stopwords.o parser.o dir_seeker.cpp -o dir_seeker.o -lstdc++


parser.o: parser.cpp
	
	$(G++) $(FLAGS) -c parser.cpp -o parser.o


stopwords.o: stopwords/stopwords.cpp 

	$(G++) $(FLAGS) -c stopwords/stopwords.cpp -o stopwords/stopwords.o

porter.o: porter/porter.c
	
	$(CC) $(FLAGS) -Wno-write-strings -c porter/porter.c -o porter/porter.o -lstdc++
	
		
#Cleanup
clean:
	rm *.o 
	
