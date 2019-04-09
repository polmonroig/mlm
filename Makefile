all: objects link


STD = -std=c++17
BOOST = -lboost_filesystem -lboost_system
OPT = -O2

objects: main.cpp Manager/Manager.cpp
	g++ -c Manager/Manager.cpp main.cpp $(STD) $(BOOST) $(OPT)

link: main.o Manager.o
	g++ -o mlm Manager.o main.o $(BOOST)

clean:
	rm -fr project* *.o
