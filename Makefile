all: objects link


STD = -std=c++17
BOOST = -lboost_filesystem -lboost_system

objects: main.cpp Manager/Manager.cpp
	g++ -c Manager/Manager.cpp main.cpp $(STD) $(BOOST)

link: main.o Manager.o
	g++ -o ml_manager Manager.o main.o $(STD) $(BOOST)

clean:
	rm -fr project* *.o
