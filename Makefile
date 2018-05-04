CC=gcc
CXX=g++
RM=rm -f

#Ruta a la libreria libcppsim. 
#MODIFICAR segun sea el caso.
INCLUDES=-I../../libcppsim-0.2.5/src/
LDFLAGS=-L../../libcppsim-0.2.5/src/
LDLIBS=-lm -lcppsim 

CXXFLAGS=-std=c++11 -Wall

BIN=sim
DIR_OBJ=objs

SRCS=$(wildcard *.cc)

OBJS=$(patsubst %.cc,$(DIR_OBJ)/%.o,$(SRCS))


all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDLIBS) $(INCLUDES)

$(DIR_OBJ)/%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LDLIBS) $(LDFLAGS) $(INCLUDES)

clean:
	$(RM) $(OBJS)
	$(RM) *~

distclean: clean
	$(RM) $(BIN)

.PHONY: all clean distclean
	