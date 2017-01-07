INCLUDE_PATH = -I ../Common
INCLUDE_PATH += -I ../Common/rapidjson/include

OBJECTS = GateServer.o \
		  main.o
		  
main: $(OBJECTS)
	g++ -std=c++11 -o main $(OBJECTS) ../Common/Client.o ../Common/BaseServer.o 

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

GateServer.o: GateServer.cpp 
	g++ -std=c++11 -c GateServer.cpp $(INCLUDE_PATH) -o GateServer.o #-Wall -g -O2

clean:
	rm -rf *.o *~
