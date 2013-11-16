CPP = g++ 
CPPFLAGS = -O2 -std=c++11 
LD = g++
LDFLAGS = 
LIBS = -lboost_system

all:
	$(CPP) $(CPPFLAGS) -o main.o -c main.cpp
	$(LD) $(LDFLAGS) -o rgbserver main.o $(LIBS)

clean:
	rm -f main.a rgbserver

.PHONY: clean