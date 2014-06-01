CC_FLAGS = -g -Wall -std=c++11 -c -I/usr/include
LN_FLAGS = -g -Wall -std=c++11 -L/usr/lib -lboost_program_options -ljsoncpp -lboost_filesystem -lboost_system

CLIENT_CPP_FILES = $(wildcard client/*.cpp)
CLIENT_OBJ_FILES = $(CLIENT_CPP_FILES:.cpp=.o)

SERVER_CPP_FILES = $(wildcard *.cpp)
SERVER_OBJ_FILES = $(SERVER_CPP_FILES:.cpp=.o)

all: server tin_client 
server: $(SERVER_OBJ_FILES)
	g++ -o $@ $^ $(LN_FLAGS)
tin_client: $(CLIENT_OBJ_FILES)
	g++ -o $@ $^ $(LN_FLAGS)

%.o: %.cpp
	g++ $(CC_FLAGS) -o $@ $<

clean:
	rm **/*.o
	rm tin_client
