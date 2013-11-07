#Project Builder
#General
G++ = g++
FLAGS = -Wall -Wextra -g -pedantic 


all: parser.o


parser.o: parser.cpp
	
	$(G++) $(FLAGS) parser.cpp -o parser.o

		
#Cleanup
clean:
	rm *.o 
	
