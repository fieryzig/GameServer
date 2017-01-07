INCLUDE_PATH = -I ./rapidjson/include
INCLUDE_PATH += -I ./utils

#OBJECTS = BaseServer.o Client.o

#$(OBJECTS): 

all:BaseServer.o Client.o


BaseServer.o: ./BaseServer/BaseServer.cpp ./BaseServer/BaseServer.h 
	g++ -std=c++11 -c ./BaseServer/BaseServer.cpp $(INCLUDE_PATH)
	
Client.o: ./Client/Client.cpp ./Client/Client.h
	g++ -std=c++11 -c ./Client/Client.cpp
	
clean:
	rm -rf *.o *~
	
