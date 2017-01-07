INCLUDE_PATH = -I ../Common

main: main.cpp
	g++ -std=c++11 -o main main.cpp $(INCLUDE_PATH) ../Common/Client.o -lpthread

clean:
	rm -rf *.o *~
