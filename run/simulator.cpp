
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
        room.input_one_FCC(vec{i, j, 0}, 128, 2, 1, vector<int>(128, 1), 1);
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
        if ((i + j) % 8 != 0 || (i - j) % 8 != 0) {
          // if (j % 2 == 0 || i % 2 == 0) {
          //|| (i + j) % 8 == 0 || (i - j) % 8 == 0) {
          // if (i != 10 || j != 10) {
          for (int k = 0; k < int(9 * room.shape[2] / 10); k += 2) {
            room.input_one_ECC(vec{i, j, k + 1}, 2, 2, vector<int>{2, 2}, 1);
          }
        } else {
          room.input_one_ECC(vec{i, j, 1}, room.shape[2] - 3, 2,
                             vector<int>(room.shape[2] - 3, 1), 1);
        }
      }
    }
  }
  virtual void simulate(tuple<double, double, double, int, double> param) {
    // auto [Ep2, Ee2e,length, T]= param;
    auto Ep2 = get<0>(param);
    auto Ep12 = get<1>(param);
    auto Ee2e = get<2>(param);
    auto length = get<3>(param);
    auto T = get<4>(param);
    T = T + 0.1 * Ee2e;
    stringstream path;
    time_t now = time(0);
    auto ltm = localtime(&now);
    mkdir("./data/eccdata/", 00744);
    path << "./data/eccdata/";
    path << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday
         << "/";

    string path_date;
    path >> path_date;
    mkdir(path_date.c_str(), 00744);
    path.clear();
    path << path_date << "Ep2=" << Ep2 << "Ep12=" << Ep12 << "Ee2e=" << Ee2e
         << "T=" << T << '/';
    string spath;
    path >> spath;
    mkdir(spath.c_str(), 00744);
    auto log_file = freopen((spath + string("out.log")).c_str(), "w", stdout);
    printf("Runing task Ep12=%f,Ep=%f ,Ee2e=%f,T=%f,length=%d\n", Ep12, Ep2,
           Ee2e, T, length);
    Room room(length, length, length,
              vector<vector<double>>{{0, 0, 0}, {0, 0, Ep12}, {0, Ep12, Ep2}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, Ee2e}}, 24);
    install_model(room);

    cout << "installed" << endl;
    cout << "preheating" << endl;
    room.save(spath + string("init"));
    room.preheat(100000, 1000);
    cout << "preheated" << endl;
    room.movie(500000, 1000, T, spath);
    fclose(log_file);
  }
  virtual void simulate2(tuple<double, double, double, int, double> param) {
    // auto [Ep2, Ee2e,length, T]= param;
    auto Ep2 = get<0>(param);
    auto Ep12 = get<1>(param);
    auto Ee2e = get<2>(param);
    auto length = get<3>(param);
    auto T = get<4>(param);
    T = T + 0.25 * Ee2e - 1;
    auto Ep1 = (0.35 * Ee2e + 3 * Ep2) / 5;
    stringstream path;
    time_t now = time(0);
    auto ltm = localtime(&now);
    mkdir("./data/Ef_0_eccdata_ep1/", 00755);
    path << "./data/Ef_0_eccdata_ep1/";
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
    printf("Runing task Ep12=%f,Ep=%f ,Ee2e=%f,T=%f,length=%d\n", Ep12, Ep2,
           Ee2e, T, length);
    Room room(length, length, length,
              vector<vector<double>>{{0, 0, 0}, {0, Ep1, Ep12}, {0, Ep12, Ep2}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, Ee2e}}, 24);
    install_model(room);

    cout << "installed" << endl;
    cout << "preheating" << endl;
    room.save(spath + string("init"));
    room.preheat(10000, 1000);
    cout << "preheated" << endl;
    room.movie(500000, 1000, T, spath);
    fclose(log_file);
  }
  virtual void simulate3(tuple<double, double, double, int> param, double T) {
    // auto [Ep2, Ee2e,length, T]= param;
    auto Ep2 = get<0>(param);
    auto Ep12 = get<1>(param);
    auto Ee2e = get<2>(param);
    auto length = get<3>(param);
    // auto T = get<4>(param);

    stringstream path;
    time_t now = time(0);
    auto ltm = localtime(&now);
    mkdir("./data/IC_isothermal_less", 00755);
    path << "./data/IC_isothermal_less/";
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
    install_model(room);

    cout << "installed" << endl;
    room.save(spath + string("init"));

#if 0
    room.preheat(100000, 10000);
    for (auto a : myrange<double>(5.0, 0.0, -0.1)) {
      cout << a << endl;
      room.movie(100000, 20000, a, spath);
    }
#elif 0

    for (auto a : myrange<double>(0.1, 10, 0.05)) {
      cout << a << endl;
      room.movie(100000, 200000, a, spath);
    }
#elif 0
    double Tm = 0.3 * Ee2e + 3.5 * Ep2;
    double Ticm = 0.2 * Ee2e + 3.5 * Ep2 + 1.6 * Ep12;
    room.preheat(10000, 1000);
    for (auto a : myrange<double>(Ticm, Tm, -0.05)) {
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

  auto parameters() -> vector<tuple<double, double, double, int, double>> {
    auto Ep2 = vector<double>{0.2};
    auto Ep12 = myrange<double>(0.4, 1.3, 0.2);
    auto Ee2e = vector<double>{8, 12};
    auto length = vector<int>{32};
    auto T = myrange<double>(1.2, 2.5, 0.2);
    return Cartesian_product(Ep2, Ep12, Ee2e, length, T);
  }
  auto parameters2() -> vector<tuple<double, double, double, int>> {
    auto Ep2 = vector<double>{0.2};
    auto Ep12 = myrange<double>(0.6, 1.3, 0.2);
    auto Ee2e = vector<double>{4, 6, 8};
    auto length = vector<int>{40};
    // auto T = myrange<double>(0.2, 2.1, 0.2);
    return Cartesian_product(Ep2, Ep12, Ee2e, length);
  }
};
class UreaCrystal : public Simulator {
 public:
  virtual void install_model(Room &room) {
    for (int i = 0; i < room.shape[0]; i++) {
      for (int j = 0; j < room.shape[1]; j++) {
        for (int k = 0; k < int(9 * room.shape[2] / 10); k += 2) {
          room.input_one_ECC(vec{i, j, k + 1}, 2, 2, vector<int>{2, 2}, 1);
        }
      }
    }
  }
  virtual void simulate(tuple<double, double, int, double> param) {
    // auto [Ep2, Ee2e,length, T]= param;
    auto Ep2 = get<0>(param);

    auto Ee2e = get<1>(param);
    auto length = get<2>(param);
    auto T = get<3>(param);
    stringstream path;
    time_t now = time(0);
    auto ltm = localtime(&now);
    mkdir("./data/", 00744);
    path << "./data/";
    path << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday
         << "/";

    string path_date;
    path >> path_date;
    mkdir(path_date.c_str(), 00744);
    path.clear();
    path << path_date << "smallEp2=" << Ep2 << "Ee2e=" << Ee2e << "T=" << T
         << '/';
    string spath;
    path >> spath;
    // cout<<spath<<endl;
    mkdir(spath.c_str(), 00744);
    auto log_file = freopen((spath + string("out.log")).c_str(), "w", stdout);
    printf("Runing task,Ep=%f ,Ee2e=%f,T=%f,length=%d\n", Ep2, Ee2e, T, length);
    Room room(length, 3 * length, length,
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, Ep2}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, Ee2e}}, 24);
    install_model(room);

    cout << "installed" << endl;
    cout << "preheating" << endl;
    room.save(spath + string("init"));
    room.preheat(50000, 1000);
    cout << "preheated" << endl;
    room.movie(20000, 100, T, spath);
    fclose(log_file);
  }
  virtual void simulate2(tuple<double, double, int> param) {
    // step heating &&cooling .
    // auto [Ep2, Ee2e,length, T]= param;//std=c++17
    auto Ep2 = get<0>(param);
    auto Ee2e = get<1>(param);
    auto length = get<2>(param);
    stringstream path;
    time_t now = time(0);
    auto ltm = localtime(&now);
#if 1
    mkdir("./30datastepcool/", 00744);
    path << "./30datastepcool/";
#else
    mkdir("./30datastepheat/", 00744);
    path << "./30datastepheat/";
#endif
    path << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday
         << "/";

    string path_date;
    path >> path_date;
    path.clear();
    mkdir(path_date.c_str(), 00744);
    path << path_date << "Ep2=" << Ep2 << "Ee2e=" << Ee2e << '/';
    string spath;
    path >> spath;
    mkdir(spath.c_str(), 00744);
    auto log_file = freopen((spath + string("out.log")).c_str(), "w", stdout);
    printf("Runing task,Ep=%f ,Ee2e=%f,length=%d\n", Ep2, Ee2e, length);
    Room room(length, length, length,
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, Ep2}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, Ee2e}}, 24);
    install_model(room);

    cout << "installed" << endl;
    room.save(spath + string("init"));
