# Makefile for the minic project

PROJECT=minic
CXX := clang++-10
CXXFLAGS := -g -O0 -march=native
CC := clang-10
LEX := flex
LFLAGS= 
YACC := bison
YFLAGS=

$(PROJECT): $(PROJECT).yy.cxx $(PROJECT).tab.cxx ast.o
	$(CXX) $(CXXFLAGS) $(PROJECT).yy.cxx $(PROJECT).tab.cxx ast.o -o $(PROJECT)

$(PROJECT).yy.cxx: $(PROJECT).lxx $(PROJECT).tab.cxx $(PROJECT).tab.hxx
	$(LEX) $(LFLAGS) -o $(PROJECT).yy.cxx $(PROJECT).lxx

$(PROJECT).tab.cxx $(PROJECT).tab.hxx: $(PROJECT).yxx ast.hpp
	$(YACC) $(YFLAGS) -d $(PROJECT).yxx

ast.o: ast.cpp ast.hpp
	$(CXX) $(CXXFLAGS) -c ast.cpp -o ast.o

.PHONY: clean
clean:
	rm -f $(PROJECT) $(PROJECT).yy.cxx $(PROJECT).tab.hxx $(PROJECT).tab.cxx ast.o
