INCLUDE_PATH = -I ../Common

main: main.o
	g++ -std=c++11 -o main ../Common/Client.o main.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp $(INCLUDE_PATH) -o main.o
	
clean:
	rm -rf *.o *~
