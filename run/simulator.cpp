
#include <sys/stat.h>
#include <sys/types.h>

#include <functional>
#include <regex>
#include <sstream>
#include <tuple>
#include <vector>
#include<ctime>

#include "../cpp/room.h"
#include "../cpp/utils.h"
#include "threadpool.h"
using namespace std;

//
class Simulator {
   public:
    virtual void install_model(){};
    virtual void simulate(){};
};
class SecondNuclear : public Simulator {
   public:
    SecondNuclear() = default;

    virtual void install_model(Room &room) {
        for (int i = 0; i < room.shape[1]; i++) {
            room.input_one_ECC(vec{0, i, 8}, 16, 2, vector<int>(16, 1), 0);
        }
        for (int i = 2; i < room.shape[0] - 1; i++) {
            for (int j = 0; j < room.shape[1] - 1; j += 2) {
                room.input_one_FCC(vec{i, j, 0}, 128, 2, 1, vector<int>(128, 1),
                                   1);
            }
        }
    }
    virtual void simulate(tuple<double, double, double> param) {
        auto p1 = get<0>(param);
        auto p2 = get<1>(param);
        auto p3 = get<2>(param);

        vector<vector<double>> Ep_matrix = {{p1, 0, 0}, {0, 0, 0}, {0, 0, 0}};
        vector<vector<double>> Ee2e_matrix = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

        vector<vector<double>> Eb_matrix = {{p2, 0, p3}, {0, 0, 0}, {0, 0, 0}};

        Room room(64, 64, 64, Ep_matrix, Eb_matrix, Ee2e_matrix, 24);
        cout << "installing" << endl;
        install_model(room);
        cout << "preheat" << endl;
        string path;
        int a = 0;
        stringstream stream;
        stream << "./data/"
               << "f" << p1 << p2 << p3;
        stream >> path;
        room.save(path);
        room.preheat(10000, 1000);
        room.movie(50000, 1000, 10, path);
    }
    vector<tuple<double, double, double>> parameters() {
        auto b = Cartesian_product(vector<double>{1, 2}, vector<double>{3, 4},
                                   vector<double>{1, 2});
        return b;
    }
};
class ExtendedChainCrystal : public Simulator {
   public:
    virtual void install_model(Room &room) {
        for (int i = 0; i < room.shape[0]; i++) {
            for (int j = 0; j < room.shape[1]; j++) {
                if (j % 2 == 0 || i % 2 == 0 || (i + j) % 8 == 0 ||
                    (i - j) % 8 == 0) {
                    for (int k = 0; k < int(9 * room.shape[2] / 10); k += 2) {
                        room.input_one_ECC(
                            vec{i, j, k + 1}, 2, 2,
                            vector<int>{2, 2}, 1);
                    }
                } else {
                    room.input_one_ECC(vec{i, j, 1}, room.shape[2] - 4, 2,
                                       vector<int>(room.shape[2] - 4, 1), 1);
                }
            }
        }
    }
    virtual void simulate(tuple<double,double, double, int, double> param) {
        // auto [Ep2, Ee2e,length, T]= param;
        auto Ep2 = get<0>(param);
        auto Ep12=get<1>(param);
        auto Ee2e = get<2>(param);
        auto length = get<3>(param);
        auto T = get<4>(param);
        stringstream path;
        time_t now= time(0);
        auto ltm=localtime(&now);
        path << "./data/";
        path<<1900 + ltm->tm_year<<"-"<<1 + ltm->tm_mon<<"-"<<ltm->tm_mday<<"/";
        
        string path_date;
        path>>path_date;
        mkdir(path_date.c_str(), 00744);
        path<<path_date<<"0.9loadEp2=" << Ep2 <<"Ep12="<<Ep12<< "Ee2e=" << Ee2e << "T=" << T << '/';
        string spath;
        path >> spath;
        mkdir(spath.c_str(), 00744);
        freopen((spath+string("out.log")).c_str(), "w", stdout);
        printf("Runing task Ep12=%f,Ep=%f ,Ee2e=%f,T=%f,length=%d\n",Ep12, Ep2, Ee2e, T, length );
        Room room(length , length , length ,
                  vector<vector<double>>{{0, 0, 0}, {0, 0,Ep12}, {0,Ep12, Ep2}},
                  vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
                  vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, Ee2e}},
                  24);
        install_model(room);
        
        cout << "installed" << endl;
        cout << "preheating" << endl;
        room.save(spath+string("init"));
        room.preheat(50000, 1000);
        cout << "preheated" << endl;
        room.movie(200000, 10, T, spath);
    }
    auto parameters() -> vector<tuple<double, double ,double, int, double>> {
        auto Ep2 = vector<double>{1};
        auto Ep12 = vector<double>{1,2,4};
        auto Ee2e = vector<double>{5, 10, 20};
        auto length = vector<int>{32};
        auto T = range(1, 6.0, 0.3);
        return Cartesian_product(Ep2,Ep12, Ee2e, length, T);
    }
    void inputECC_with_small(Room &room) {
        int num = 0;
        for (int i = 0; i < room.shape[0]; i++) {
            for (int j = 0; j < room.shape[1]; j++) {
                if (j % 2 == 0 || i % 2 == 0 || (i + j) % 8 == 0 ||
                    (i - j) % 8 == 0) {
                    for (int k = 0; k < 3 * room.shape[2] / 4; k += 2) {
                        room.input_one_ECC(
                            vec{i, j, k + int(room.shape[2] / 8)}, 2, 2,
                            vector<int>(2, 2), 0);
                        num += 1;
                    }
                } else {
                    room.input_one_ECC(vec{i, j, 1}, room.shape[2] - 5, 2,
                                       vector<int>(room.shape[2] - 5, 1), 0);
                    num += 1;
                }
            }
        }
    }
};
