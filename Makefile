CXX = clang++
#Flags passed to the compiler:
#	-Wall -> enable common warnings 
#	-Wextra -> enable extra warnings
#	-Iinclude -> tells the compiler to look for header file in include/ directory
CXXFLAGS = -Wall -Wextra -Iinclude

#Finds all .c files inside a dir
SRC = $(wildcard src/*.cpp) main.cpp
# Converts all .c source files into .o object filennames 
OBJ = $(SRC:.cpp=.o)
# .o files are compiled pices of code before linking

# Create a binary / executable
BIN = build/http-server

#default 
all: $(BIN)

# Rule: how to build the binary
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o main.o $(BIN)
