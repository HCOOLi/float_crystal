pypath=/home/centos/anaconda3/envs/py3_5
boostpath=/home/zy/package/boost_1_68_0

CXXFLAGS=-O3 -g -fPIC -std=c++11

target: main ureamain ICmain foldmain analyze

dirlib: ./lib
	-mkdir ./lib

main:crystal.o  ConnectedComponent.o
	g++  $(CXXFLAGS) ./lib/ConnectedComponent.o ./run/simulator.cpp ./run/main.cpp ./lib/crystal.o -o main  -pthread 

ureamain:crystal.o  ConnectedComponent.o
	g++  $(CXXFLAGS) ./lib/ConnectedComponent.o ./run/simulator.cpp ./run/ureamain.cpp ./lib/crystal.o -o ureamain  -pthread 

ICmain:crystal.o  ConnectedComponent.o
	g++  $(CXXFLAGS) ./lib/ConnectedComponent.o ./run/ICsimulator.cpp ./run/ICmain.cpp ./lib/crystal.o -o ICmain  -pthread 

foldmain:crystal.o  ConnectedComponent.o
	g++  $(CXXFLAGS) ./lib/ConnectedComponent.o ./run/foldsimulator.cpp ./run/foldmain.cpp ./lib/crystal.o -o foldmain  -pthread 

analyze:crystal.o  ConnectedComponent.o
	g++  $(CXXFLAGS) ./lib/ConnectedComponent.o  ./analyze/analyze.cpp ./lib/crystal.o -o analy  -pthread 

crystal.so:crystal.o ConnectedComponent.o
	g++ -shared -fPIC ./lib/ConnectedComponent.o  ./lib/ crystal.o -o ./python/crystal.so -L $(pypath)/lib -L $(boostpath)/stage/lib  -lpython3.5m  -lboost_python35

crystal.o: ConnectedComponent.o ./cpp/room.cpp
	g++ -c  $(CXXFLAGS) ./cpp/room.cpp -o ./lib/crystal.o 
	
ConnectedComponent.o:  ./lib
	g++ -c $(CXXFLAGS) ./cpp/ConnectedComponent.cpp -o ./lib/ConnectedComponent.o 

clean: 
	-rm -f ./lib/*


