INCLUDE_PATH = -I ../Common
INCLUDE_PATH += -I ../Common/rapidjson/include

OBJECTS = LogicServer.o \
		  main.o

main: $(OBJECTS)
	g++ -std=c++11 -o main $(OBJECTS) ../Common/Client.o ../Common/BaseServer.o
	
main.o: main.cpp
	g++ -std=c++11 -c main.cpp
	
LogicServer.o: LogicServer.cpp
	g++ -std=c++11 -c LogicServer.cpp $(INCLUDE_PATH) -o LogicServer.o
	
clean:
	rm -rf *.o *~
