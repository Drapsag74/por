CXXFLAGS=`pkg-config fflas-ffpack --cflags`
LDFLAGS=`pkg-config fflas-ffpack --libs`
CXXFLAGS+=-O2 -march=native
EXEC = por
SRC=$(wildcard *.C)
OBJ=$(SRC:.C=.o)

all: $(EXEC)

por: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.C Client.h Server.h
	$(CXX) -c $(CXXFLAGS) $< -o main.o $(LDFLAGS)
	
%.o:%.C
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f *.o por

#main: main.C Client.h Server.h
#	$(CXX) $(CXXFLAGS) $main.C -o $@ $^

