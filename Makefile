CPP = g++
CPPFLAGS = -O2  
LD = g++
LDFLAGS =  
LIBS = -lboost_system -lboost_thread

all:main.cpp
	$(CPP) $(CPPFLAGS) -o main.o -c main.cpp
	$(LD) $(LDFLAGS) -o rgbserver main.o $(LIBS)

clean:
	rm -f main.a rgbserver

.PHONY: clean
