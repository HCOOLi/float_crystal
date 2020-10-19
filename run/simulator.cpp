#pragma once
#include<vector>
#include<functional>
#include<regex>
#include"room.h"
#include <tuple>
#include "../cpp/threadpool.h"
using namespace std;

template<class First, class Second>
auto Cartesian_product(vector<First> F, vector<Second> S) -> vector<tuple<First, Second >> {
    vector<tuple<First, Second >> result;
    for (auto f:F) {
        for (const auto &s:S) {
            result.push_back(std::make_tuple(f, s));
        }
    }
    return result;
}

template<class First, class... Args>
auto Cartesian_product(vector<First> F, vector<Args> ... rests) -> vector<tuple<First, Args ... >> {
    vector<tuple<First, Args ... >> result;
    auto rest_results = Cartesian_product(rests...);
    for (auto f:F) {
        for (const auto &r:rest_results) {
            result.push_back(tuple_cat(std::make_tuple(f), r));
        }
    }
    return result;

}

//
class Simulator {
public:
	void install_model()=0;
    virtual void simulate()=0;

};
class SecondNuclear :public Simulator {
public:
    SecondNuclear() = default;

    void install_model(Room &room) {
        for (int i = 0; i < room.shape[1]; i++) {
            room.input_one_ECC(vec{0, i, 8}, 16, 2, vector<int>(16, 1), 1);
        }
        for (int i = 2; i < room.shape[0] - 1; i++) {
            for (int j = 0; j < room.shape[1] - 1; j += 2) {
                room.input_one_FCC(vec{i, j, 0}, 128, 2, 1,
                                   vector<int>(1, 128), 0);
            }
        }
        
	}
    void simulate(tuple<double,double,double> parameters) override {

        vector<vector<double> > Ep_matrix={{},{},{}};

        vector<vector<double >> Eb_matrix={{},{},{}};

        Room room(64, 64, 64, Ep_matrix, Eb_matrix, 24);
        cout << "installing" << endl;
        install_model(room);
        cout << "preheat" << endl;
        room.preheat(10000);
		room.movie(5000000, 100000, 100);
		string path=sprintf("Data/no-ECCheated%3.2f-d.json",Ep*T)
		room.save(path);


    }
	vector<tuple<double,double,double>> parameters(){

	}


int main(int argc, char *argv[]) {
    int thread_number = atoi(argv[1]);
    ThreadPool pool(thread_number);
    auto b = Cartesian_product(vector<int>{1, 2}, vector<int>{3, 4}, vector<int>{1, 2});
    for (auto p:b) {
        SecondNuclear S;
		S.install_model();
        pool.enqueue(mem_fn(&SecondNuclear::simulate, *S,p));
    }


}