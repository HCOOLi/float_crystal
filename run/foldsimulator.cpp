#include <sys/stat.h>
#include <sys/types.h>

#include <ctime>
#include <functional>
#include <sstream>
#include <tuple>
#include <vector>

#include "../cpp/room.h"
#include "../cpp/utils.h"
using namespace std;

class FoldChainCrystal {
 public:
  virtual void install_model(Room &room, int nums) {
    int count = 0;
    for (int i = 0; i < room.shape[0]; i++) {
      for (int j = 0; j < room.shape[1]; j++) {
        if (j % 2 == 0 || i % 2 == 0 || count >= nums) {
          for (int k = 0; k < int(9 * room.shape[2] / 10); k += 2) {
            room.input_one_ECC(vec{i, j, k + 1}, 2, 2, vector<int>{2, 2}, 1);
          }
        } else {
          room.input_one_FCC(vec{i, j, 1}, room.shape[2] - 3, 2, 2,
                             vector<int>(room.shape[2] - 3, 1), 1);
          count++;
        }
      }
    }
  }

  virtual void simulate(tuple<double, double, double, int> param, double T,
                        int nums) {
    // auto [Ep2, Ee2e,length, T]= param;
    auto Ep2 = get<0>(param);
    auto Ep12 = get<1>(param);
    auto Ee2e = get<2>(param);
    auto length = get<3>(param);
    // auto T = get<4>(param);

    stringstream path;
    time_t now = time(0);
    auto ltm = localtime(&now);
#if 1
    mkdir("./data/IC_stepheating/", 00755);
    path << "./data/IC_stepcooling/";
#else
    mkdir("./data/IC_isothermal_less", 00755);
    path << "./data/IC_isothermal_less/";

#endif

    path << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday
         << "/";

    string path_date;
    path >> path_date;
    mkdir(path_date.c_str(), 00755);
    path.clear();
    path << path_date << "Ep2=" << Ep2 << "Ep12=" << Ep12 << "Ee2e=" << Ee2e
         << "T=" << T << '/';
    string spath;
    path >> spath;
    mkdir(spath.c_str(), 00755);
    auto log_file = freopen((spath + string("out.log")).c_str(), "w", stdout);
    printf("Runing task Ep12=%f,Ep=%f ,Ee2e=%f,length=%d,T=%g\n", Ep12, Ep2,
           Ee2e, length, T);
    Room room(length, length, length,
              vector<vector<double>>{{0, 0, 0}, {0, 0, Ep12}, {0, Ep12, Ep2}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, Ee2e}}, 24);
    install_model(room, nums);

    cout << "installed" << endl;
    room.save(spath + string("init"));

#if 0
    room.preheat(100000, 10000);
    for (auto a : myrange<double>(5.0, 0.0, -0.1)) {
      cout << a << endl;
      room.movie(100000, 20000, a, spath);
    }
#elif 0

    for (auto a : myrange<double>(0.1, 10, 0.01)) {
      cout << a << endl;
      room.movie(10000, 200000, a, spath);
    }
#elif 0
    double Tm = 0.3 * Ee2e + 3.5 * Ep2;
    double Ticm = 0.2 * Ee2e + 3.5 * Ep2 + 1.6 * Ep12;
    room.preheat(10000, 1000);
    for (auto a : myrange<double>(Ticm, Tm, -0.01)) {
      cout << a << endl;
      room.movie(10000, 200000, a, spath);
    }
#else
    double Tm = 0.3 * Ee2e + 2.5 * Ep2;
    // double Ticm = 0.2 * Ee2e + 3.3 * Ep2 + 1.6 * Ep12;
    double T_crystal = Tm + T;
    room.preheat(100000, 10000);
    room.movie(500000, 20000, T_crystal, spath);

#endif
    fclose(log_file);
  }

  auto parameters() -> vector<tuple<double, double, double, int>> {
    auto Ep2 = vector<double>{0.2};
    auto Ep12 = myrange<double>(0.6, 1.3, 0.2);
    auto Ee2e = vector<double>{4, 6, 8};
    auto length = vector<int>{40};
    // auto T = myrange<double>(0.2, 2.1, 0.2);
    return Cartesian_product(Ep2, Ep12, Ee2e, length);
  }
};