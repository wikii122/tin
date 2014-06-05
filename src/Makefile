# 
# Makefile for project
# Generates two executables - tin_client and server
#

CC_FLAGS = -g -Wall -std=c++11 -c -I/usr/include -I.
LN_FLAGS = -g -Wall -std=c++11 -L/usr/lib -lboost_program_options -ljsoncpp -lboost_filesystem -lboost_system

CLIENT_CPP_FILES := $(wildcard client/*.cpp)
CLIENT_OBJ_FILES := $(CLIENT_CPP_FILES:.cpp=.o)

SERVER_CPP_FILES := $(wildcard *.cpp)
SERVER_CPP_FILES += $(wildcard files/*.cpp)
SERVER_CPP_FILES += $(wildcard packet/*.cpp)
SERVER_OBJ_FILES := $(SERVER_CPP_FILES:.cpp=.o)


DEPENDENCIES := $(CLIENT_CPP_FILES:.cpp=.d)
DEPENDENCIES += $(SERVER_CPP_FILES:.cpp=.d)

all: tin_client server   
server: $(SERVER_OBJ_FILES) 
	g++ -o $@ $^ $(LN_FLAGS)
tin_client: $(CLIENT_OBJ_FILES)	
	g++ -o $@ $^ $(LN_FLAGS)

%.o: %.cpp  
	g++ $(CC_FLAGS) -o $@ -MMD -MP -MF ${@:.o=.d} $<

-include $(DEPENDENCIES)

clean: ready
	rm -f *.o */*.o
	rm -f *.d */*.d
	rm -f tin_client server

ready:
	rm -f tin.socket lock.file
	rm -rf file_store