#if 1
    room.preheat(100000, 10000);
    for (auto a : myrange<double>(5.0, 0.0, -0.1)) {
      cout << a << endl;
      room.movie(100000, 20000, a, spath);
    }
#else
    for (auto a : myrange<double>(0.1, 5, 0.1)) {
      cout << a << endl;
      room.movie(100000, 20000, a, spath);
    }
#endif
    fclose(log_file);
  }
  virtual void simulate3(tuple<double, double, int> param) {
    auto Ep2 = get<0>(param);
    auto Ee2e = get<1>(param);
    auto length = get<2>(param);
    stringstream path;
    time_t now = time(0);
    auto ltm = localtime(&now);
    mkdir("./dataisothermal/", 00744);
    path << "./dataisothermal/";
    path << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday
         << "/";
    string path_date;
    path >> path_date;
    path.clear();
    mkdir(path_date.c_str(), 00744);
    path << path_date << "Ep2=" << Ep2 << "Ee2e=" << Ee2e << '/';
    string spath;
    path >> spath;
    mkdir(spath.c_str(), 00744);
    auto log_file = freopen((spath + string("out.log")).c_str(), "w", stdout);
    printf("Runing task,Ep=%1g ,Ee2e=%1g,length=%d\n", Ep2, Ee2e, length);
    Room room(length, length, length,
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, Ep2}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
              vector<vector<double>>{{0, 0, 0}, {0, 0, 0}, {0, 0, Ee2e}}, 24);
    install_model(room);

    cout << "installed" << endl;
    room.save(spath + string("init"));

    // room.preheat(100000, 10000);
    // room.movie(1000000, 10000, , spath);
    fclose(log_file);
  }
  auto parameters() -> vector<tuple<double, double, int, double>> {
    auto Ep2 = myrange<double>(0.2, 1.1, 0.2);
    auto Ee2e = myrange<double>(4.0, 11, 2);
    auto length = vector<int>{24};
    auto T = myrange<double>(1, 5.0, 0.5);
    return Cartesian_product(Ep2, Ee2e, length, T);
  }
  auto parameters2() -> vector<tuple<double, double, int>> {
    auto Ep2 = myrange<double>(0.2, 1.1, 0.2);
    auto Ee2e = myrange<double>(4.0, 11, 2);
    auto length = vector<int>{30};
    return Cartesian_product(Ep2, Ee2e, length);
  }
  auto parameters3() -> vector<tuple<double, double, int>> {
    auto Ep2 = myrange<double>(0.2, 0.9, 0.2);
    auto Ee2e = myrange<double>(4.0, 8.1, 2);
    auto length = vector<int>{24};
    return Cartesian_product(Ep2, Ee2e, length);
  }
};
