pypath=/home/centos/anaconda3/envs/py3_5
boostpath=/home/zy/package/boost_1_68_0

crystal.so:polymer.o crystal.o ConnectedComponent.o
	g++ -O3 -shared -fPIC ConnectedComponent.o polymer.o crystal.o -o crystal.so -L $(pypath)/lib -L $(boostpath)/stage/lib  -lpython3.5m  -lboost_python35

polymer.o: ConnectedComponent.o
	g++ -O3 -c -fPIC -I$(pypath)/include/python3.5m -I$(boostpath) polymer.cpp -o polymer.o -std=c++11

crystal.o: polymer.o ConnectedComponent.o
	g++ -O3 -c -fPIC -I$(pypath)/include/python3.5m -I$(boostpath)  crystal.cpp -o crystal.o -std=c++11
ConnectedComponent.o:
	g++ -O3 -c -fPIC -I$(pypath)/include/python3.5m -I$(boostpath)  ConnectedComponent.cpp -o ConnectedComponent.o -std=c++11

clean: 
	rm -rf polymer.o crystal.o ConnectedComponent.o


