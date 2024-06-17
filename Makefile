CXX := clang++
CXXFLAGS = -std=c++23

DEBUG := -g -pedantic -O0
RELEASE := -O2

BUILD ?= DEBUG

BIN := cxx

SRC := $(wildcard src/*.cpp)

build: $(SRC)
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