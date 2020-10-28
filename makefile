pypath=/home/centos/anaconda3/envs/py3_5
boostpath=/home/zy/package/boost_1_68_0

CXXFLAGS=-O3 -g -fPIC -std=c++11

dirlib:
	-mkdir ./lib
main:crystal.o polymer.o ConnectedComponent.o
	g++  $(CXXFLAGS) ./lib/ConnectedComponent.o ./run/simulator.cpp ./run/main.cpp ./lib/crystal.o ./lib/polymer.o -o main  -pthread 

crystal.so:polymer.o crystal.o ConnectedComponent.o
	g++ -shared -fPIC ./lib/ConnectedComponent.o ./lib/polymer.o./lib/ crystal.o -o ./python/crystal.so -L $(pypath)/lib -L $(boostpath)/stage/lib  -lpython3.5m  -lboost_python35

polymer.o: ConnectedComponent.o ./cpp/polymer.cpp
	g++ -c  $(CXXFLAGS) ./cpp/polymer.cpp -o ./lib/polymer.o 

crystal.o: polymer.o ConnectedComponent.o ./cpp/room.cpp
	g++ -c  $(CXXFLAGS) ./cpp/room.cpp -o ./lib/crystal.o 
ConnectedComponent.o: ./cpp/ConnectedComponent.cpp ./lib
	g++ -c $(CXXFLAGS) ./cpp/ConnectedComponent.cpp -o ./lib/ConnectedComponent.o 

clean: 
	-rm -f ./lib/*


