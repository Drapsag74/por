CXXFLAGS=`pkg-config givaro --cflags`
CXXFLAGS+=`pkg-config fflas-ffpack --cflags`
LDFLAGS=`pkg-config givaro --libs`
LDFLAGS+=`pkg-config fflas-ffpack --libs`
LDFLAGS+= -lssl -lcrypto
CXXFLAGS+= -O2 -march=native
EXEC = por
SRC=$(wildcard *.C)
OBJ=$(SRC:.C=.o)

all: $(EXEC)

por: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.C Client.h Server.h naccache_stern.h
	$(CXX) -c $(CXXFLAGS) $< -o main.o $(LDFLAGS)
	
%.o:%.C
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f *.o por

#main: main.C Client.h Server.h
#	$(CXX) $(CXXFLAGS) $main.C -o $@ $^

