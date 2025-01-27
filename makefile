TARGET := main
CC := g++

COMP_TYPE := debug
cflags.debug := -ggdb -pedantic-errors -Wall -Werror -Weffc++ -Wextra -Wconversion -Wsign-conversion -Wshadow
cflags.release := -O2 -DNDEBUG
CFLAGS := ${cflags.${COMP_TYPE}} -std=c++20

SRC := ./src/
BUILD := ./build/

HEADERS := $(wildcard $(SRC)*.h)
CFILES := $(notdir $(wildcard $(SRC)*.cpp))
OBJECTS := $(addprefix $(BUILD), $(CFILES:.cpp=.o))

$(TARGET): $(OBJECTS)
	@$(CC) $(LIB) $(OBJECTS) $(CFLAGS) -o $(BUILD)$@

build/%.o: src/%.cpp $(HEADERS)
	@$(CC) $(LIB) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@./build/main

clean:
	@-rm -f $(BUILD)*.o
	@-rm -f $(BUILD)$(TARGET)
	@echo "---- Build Cleaned ----"