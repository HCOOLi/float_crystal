
#include<vector>
#include<functional>
#include<regex>
#include"../cpp/room.h"
#include"../cpp/utils.h"
#include <tuple>
#include "threadpool.h"
#include<sstream>
using namespace std;


//
class Simulator {
public:
	virtual void install_model(){};
    virtual void simulate(){};

};
class SecondNuclear :public Simulator {
    Room room;
public:
    SecondNuclear() = default;

    virtual void install_model(Room &room) {
        for (int i = 0; i < room.shape[1]; i++) {
            room.input_one_ECC(vec{0, i, 8}, 16, 2, vector<int>(16, 1), 1);
        }
        for (int i = 2; i < room.shape[0] - 1; i++) {
            for (int j = 0; j < room.shape[1] - 1; j += 2) {
                room.input_one_FCC(vec{i, j, 0}, 128, 2, 1,
                                   vector<int>(128,1), 0);
            }
        }
        
	}
    virtual void simulate(tuple<double,double,double> param){
        auto [p1,p2,p3]=param;

        vector<vector<double> > Ep_matrix={{p1,0,0},{0,0,0},{0,0,0}};

        vector<vector<double >> Eb_matrix={{p2,0,p3},{0,0,0},{0,0,0}};

        Room room(64, 64, 64, Ep_matrix, Eb_matrix, 24);
        cout << "installing" << endl;
        install_model(room);
        cout << "preheat" << endl;
        room.preheat(10000);
		room.movie(50000, 10000, 100);
		string path;int a=0;
        stringstream stream;
        stream<<"./data/"<<"f"<<p1<<p2<<p3;
        stream>>path;
        
		room.save(path);


    }
	vector<tuple<double,double,double>> parameters(){

	}

};
int main(int argc, char *argv[]) {
    int thread_number;
    if(argc>1){
         thread_number = atoi(argv[1]);
    }else{
        thread_number=12;
    }
    ThreadPool pool(thread_number);
    auto b = Cartesian_product(vector<double>{1, 2}, vector<double>{3, 4}, vector<double>{1, 2});
    for (auto p:b) {
        SecondNuclear S;
        pool.enqueue(bind(&SecondNuclear::simulate, S,p));
    }


}