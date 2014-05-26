CC_FLAGS = -Wall -std=c++11 -c -I/usr/include
LN_FLAGS = -Wall -std=c++11 -L/usr/lib -lboost_program_options -ljsoncpp -lboost_filesystem -lboost_system

CLIENT_CPP_FILES = $(wildcard client/*.cpp)
CLIENT_OBJ_FILES = $(CLIENT_CPP_FILES:.cpp=.o)

all: cli.out
cli.out: $(CLIENT_OBJ_FILES)
	g++ -o $@ $^ $(LN_FLAGS)
client/%.o: client/%.cpp
	g++ $(CC_FLAGS) -o $@ $<

clean:
	rm **/*.o
