CXX := g++
CXXFLAGS = -std=c++23

DEBUG := -g -Wall -Wpedantic -Werror -O0
RELEASE := -O2

BUILD ?= DEBUG

BIN := cxx

SRC := $(wildcard src/*.cc)
OBJ := $(patsubst src/%.cc, out/%.o, $(SRC))

out/%.o: src/%.cc
	@$(CXX) $(CXXFLAGS) $($(BUILD)) -c $< -o $@
	@echo [COMPILED] $<

build: $(SRC) $(OBJ)
	@echo [FLAGS] $(CXXFLAGS) $($(BUILD))
	@$(CXX) $(CXXFLAGS) $($(BUILD)) $(OBJ) -o $(BIN)
	@echo [BUILD SUCCESSFUL]

release: BUILD = RELEASE
release: build
clean:
	rm -f out/*.o ./$(BIN)
.PHONY: build clean debug release