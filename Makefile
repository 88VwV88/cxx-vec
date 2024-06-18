CXX := clang++
CXXFLAGS = -std=c++23

DEBUG := -g -pedantic -O0
RELEASE := -O2

BUILD ?= DEBUG

BIN := cxx

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp, out/%.o, $(SRC))

out/%.o: src/%.cpp
	@echo [COMPILE] $@
	@$(CXX) $(CXXFLAGS) $($(BUILD)) -c $< -o $@

build: $(SRC) $(OBJ)
	@echo [SOURCES] $(SRC) 
	@echo [OBJECTS] $(OBJ)
	@echo [FLAGS] $(CXXFLAGS) $($(BUILD))
	@$(CXX) $(CXXFLAGS) $($(BUILD)) $(SRC) -o $(BIN)
	@echo [BUILD SUCCESSFUL]

test: build
	@./cxx
release: BUILD = RELEASE
release: build
clean:
	rm -f out/*.o ./$(BIN)
.PHONY: build test clean debug release