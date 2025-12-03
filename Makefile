COMPILER = clang++
COMPILER_FLAGS = -Wall -Wextra -I./include
# -Wall - enable common warnings
# -Wextra - enable extra warnings
# -I./include - tells the compiler where to find .h files

# dirs
OBJDIR = build/obj

# cpp files
CPP = $(wildcard src/*.cpp) main.cpp
# "src/file.cpp" -> "build/obj/src/file.o"
OBJ = $(addprefix $(OBJDIR)/, $(CPP:.cpp=.o))
# bin
BIN = build/http-server

#default
all: $(BIN)

# Links all .o files 
$(BIN): $(OBJ)
	$(COMPILER) $(COMPILER_FLAGS) -o $@ $^

# Ensure obj subdirectories exist and compile each file
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BIN)
