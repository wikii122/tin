# Warning!
# This makefile is not watching for changes in headers.
# clean up after changing them or remove dependant .o manually.
# TODO: fix this.
CC_FLAGS = -g -Wall -std=c++11 -c -I/usr/include
LN_FLAGS = -g -Wall -std=c++11 -L/usr/lib -lboost_program_options -ljsoncpp -lboost_filesystem -lboost_system

CLIENT_CPP_FILES = $(wildcard client/*.cpp)
CLIENT_OBJ_FILES = $(CLIENT_CPP_FILES:.cpp=.o)

SERVER_CPP_FILES = $(wildcard *.cpp)
SERVER_OBJ_FILES = $(SERVER_CPP_FILES:.cpp=.o)

all: tin_client server ready  
	@echo "Done"
server: $(SERVER_OBJ_FILES) 
	g++ -o $@ $^ $(LN_FLAGS)
tin_client: $(CLIENT_OBJ_FILES)	
	g++ -o $@ $^ $(LN_FLAGS)

%.o: %.cpp  
	g++ $(CC_FLAGS) -o $@ $<

clean: ready
	rm -f *.o */*.o
	rm -f tin_client server

ready:
	rm -f tin.socket lock.file
