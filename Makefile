all : exec

exec : main.o Cellule.o Envir.o
	g++ -g main.o Cellule.o Envir.o -o exec  -Wall -Wextra -std=c++11

main.o : main.cpp Cellule.h
	g++ -g -c main.cpp -o main.o  -Wall -Wextra -std=c++11
	
Cellule.o : Cellule.cpp Cellule.h
	g++ -g -c Cellule.cpp -o Cellule.o -Wall -Wextra -std=c++11 
	
Envir.o: Envir.cpp Envir.h Cellule.h
	g++ -g -c Envir.cpp -o Envir.o -Wall -Wextra -std=c++11
	
clean :
	rm *.o
