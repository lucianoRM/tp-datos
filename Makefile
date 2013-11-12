
#Project Builder
#General
G++ = g++
FLAGS = -Wall -Wextra -g -pedantic 


all: stopwords.o  parser.o dir_seeker.o


dir_seeker.o: dir_seeker.cpp

	$(G++) $(FLAGS) stopwords/stopwords.o  parser.o dir_seeker.cpp -o dir_seeker.o


parser.o: parser.cpp
	
	$(G++) $(FLAGS)  -c parser.cpp -o parser.o


stopwords.o: stopwords/stopwords.cpp 

	$(G++) $(FLAGS) -c stopwords/stopwords.cpp -o stopwords/stopwords.o

		
#Cleanup
clean:
	rm *.o 
	
