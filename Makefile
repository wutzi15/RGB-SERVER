CPP = g++
CPPFLAGS = -O2  -std=c++11
LD = g++
LDFLAGS =  
LIBS = -lboost_system -lboost_thread

all:main.cpp
	$(CPP) $(CPPFLAGS) -o main.o -c main.cpp
	$(CPP) $(CPPFLAGS) -o SerialCom.o -c SerialCom.cpp
	$(LD) $(LDFLAGS) -o rgbserver main.o SerialCom.o $(LIBS)

clean:
	rm -f main.a rgbserver

.PHONY: clean
