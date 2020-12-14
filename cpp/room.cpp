
#include "room.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <fstream>
#include <random>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <utility>

#include "utils.h"

#ifndef _WIN32
#define sscanf_s sscanf
#endif
Room::Room(int x, int y, int z, int type)
    : lattice(x, y, z), shape(vec{x, y, z}) {
  cout << "construction" << endl;
  if (type == 24) {
    cout << "using count_parallel_nearby24" << endl;
    count_parallel = &Room::count_parallel_nearby24;
  }
  // else if (type == 8)
  // {
  //     cout << "using count_parallel_nearby8" << endl;
  //     count_parallel = &Room::count_parallel_nearby8;
  // }
  // else if (type == 4)
  // {
  //     cout << "using count_parallel_nearby4" << endl;
  //     count_parallel = &Room::count_parallel_nearby4;
  // }
  initmoves();
  srand(time(NULL));
}

Room::Room(int x, int y, int z, vector<vector<double>> Ep,
           vector<vector<double>> Eb, vector<vector<double>> Ee2e, int type)
    : lattice(x, y, z), shape(vec{x, y, z}) {
  Ep_matrix = std::move(Ep);
  Eb_matrix = std::move(Eb);
  Ee2e_matrix = std::move(Ee2e);
  if (type == 24) {
    cout << "using count_parallel_nearby24" << endl;
    count_parallel = &Room::count_parallel_nearby24;
  }
  // else if (type == 8)
  // {
  //     cout << "using count_parallel_nearby8" << endl;
  //     count_parallel = &Room::count_parallel_nearby8;
  // }
  // else if (type == 4)
  // {
  //     cout << "using count_parallel_nearby4" << endl;
  //     count_parallel = &Room::count_parallel_nearby4;
  // }
  initmoves();
  srand(time(NULL));
  // cout<< time(NULL);
}

void Room::initmoves()  //
{
  if (dimension == 3) {
    for (int x = -1; x <= 1; x++) {
      for (int y = -1; y <= 1; y++) {
        for (int z = -1; z <= 1; z++) {
          moves.emplace_back(vec{x, y, z});
        }
      }
    }
  }
}

void Room::input_one_circle(vec init, int length, int direction, int movable) {
  // TODO
}

void Room::input_one_ECC(vec init, int length, int direction,
                         vector<int> type_list, int movable) {
  Polymer p;
  p.chain.resize(length);
  int chain_num = polymer_list.size();
  for (int j = 0; j < type_list.size(); j++) {
    int type = type_list[j];
    vec point(init);
    // cout << point << endl;
    point[direction] += j;
    p[j] = Point(point, chain_num, j, type, movable);
    lattice[p[j].location] = &p[j];
  }
  polymer_list.emplace_back(move(p));
}

void Room::input_one_FCC(vec init, int length, int direction,
                         int fold_direction, vector<int> type_list,
                         int movable) {
  Polymer p;
  p.chain.resize(length);
  // cout << init << endl;
  int chain_num = polymer_list.size();
  for (int j = 0; j < length; j++) {
    int type = type_list[j];
    vec point(init);

    if ((j / shape[direction]) % 2 == 0) {
      point[direction] += j % shape[direction];
    } else {
      point[direction] += shape[direction] - j % shape[direction] - 1;
    }
    point[fold_direction] += int(j / shape[direction]);

    p[j] = Point(point, chain_num, j, type, movable);
    lattice[p[j].location] = &p[j];
  }
  polymer_list.emplace_back(move(p));
}

void Room::inputECC(int num, int length) {  //

  vec start_point = shape / 2;
  start_point[1] -= length / 2;

  int sqrt_num = ceil(sqrt(num));
  start_point[0] -= sqrt_num / 2;
  start_point[2] -= sqrt_num / 2;
  for (int i = 0; i < sqrt_num; i++) {
    for (int j = 0; j < sqrt_num; j++) {
      // int type = rand() % 2 + 1;
      if (i * sqrt_num + j < num) {
        vec init{start_point[0] + i, start_point[1] + j, start_point[2]};
        // py_input_one_ECC( init, length,2,type,0);
      } else {
        return;
      }
    }
  }
}

void Room::inputcircle(int num, int length) {}

void Room::input_stop_chain() {
  for (int i = 0; i < shape[0] - 1; i++) {
    for (int j = 0; j < 2; j++) {
      vec init = {i, i + j, 0};
      // py_input_one_ECC(init, shape[2], 2, 1,1);
    }
  }
}

void Room::input_stop_chain2() {
  for (int i = 0; i < shape[0]; i++) {
    vec init = {0, i, 0};
  }
}

bool Room::intersect(const vec &point1, const vec &point2) const {  //
  vec direction = cal_direction(point1, point2);

  if (direction * direction == 3) {
    vec p1{point2[0], point1[1], point1[2]};
    vec p2{point2[0], point2[1], point1[2]};
    vec p3{point1[0], point2[1], point1[2]};
    vec p4{point1[0], point2[1], point2[2]};
    vec p5{point1[0], point1[1], point2[2]};
    vec p6{point2[0], point1[1], point2[2]};

    return get_chain_num(p1, p4) != -1 || get_chain_num(p2, p5) != -1 ||
           get_chain_num(p3, p6) != -1;
  } else if (direction * direction == 2) {
    if (point1[0] == point2[0]) {
      vec p1{point1[0], point2[1], point1[2]};
      vec p2{point2[0], point1[1], point2[2]};
      return get_chain_num(p1, p2) != -1;
    } else if (point1[1] == point2[1]) {
      vec p1{point2[0], point1[1], point1[2]};
      vec p2{point1[0], point2[1], point2[2]};
      return get_chain_num(p1, p2) != -1;
    } else if (point1[2] == point2[2]) {
      vec p1{point2[0], point1[1], point1[2]};
      vec p2{point1[0], point2[1], point2[2]};
      return get_chain_num(p1, p2) != -1;
    }
  }
  return false;
}

int Room::get_chain_num(const vec &p1, const vec &p2) const {
  Point *a = lattice[p1];
  if (a == nullptr) return -1;
  Point *b = lattice[p2];
  if (b == nullptr) return -1;
  if (a->chain_num == b->chain_num) {
    if (abs(a->pos_in_chain - b->pos_in_chain) == 1) return a->chain_num;
  }
  return -1;
}

bool Room::canMove(const vec &point, const vec &direction) const {
  Point *p = lattice[point];
  if (!p->movable) {
    return false;
  }
  vec p_next = (point + direction) % shape;
  if (lattice[p_next] == nullptr) {
    if (!intersect(point, p_next)) return true;
  }
  return false;
}

#ifdef TRUE_POSITION
void Room::stepMove(vec &position, vec &next_position, stack<Position> &path,
                    int true_p) {
  if (position == next_position) {
    if (lattice[position]->true_position == true_p) {
      return;
    }
    path.push(make_pair(position, lattice[position]->true_position));
    path.push(make_pair(next_position, true_p));

    lattice[position]->true_position = true_p;
  } else {
    path.push(make_pair(position, lattice[position]->true_position));
    path.push(make_pair(next_position, true_p));
    Point *temp = lattice[position];
    lattice[position] = nullptr;
    temp->location = next_position;
    temp->true_position = true_p;
    lattice[next_position] = temp;
  }
}
#else
void Room::stepMove(vec &position, vec &next_position, stack<Position> &path) {
  path.push(position);
  path.push(next_position);
  Point *temp = lattice[position];
  lattice[position] = nullptr;
  temp->location = next_position;
  lattice[next_position] = temp;
}
#endif
void Room::localSnakeMove(int i, int start_point_i, stack<Position> &path) {  //
  Polymer &polymer = polymer_list[i];
  if (!polymer[0].movable) return;
  int length = polymer.chain.size();
  if (length == 0) {
    return;
  }

  Point &start_point = polymer[start_point_i];

  vec p1, p2;
#ifdef TRUE_POSITION
  int m_rand = rand() % this->q;
  int true_p = m_rand % (this->q / moves.size());
  m_rand /= this->q / moves.size();
  vec direction(moves[m_rand]);
  if (direction == vec{0, 0, 0}) {
    stepMove((*pol_iter).location, (*pol_iter).location, path, true_p);
    return;
  }
#else
  vec direction(moves[rand() % moves.size()]);
  if (direction == vec{0, 0, 0}) {
    return;
  }

#endif
  vec p_next;
  if (canMove(start_point.location, direction)) {
    p_next = (start_point.location + direction) % shape;
    if (start_point_i > 0 && start_point_i < length - 1) {
      if (distance_squre(p_next, polymer[start_point_i + 1].location) >
              dimension &&
          distance_squre(p_next, polymer[start_point_i - 1].location) >
              dimension) {
        return;
      }
    }
    if (((start_point_i < length - 1) &&
         intersect(p_next, polymer[start_point_i + 1].location)) ||
        ((start_point_i > 0) &&
         intersect(p_next, polymer[start_point_i - 1].location))) {
      return;
    }
    p1 = start_point.location;
    p2 = start_point.location;
    stepMove(start_point.location, p_next, path);
  } else {
    return;
  }
  int j;
  for (j = start_point_i - 1; j > -1; j--) {
    if (distance_squre(polymer[j].location, polymer[j + 1].location) >
        dimension) {
      if (lattice[p1] == nullptr) {
        vec t1 = polymer[j].location;
        stepMove(t1, p1, path);
        p1 = t1;
      } else {
        cerr << __FUNCTION__ << ":  line:" << __LINE__ << "  " << p1
             << "not null" << endl;
        throw;
      }
    } else {
      break;
    }
  }
  if (j >= 0 && intersect(polymer[j].location, polymer[j + 1].location)) {
    repair(path);
    return;
  }
  for (j = start_point_i + 1; j < length; j++) {
    if (distance_squre(polymer[j].location, polymer[j - 1].location) >
        dimension) {
      if (lattice[p2] == nullptr) {
        vec t2 = polymer[j].location;
        stepMove(t2, p2, path);
        p2 = t2;
      } else {
        cerr << __FUNCTION__ << ":  line:" << __LINE__ << "  " << p1
             << "not null" << endl;
        throw;
      }
    } else {
      break;
    }
  }
  if (j < length && intersect(polymer[j].location, polymer[j - 1].location)) {
    repair(path);
    return;
  }
}

int Room::cal_crystallinity(int q) const {
  int num = 0;
  deque<Position> a;
  for (auto &p : polymer_list) {
    int length = p.chain.size();
    for (int j = 1; j < length; j++) {
      if ((this->*count_parallel)(p[j - 1].location, p[j].location, a, 1) > q) {
        num++;
      }
    }
  }
  return num;
}
void Room::init_queue(vector<array<int, 2>> &queue) {
  for (int i = 0; i < polymer_list.size(); i++) {
    for (int j = 0; j < polymer_list[i].chain.size(); j++) {
      queue.emplace_back(array<int, 2>{i, j});
    }
  }
  //;cout<<queue.size()<<endl;
}

void Room::movie(int m, int n, double T, string path) {
  double Ec = cal_Ec() * Ec0;
  double Ep = cal_Ep();
  //	double Eb = cal_Eb();
  double Ee2e = cal_Ee2e();
  double E = Ep + Ee2e + Ec;

  /*double a = cal_Ec() + cal_Ep();*/
  vector<array<int, 2>> queue;
  init_queue(queue);
  for (int i = 0; i < m; i++) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(queue.begin(), queue.end(), std::default_random_engine(seed));
    for (auto v : queue) {
      int k = v[0];
      int start_point_i = v[1];

      stack<Position> path;
      this->localSnakeMove(k, start_point_i, path);
      if (path.empty()) {
        continue;
      }
      double dEc = cal_dEc_nearby(path) * Ec0;
      double dEp = cal_dEp_nearby(path);
      //  double dEb = cal_dEb_nearby(path);
      double dEf = Ef0 * cal_dEf(path);  // *;
      double dEe2e = cal_dEe2e_nearby(path);
      double dE = dEp + dEe2e + dEc;
      if (dE - dEf >= 0) {
        E += dE;
        Ec += dEc;
        Ep += dEp;
        Ee2e += dEe2e;
        //				Eb += dEb;
      } else {
        auto a = randfloat();
        if (a < exp((dE - dEf) / T)) {
          // cout <<a;
          E += dE;
          Ec += dEc;
          Ep += dEp;
          Ee2e += dEe2e;
          //					Eb += dEb;
        } else {
          repair(path);
        }
      }
    }
    if (i < 40000) {
      if (i % (n / 10) == 0) {
#ifdef DEBUG
        if (Ee2e != cal_Ee2e()) {
          cerr << "Ee2e cal error" << Ee2e << "!=" << cal_Ee2e() << endl;
          throw std::runtime_error("Ee2e cal error");
        }
        if (Ep != cal_Ep()) {
          cerr << "Ep cal error" << Ep << "!=" << cal_Ep() << endl;
          throw std::runtime_error("Ep cal error");
        }

#endif
        cout << "i=" << i << "\t" << std::left << E << "\t" << std::left << Ep
             << "\t" << std::left << Ee2e << "\t" << std::left << Ec << "\n";
        save(path + to_string(T) + string("-") + to_string(i));
      }
    } else if (i % n == 0) {
#ifdef DEBUG

      if (Ee2e != cal_Ee2e()) {
        cerr << "Ee2e cal error" << Ee2e << "!=" << cal_Ee2e() << endl;
        throw std::runtime_error("Ee2e cal error");
      }
      if (Ep != cal_Ep()) {
        cerr << "Ep cal error" << Ep << "!=" << cal_Ep() << endl;
        throw std::runtime_error("Ep cal error");
      }
#endif
      cout << "i=" << i << "\t" << std::left << E << "\t" << std::left << Ep
           << "\t" << std::left << Ee2e << "\n";
      save(path + to_string(T) + string("-") + to_string(i));
    }
  }
}

void Room::preheat(int m, int n) {
  vector<array<int, 2>> queue;
  init_queue(queue);
  for (int i = 0; i < m; i++) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(queue.begin(), queue.end(), std::default_random_engine(seed));
    for (auto v : queue) {
      int k = v[0];
      int start_point_i = v[2];
      stack<Position> path;
      this->localSnakeMove(k, start_point_i, path);
    }
    if (i % n == 0) {
      time_t now = time(0);
      char *dt = ctime(&now);
      printf("%s i=%d\n", dt, i);
    }
  }
}

void Room::save(string filename) const {  // TODO
  ofstream file(filename, ios::out | ios::trunc);
  if (!file) {
    throw("can't create file");
  }
  file << "# "
       << "shape " << this->shape[0] << '\t' << this->shape[1] << '\t'
       << this->shape[2] << "\n";
  file << "# "
       << "Ep ";
  for (auto Ep_list : Ep_matrix) {
    for (auto Ep : Ep_list) {
      file << Ep << '\t';
    }
  }
  file << "\n";
  file << "# "
       << "Eb ";
  for (auto Eb_list : Eb_matrix) {
    for (auto Eb : Eb_list) {
      file << Eb << '\t';
    }
  }
  file << "\n";
  file << "# "
       << "nums " << this->polymer_list.size() << "\n";

  for (auto &p : polymer_list) {
    for (const Point &point : p.chain) {
      file << point.location[0] << '\t' << point.location[1] << '\t'
           << point.location[2] << '\t' << point.type << '\t' << point.movable
           << '\t'
           //<< point.true_position
           << "\n";
    }
    file << "####"
         << "\n";
  }

  file.close();
}

void Room::load(string filename) {
  ifstream file(filename, ios::in);
  if (!file) {
    throw "file open failed";
  }
  stringstream filestream;
  filestream << file.rdbuf();
  file.close();

  string temp;
  int chain_num = 0, pos_in_chain = 0;
  Polymer p;
  while (getline(filestream, temp)) {
    if (temp[0] == '#') {
      if (temp == "####") {
        for (auto point_tmp : p.chain) {
          lattice[point_tmp.location] = &p.chain[point_tmp.pos_in_chain];
        }

        polymer_list[chain_num] = std::move(p);
        chain_num++;
        pos_in_chain = 0;
        p = Polymer();
      }
      int pos = temp.find("shape");
      if (pos != -1) {
        auto data_str = temp.substr(pos + 6);
        auto result = split(data_str, '\t');

        if (result.size() == 3) {
#ifdef _WIN32
          sscanf_s(data_str.c_str(), "%d%d%d", &this->shape[0], &this->shape[1],
                   &this->shape[2]);
#else
          sscanf(data_str.c_str(), "%d%d%d", &this->shape[0], &this->shape[1],
                 &this->shape[2]);
#endif
          lattice.resize(shape[0], shape[1], shape[2]);
          continue;
        } else {
          throw "shape is not 3d\n";
        };
      }
      pos = temp.find("Ep");
      if (pos != -1) {
        auto data_str = temp.substr(pos + 3);
        auto result = split(data_str, '\t');
        if (result.size() == 9) {
          Ep_matrix.resize(3);
          for (auto &x : Ep_matrix) {
            x.resize(3);
          }

          sscanf_s(data_str.c_str(), "%lf%lf%lf%lf%lf%lf%lf%lf%lf",
                   &this->Ep_matrix[0][0], &this->Ep_matrix[0][1],
                   &this->Ep_matrix[0][2], &this->Ep_matrix[1][0],
                   &this->Ep_matrix[1][1], &this->Ep_matrix[1][2],
                   &this->Ep_matrix[2][0], &this->Ep_matrix[2][1],
                   &this->Ep_matrix[2][2]);
          continue;
        } else if (result.size() == 4) {
          Ep_matrix.resize(2);
          for (auto &x : Ep_matrix) {
            x.resize(2);
          }

          sscanf_s(data_str.c_str(), "%lf%lf%lf%lf", &this->Ep_matrix[0][0],
                   &this->Ep_matrix[0][1], &this->Ep_matrix[1][0],
                   &this->Ep_matrix[1][1]);
          continue;
        } else {
          printf("Ep_matrix is not squre,num=%ld\n", result.size());
          for (auto &x : result) {
            cout << "Ep" << x;
          }
          cout << "\n";
          throw "Ep_matrix is not squre\n";
        };
      }
      pos = temp.find("Eb");
      if (pos != -1) {
        continue;
      }
      pos = temp.find("nums");
      if (pos != -1) {
        auto data_str = temp.substr(pos + 4);
        int nums;
        sscanf_s(data_str.c_str(), "%d", &nums);
        polymer_list.resize(nums);
        continue;
      };
    } else {
      int x, y, z, t, m;
      sscanf_s(temp.c_str(), "%d%d%d%d%d", &x, &y, &z, &t, &m);

      p.chain.push_back(Point(vec{x, y, z}, chain_num, pos_in_chain, t, m));
      // ;
      pos_in_chain++;
    }
  }
  initmoves();
  file.close();
}

double Room::cal_Ec() const {
  double num = 0;
  for (const auto &p : polymer_list) {
    int length = p.chain.size();
    for (int j = 2; j < length; j++) {
      num += cal_ifline(p[j - 2].location, p[j - 1].location, p[j].location);
    }
  }
  return -num;
}

double Room::cal_dEp(deque<Position> &path) const {
  double num = 0;
  int i = 0;
  vec v1, v2;
  auto iter = path.begin();
  if (iter != path.end()) {
#ifdef TRUE_POSITION
    v1 = (*iter).first;
#else
    v1 = *iter;
#endif

  } else {
    return num;
  }
  iter++;
  while (iter != path.end()) {
#ifdef TRUE_POSITION
    v2 = (*iter).first;
#else
    v2 = *iter;
#endif
    num += ((this->*count_parallel))(v1, v2, path, 1);
    v1 = v2;
    iter++;
  }
#ifdef DEBUG
  cout << __FUNCTION__ << num << endl;
#endif
  return num;
}

double Room::cal_dEe2e(deque<Position> &path) {
  double num = 0;
  vec v1, v2;
  auto iter = path.begin();
  if (iter != path.end())
#ifdef TRUE_POSITION
    v1 = (*iter).first;
#else
    v1 = *iter;
#endif
  else
    return num;
  iter++;

  while (iter != path.end()) {
#ifdef TRUE_POSITION
    v2 = (*iter).first;
#else
    v2 = *iter;
#endif
    num += count_e2e(v1, v2);
    v1 = v2;
    iter++;
  }
  return num;
}
double Room::cal_dEc(deque<Position> &path) const {
  double num = 0;
  vec v1, v2, v3;
  auto iter = path.begin();
  if (iter != path.end())
#ifdef TRUE_POSITION
    v1 = (*iter).first;
#else
    v1 = *iter;
#endif
  else
    return num;
  iter++;
  if (iter != path.end())
#ifdef TRUE_POSITION
    v2 = (*iter).first;
#else
    v2 = *iter;
#endif
  else
    return num;
  iter++;
  while (iter != path.end()) {
#ifdef TRUE_POSITION
    v3 = (*iter).first;
#else
    v3 = *iter;
#endif
    num += cal_ifline(v1, v2, v3);
    v1 = v2;
    v2 = v3;
    iter++;
  }
  return -num;
}

double Room::cal_dEf(stack<Position> path) {
  if (path.empty()) return 0.0;
  deque<Position> new_path, old_path;

  stack<Position> do_path(path);

  while (!path.empty()) {
    auto v1 = path.top();
    //		cout<<v1.first<<endl;
    path.pop();
    auto v2 = path.top();
    //        cout<<v2.first<<endl;
    path.pop();
    new_path.emplace_back(v1);
    old_path.emplace_back(v2);
  }
  double num = 0;
  vec v1, v2;
  stack<Position> re = repair(do_path);

  auto iter = old_path.begin();
  if (iter != old_path.end())
#ifdef TRUE_POSITION
    v1 = (*iter).first;
#else
    v1 = *iter;
#endif
  else
    return num;
  iter++;
  while (iter != old_path.end()) {
#ifdef TRUE_POSITION
    v2 = (*iter).first;
#else
    v2 = *iter;
#endif
    num += (this->*count_parallel)(v1, v2, old_path, 2);
    v1 = v2;
    iter++;
  }
  repair(re);
  return num;
}

double Room::cal_if_inter(int i) const {
  const auto &p = polymer_list[i];
  int length = p.chain.size();
  for (int j = 1; j < length; j++) {
    if (intersect(p.chain[j - 1].location, p.chain[j].location)) {
      cerr << "at" << i << "intersected" << p.chain[j - 1].location
           << p.chain[j].location;
      this->save("test.data");
      throw;
    }
  }
  return 0;
}

double Room::cal_one_Ec(int i) const {  // has some bugs
  double num = 0;
  for (const auto &p : polymer_list) {
    int length = p.chain.size();
    for (int j = 2; j < length; j++) {
      num += cal_ifline(p.chain[j - 2].location, p.chain[j - 1].location,
                        p.chain[j].location);
    }
  }
  return -num;
}

double Room::cal_dEe2e_nearby(stack<Position> path) {
  if (path.empty()) return 0.0;
  deque<Position> new_path, old_path;
  stack<Position> do_path(path);

  while (!path.empty()) {
    auto v1 = path.top();
    path.pop();
    auto v2 = path.top();
    path.pop();
    new_path.emplace_back(v1);
    old_path.emplace_back(v2);
  }
  vec v1 = new_path[0];
  vec v2 = new_path.back();
  Point *p1 = lattice[v1];
  Point *p2 = lattice[v2];
  int chain_num = p1->chain_num;
  int length = polymer_list[chain_num].chain.size();
  if ((p1->pos_in_chain) > (p2->pos_in_chain)) {
    for (int i = p1->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
      new_path.push_front(polymer_list[chain_num][i].location);
      old_path.push_front(polymer_list[chain_num][i].location);
    }
    for (int i = p2->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
      new_path.push_back(polymer_list[chain_num][i].location);
      old_path.push_back(polymer_list[chain_num][i].location);
    }
  } else if (p1->pos_in_chain == p2->pos_in_chain) {
    for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
      new_path.push_front(polymer_list[chain_num][i].location);
      old_path.push_front(polymer_list[chain_num][i].location);
    }
    for (int i = p2->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
      new_path.push_back(polymer_list[chain_num][i].location);
      old_path.push_back(polymer_list[chain_num][i].location);
    }
  } else {
    for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
      new_path.push_front(polymer_list[chain_num][i].location);
      old_path.push_front(polymer_list[chain_num][i].location);
    }
    for (int i = p2->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
      new_path.push_back(polymer_list[chain_num][i].location);
      old_path.push_back(polymer_list[chain_num][i].location);
    }
  }

  double dEe2e1 = cal_dEe2e(new_path);
  stack<Position> re = repair(do_path);
  double dEe2e2 = cal_dEe2e(old_path);
  repair(re);
  return dEe2e1 - dEe2e2;
}

double Room::cal_dEc_nearby(stack<Position> path) const {
  if (path.empty()) return 0.0;
  deque<Position> new_path, old_path;

  while (!path.empty()) {
    auto v1 = path.top();
    path.pop();
    auto v2 = path.top();
    path.pop();
    new_path.emplace_back(v1);
    old_path.emplace_back(v2);
  }
  vec v1 = new_path[0];
  vec v2 = new_path.back();
  Point *p1 = lattice[v1];
  Point *p2 = lattice[v2];
  int chain_num = p1->chain_num;
  int length = polymer_list[chain_num].chain.size();
  if ((p1->pos_in_chain) > (p2->pos_in_chain)) {
    for (int i = p1->pos_in_chain + 1, j = 0; i < length && j < 2; i++, j++) {
      new_path.push_front(polymer_list[chain_num][i].location);
      old_path.push_front(polymer_list[chain_num][i].location);
    }
    for (int i = p2->pos_in_chain - 1, j = 0; i >= 0 && j < 2; i--, j++) {
      new_path.push_back(polymer_list[chain_num][i].location);
      old_path.push_back(polymer_list[chain_num][i].location);
    }
  } else if (p1->pos_in_chain == p2->pos_in_chain) {
    for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 2; i--, j++) {
      new_path.push_front(polymer_list[chain_num][i].location);
      old_path.push_front(polymer_list[chain_num][i].location);
    }
    for (int i = p2->pos_in_chain + 1, j = 0; i < length && j < 2; i++, j++) {
      new_path.push_back(polymer_list[chain_num][i].location);
      old_path.push_back(polymer_list[chain_num][i].location);
    }
  } else {
    for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 2; i--, j++) {
      new_path.push_front(polymer_list[chain_num][i].location);
      old_path.push_front(polymer_list[chain_num][i].location);
    }
    for (int i = p2->pos_in_chain + 1, j = 0; i < length && j < 2; i++, j++) {
      new_path.push_back(polymer_list[chain_num][i].location);
      old_path.push_back(polymer_list[chain_num][i].location);
    }
  }

  double dEc = cal_dEc(new_path) - cal_dEc(old_path);
  return dEc;
}

double Room::cal_dEp_nearby(stack<Position> path) {
  //    printf(__FUNCTION__);

  if (path.empty()) return 0.0;
  deque<Position> new_path, old_path;

  stack<Position> do_path(path);

  while (!path.empty()) {
    auto v1 = path.top();
    //		cout<<v1.first<<endl;
    path.pop();
    auto v2 = path.top();
    //        cout<<v2.first<<endl;
    path.pop();
    new_path.emplace_back(v1);
    old_path.emplace_back(v2);
  }

  vec v1 = new_path[0];
  vec v2 = new_path.back();
  Point *p1 = lattice[v1];
  Point *p2 = lattice[v2];
  int chain_num = p1->chain_num;
  int length = polymer_list[chain_num].chain.size();
  if (p1->pos_in_chain > p2->pos_in_chain) {
    for (int i = p1->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
      new_path.push_front(polymer_list[chain_num][i].location);
      old_path.push_front(polymer_list[chain_num][i].location);
    }
    for (int i = p2->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
      new_path.push_back(polymer_list[chain_num][i].location);
      old_path.push_back(polymer_list[chain_num][i].location);
    }
  } else if (p1->pos_in_chain == p2->pos_in_chain) {
    for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
      new_path.push_front(polymer_list[chain_num][i].location);
      old_path.push_front(polymer_list[chain_num][i].location);
    }
    for (int i = p1->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
      new_path.push_back(polymer_list[chain_num][i].location);
      old_path.push_back(polymer_list[chain_num][i].location);
    }
  } else {
    for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
      new_path.push_front(polymer_list[chain_num][i].location);
      old_path.push_front(polymer_list[chain_num][i].location);
    }
    for (int i = p2->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
      new_path.push_back(polymer_list[chain_num][i].location);
      old_path.push_back(polymer_list[chain_num][i].location);
    }
  }

  double dEp1;
  dEp1 = cal_dEp(new_path);
  stack<Position> re = repair(do_path);
  double dEp2;
  dEp2 = cal_dEp(old_path);
  repair(re);
  return dEp1 - dEp2;
}

// double Room::cal_dEb_nearby(stack<pair<vec,int>> path)
//{
//
//	if (path.empty()) return 0.0;
//	deque<pair<vec,int>> new_path, old_path;
//
//	stack<pair<vec,int>> do_path(path);
//
//	while (!path.empty()) {
//        auto v1 = path.top();
//		path.pop();
//        auto v2 = path.top();
//		path.pop();
//		//cout << v1 << ',' << v2 << endl;
//		new_path.emplace_back(v1);
//		old_path.emplace_back(v2);
//	}
//
//	vec first_point_new = new_path.back();
//
//
//	vec	last_point_old = old_path.front();
//
//	double Eb1 = cal_Eb_point(first_point_new, 2) +
// cal_Eb_point(last_point_old, 2); 	stack<pair<vec,int>> re =
// repair(do_path); 	double Eb2 = cal_Eb_point(last_point_old, 2) +
// cal_Eb_point(first_point_new, 2); 	repair(re); 	return Eb1 - Eb2;
//
//}

double Room::cal_Ep() const {
  double num = 0;
  deque<Position> a;

  for (const auto &p : polymer_list) {
    int length = p.chain.size();
    for (int j = 1; j < length; j++) {
      num += (this->*count_parallel)(p[j - 1].location, p[j].location, a, 1);
    }
  }
  return num / 2.0;
}

double Room::cal_Ee2e() {
  double num = 0;
  deque<Position> a;

  for (const auto &p : polymer_list) {
    int length = p.chain.size();
    for (int j = 1; j < length; j++) {
      num += count_e2e(p[j - 1].location, p[j].location);  // TODO
    }
  }
  return num / 2.0;
}
double Room::count_e2e(const vec &point1, const vec &point2) {  // TODO

  double num_self = 0;
  int chain_num;
  assert(lattice[point1]);
  assert(lattice[point2]);

  chain_num = lattice[point1]->chain_num;
  vec p1(point1), p2(point2);
  int type1 = lattice[point1]->type;
  vec dir = 2 * cal_direction(p1, p2);
  p1 = (point1 + dir) % shape;
  p2 = (point2 + dir) % shape;
  int result = get_chain_num(p1, p2);

  if (result >= 0) {
    int type2 = lattice[p1]->type;
    num_self += Ee2e_matrix[type1][type2];
  }
  p1 = (point1 - dir) % shape;
  p2 = (point2 - dir) % shape;
  result = get_chain_num(p1, p2);
  if (result >= 0) {
    int type2 = lattice[p1]->type;
    num_self += Ee2e_matrix[type1][type2];
  }
  return num_self;
}

// double Room::cal_Eb() const
//{
//	double sum = 0;
//	for (auto &p : polymer_list) {
//		for (auto &point : p.chain) {
//			sum += cal_Eb_point(point->location);
//		}
//	}
//	return sum;
//}

double Room::cal_one_Ep(int i) const {
  deque<Position> a;
  double num = 0;
  const Polymer &polymer = polymer_list[i];
  int length = polymer.chain.size();
  for (int j = 1; j < length; j++) {
    num += (this->*count_parallel)(polymer[j - 1].location, polymer[j].location,
                                   a, 0);
  }
  return num;
}

double Room::cal_one_Eb(int) const { return 0.0; }

// TODO
double Room::count_parallel_nearby24(const vec &point1, const vec &point2,
                                     deque<Position> &que, int cal_type) const {
  double num_self = 0, num_others = 0;
  int crystal_nums = 0;
  int chain_num;
  if (lattice[point1] == nullptr || lattice[point2] == nullptr) {
    cerr << __FUNCTION__ << ":  line:" << __LINE__ << "  " << point1 << "null"
         << endl;
    throw "NULL";
  }
#ifdef TRUE_POSITION

  if (lattice[point1]->true_position != 0 ||
      lattice[point2]->true_position != 0) {
    return 0;
  }
#endif
  chain_num = lattice[point1]->chain_num;
  int type1 = max(lattice[point1]->type, lattice[point2]->type);
  vec p1, p2;
  vec direction = cal_direction(point1, point2);

  // cout << direction<<endl;
  for (auto &direc : moves) {
    if ((direc == direction) || if_opposite(direc, direction) ||
        (direc == vec{0, 0, 0})) {
      // cout << vec{ x,y,z };
      continue;
    }
    p1 = (point1 + direc) % shape;
    p2 = (point2 + direc) % shape;
    int result = get_chain_num(p1, p2);

    if (result == -1) {
      continue;
    } else {
      crystal_nums++;
#ifdef TRUE_POSITION
      if (lattice[p1]->true_position != 0 || lattice[p2]->true_position != 0) {
        continue;
      }
#endif
      int type2 = max(lattice[p1]->type, lattice[p2]->type);
      double Ep_cross = Ep_matrix[type1][type2];
      if (result == chain_num) {
#ifdef TRUE_POSITION
        if (find_in_que(que, make_pair(p1, lattice[p1]->true_position)) &&
            find_in_que(que, make_pair(p2, lattice[p2]->true_position)))
#else
        if (find_in_que(que, p1) && find_in_que(que, p2))
#endif
        {
          //                    cout<<"find"<<endl;
          num_self += 0.5 * Ep_cross;
        } else {
          num_self += Ep_cross;
        }
      } else {
        num_others += Ep_cross;
      }
    }
  }

  if (cal_type == 0) {
    return num_others + num_self / 2.0;
    // cout << num_others << ',' << num_self << endl;
  } else if (cal_type == 2) {
    return crystal_nums;
  } else {
    return num_others + num_self;
  }
}

// double Room::count_parallel_nearby12(vec &point1, vec &point2,
//	deque<vec> & que, int cal_type)const {//TODO
//
//	double num_self = 0, num_others = 0;
//	int chain_num;
//	if (lattice[point1] == nullptr)
//		throw "NULL";
//	chain_num = lattice[point1]->chain_num;
//	vec p1(point1), p2(point2);
//	vec dir = cal_direction(p1, p2);
//	//int i = 0; j = 0;
//	//int k = 3 - i - j;
//	for (int x = -1; x <= 1; x++) {
//		for (int y = -1; y <= 1; y++) {
//			if (x == 0 && y == 0) {
//				continue;
//			}
//			p1[i] = (point1[i] + shape[i] + x) % shape[i];
//			p2[i] = (point2[i] + shape[i] + x) % shape[i];
//			p1[j] = (point1[j] + shape[j] + y) % shape[j];
//			p2[j] = (point2[j] + shape[j] + y) % shape[j];
//			int result;
//			result = get_side_num(p1, p2);
//			if (result == -1) { ; }
//			else {
//				if (result == chain_num) {
//					if (find_in_que(que, p1) &&
// find_in_que(que,
// p2)) { 						num_self += 0.5;
//					}
//					else {
//						num_self += 1;
//					}
//				}
//				else {
//					num_others += 1;
//				}
//			}
//
//		}
//
//	}
//	if (cal_type == 0) {
//		return num_others + num_self / 2;
//		//cout << num_others << ',' << num_self << endl;
//	}
//	else {
//
//		if (num_self != 0) { ; }
//		//cout << num_others << ',' << num_self << endl;
//		return num_others + num_self;
//	}
//}
//
// double Room::count_parallel_nearby4(vec &point1, vec &point2,
//                                     deque<Position> &que, int cal_type) const
// { //TODO
//     throw "bugs";
//     double num_self = 0, num_others = 0;
//     int chain_num;
//     if (lattice[point1] == nullptr)
//         throw "NULL";
//     chain_num = lattice[point1]->chain_num;
//     vec p1(point1), p2(point2);
//     vec dir = cal_direction(p1, p2);
//     int i = 0, j = 0;
//     //int k = 3 - i - j;
//     for (int x = -1; x <= 1; x++)
//     {
//         for (int y = -1; y <= 1; y++)
//         {
//             if (x == 0 || y == 0)
//             {
//                 continue;
//             }
//             p1[i] = (point1[i] + shape[i] + x) % shape[i];
//             p2[i] = (point2[i] + shape[i] + x) % shape[i];
//             p1[j] = (point1[j] + shape[j] + y) % shape[j];
//             p2[j] = (point2[j] + shape[j] + y) % shape[j];
//             int result;
//             result = get_chain_num(p1, p2);
//             if (result == -1)
//             {
//                 ;
//             }
//             else
//             {
//                 if (result == chain_num)
//                 {
//                     if (find_in_que(que, make_pair(p1,
//                     lattice[p1]->true_position)) &&
//                         find_in_que(que, make_pair(p2,
//                         lattice[p2]->true_position)))
//                     {
//                         num_self += 0.5;
//                     }
//                     else
//                     {
//                         num_self += 1;
//                     }
//                 }
//                 else
//                 {
//                     num_others += 1;
//                 }
//             }
//         }
//     }
//     if (cal_type == 0)
//     {
//         return num_others + num_self / 2;
//         //cout << num_others << ',' << num_self << endl;
//     }
//     else
//     {

//         if (num_self != 0)
//         {
//             ;
//         }
//         //cout << num_others << ',' << num_self << endl;
//         return num_others + num_self;
//     }
// }

// double Room::count_parallel_nearby8(vec &point1, vec &point2,
//                                     deque<Position> &que, int cal_type) const
// { //TODO

//     double num_self = 0, num_others = 0;
//     int chain_num;
//     if (lattice[point1] == nullptr)
//         throw "NULL";
//     chain_num = lattice[point1]->chain_num;
//     vec p1(point1), p2(point2);
//     vec dir = cal_direction(p1, p2);
//     int i = 0, j = 0;
//     //int k = 3 - i - j;
//     for (int x = -1; x <= 1; x++)
//     {
//         for (int y = -1; y <= 1; y++)
//         {
//             if (x == 0 && y == 0)
//             {
//                 continue;
//             }
//             p1[i] = (point1[i] + shape[i] + x) % shape[i];
//             p2[i] = (point2[i] + shape[i] + x) % shape[i];
//             p1[j] = (point1[j] + shape[j] + y) % shape[j];
//             p2[j] = (point2[j] + shape[j] + y) % shape[j];
//             int result;
//             result = get_chain_num(p1, p2);
//             if (result == -1)
//             {
//                 ;
//             }
//             else
//             {
//                 if (result == chain_num)
//                 {
//                     if (find_in_que(que, make_pair(p1,
//                     lattice[p1].true_position)) &&
//                         find_in_que(que, make_pair(p2,
//                         lattice[p2].true_position)))
//                     {
//                         num_self += 0.5;
//                     }
//                     else
//                     {
//                         num_self += 1;
//                     }
//                 }
//                 else
//                 {
//                     num_others += 1;
//                 }
//             }
//         }
//     }
//     if (cal_type == 0)
//     {
//         return num_others + num_self / 2;
//         //cout << num_others << ',' << num_self << endl;
//     }
//     else
//     {

//         if (num_self != 0)
//         {
//             ;
//         }
//         //cout << num_others << ',' << num_self << endl;
//         return num_others + num_self;
//     }
// }

double Room::cal_average_thick() const { return 0.0; }

// double Room::count_parallel_B(vec &point1, vec &point2,
//	deque<vec> & que, int cal_type)const {
//    //cout << __FUNCTION__ << endl;
//
//	double num_self = 0, num_others = 0;
//	int chain_num;
//	if (lattice[point1] == nullptr)
//		throw "NULL";
//	chain_num = lattice[point1]->chain_num;
//	vec p1, p2;
//	vec direction = cal_direction(point1, point2);
//	//cout << direction<<endl;
//	for (auto &direc : moves) {
//		if ((direc == direction) || ((direc + direction) == vec{ 0,0,0
//}))
//{
//			//cout << vec{ x,y,z };
//			continue;
//		}
//		for (int i = 0; i < 5; i++) {
//            if (i == 1) {
//
//            }
//			p1 = (point1 + i * direc) % shape;
//			p2 = (point2 + i * direc) % shape;
//			int result = get_side_num(p1, p2);
//            if (result == -1) {
//                break;
//            }
//			else {
//                if (i == 0) {
//                    if (result == chain_num) {
//                        if (find_in_que(que, p1) && find_in_que(que, p2)) {
//                            num_self -= 2.0;
//                        } else {
//                            num_self -= 4.0;
//                        }
//                    } else {
//                        num_others -= 4.0;
//                    }
//                }
//				if (result == chain_num) {
//					if (find_in_que(que, p1) &&
// find_in_que(que,
// p2)) { 						num_self +=
// 0.5*pow(this->b2a, i);
//					}
//					else {
//						num_self += 1 * pow(this->b2a,
// i);
//					}
//				}
//				else {
//					num_others += 1 * pow(this->b2a, i);
//				}
//			}
//		}
//	}
//
//	if (cal_type == 0) {
//		return num_others + num_self / 2.0;
//        cout << num_others << ',' << num_self << endl;
//	}
//	else {
//		if (num_self != 0) {
////			cout << num_others << ',' << num_self << endl;
//		}
//        return num_others + num_self;// TODO
//	}
//}

double Room::cal_Rg() const  //
{
  throw "NOT DONE!";
  double num = 0;
  for (auto &p : polymer_list) {
    // vec center = p.get_center();
  }

  /*for p in polymer_list :
  center = np.mean(p.chain, axis = 0)
  centers = np.asarray([center] * p.length)
  rgb = p.chain - centers
  return np.mean(rgb*rgb)*dimension*/
  return 0.0;
}

int Room::get_h2(int n) const {
  const Polymer &p = polymer_list[n];
  vec vector_end;
  vec point_last = p.chain[0].location;
  for (int i = 1; i < p.chain.size(); i++) {
    vec point = p.chain[i].location;
    vector_end = vector_end + cal_direction(point, point_last);
  }
  return vector_end * vector_end;
}
double Room::cal_h2() const  //
{
  // throw "NOT DONE!";
  int num = 0;
  double s = 0;
  for (int i = 0; i < this->polymer_list.size(); i++)
    if (polymer_list[i].chain.size() <= 2) {
      s += get_h2(i);
      num++;
    }
  if (num != 0) {
    return s / num;
  } else {
    return 0;
  }
}

#ifdef TRUE_POSITION
stack<Position> Room::repair(stack<Position> &path) {
  stack<Position> no_use;
  while (!path.empty()) {
    vec v1 = path.top().first;
    path.pop();
    vec v2 = path.top().first;
    int true_p = path.top().second;
    path.pop();
    stepMove(v1, v2, no_use, true_p);
  }
  return no_use;
}
#else
stack<Position> Room::repair(stack<Position> &path) {
  stack<Position> no_use;
  while (!path.empty()) {
    vec v1 = path.top();
    path.pop();
    vec v2 = path.top();
    path.pop();
    stepMove(v1, v2, no_use);
  }
  return no_use;
}

#endif
inline double Room::cal_ifline(const vec &p1, const vec &p2,
                               const vec &p3) const {
  if (cal_direction(p1, p2) == cal_direction(p2, p3)) return 0;
  return 1;
}

double Room::cal_Eb_point(const vec &p, int type) const {
  vec point;  // some bugs
  int i, j;
  i = lattice[p] == nullptr ? 0 : lattice[p]->type;
  double sum = 0;
  for (auto &direc : moves) {
    point = (p + direc) % shape;
    j = lattice[point] == nullptr ? 0 : lattice[point]->type;
    // cout << i << ',' << j << endl;
    sum += this->Eb_matrix[i][j];
  }
  // cout << "---dEp=" << sum << "---" << endl;
  return sum;
}

double Room::cal_Eb_point(const vec &p) const {
  vec point;  // some bugs
  int i, j;
  i = (lattice[p] == nullptr ? 0 : lattice[p]->type);
  double sum = 0;
  for (auto &direc : moves) {
    point = (p + direc) % shape;
    j = (lattice[point] == nullptr ? 0 : lattice[point]->type);
    if ((i * j) != 0)
      sum += this->Eb_matrix[i][j] / 2;
    else {
      sum += this->Eb_matrix[i][j];
    }
  }
  return sum;
}

double Room::cal_PSM() const {
  for (auto &p : polymer_list) {
    for (auto &point : p.chain) {
      cal_PSM_point(point.location);
    }
  }
  return 0.0;
}

double Room::cal_PSM_point(const vec &p) const { return 0.0; }

void Room::transfer_to() {
  for (int i = 0; i < polymer_list.size(); i++) {
    vector<Point> chain;
    Point point = polymer_list[i].chain[0];
    Point point1bak = point;
    chain.emplace_back(point);
    for (int j = 1; j < polymer_list[i].chain.size(); j++)

    {
      Point point2 = polymer_list[i].chain[j];
      Point point2bak = point2;
      point2.location =
          point.location + cal_direction(point1bak.location, point2.location);
      chain.emplace_back(point2);
      point = point2;
      point1bak = point2bak;
    }
    polymer_list[i].chain = chain;
  }
}

int Room::get_max_nucleus(int layer) {
  auto &matrix3 = this->lattice;
  matrix::Matrix2 bitmap(shape[0], shape[2] - 1);
  for (int i = 0; i < shape[1]; i++) {
    for (int j = 0; j < shape[2] - 1; ++j) {
      auto p1 = vec{layer, i, j};
      auto p2 = vec{layer, i, j + 1};
      if (get_chain_num(p1, p2) != -1) {
        bitmap[i][j] = 1;
      } else {
        bitmap[i][j] = 0;
      }
    }
  }
  //    cout<<"bitmap constructed"<<endl;
  return matrix::ConnectedComponentLabeling(bitmap);
}
int Room::get_max_nucleus() {
  int count = 0;
  // cout<<Ep_matrix[2][2]<<endl;
  // return 0;

  for (auto &polymer : polymer_list) {
    auto &point_last = polymer[0];
    for (int i = 1; i < polymer.chain.size(); i++) {
      auto &point = polymer[i];
      deque<vec> q;
      double res =
          count_parallel_nearby24(point.location, point_last.location, q, 2);
      // cout<<res<<endl;
      if (res >= 5) {
        count++;

      } else {
        if (point.type == 1) {
        } else {
          point_last.type = 0;
          point.type = 0;
        }
      }
    }
  }
  //    cout<<"bitmap constructed"<<endl;
  return count;
}

int Room::get_max_straight_length_p(int i) {
  const Polymer &p = polymer_list[i];
  if (p.chain.size() <= 2) {
    return 0;
  }
  int m = 0;
  int length = 0;
  vec point1 = p.chain[0].location;
  vec point2 = p.chain[1].location;
  for (int i = 2; i < p.chain.size(); i++) {
    vec point3 = p.chain[i].location;
    if (cal_ifline(point1, point2, point3) == 0) {
      length++;
    } else {
      m = max(m, length);
      length = 0;
    }
    point1 = point2;
    point2 = point3;
  }
  m = max(m, length);
  return m;
}

int Room::get_max_straight_length() {
  int m = 0;
  for (int i = 0; i < polymer_list.size(); i++) {
    m = max(m, get_max_straight_length_p(i));
  }
  return m;
}
double Room::get_average_straight_length() {
  int count = 0;
  int num = 0;
  for (int i = 0; i < polymer_list.size(); i++) {
    if (get_max_straight_length_p(i) > 0) {
      count += get_max_straight_length_p(i);
      num++;
    }
  }
  return double(count) / num;
}
ostream &operator<<(ostream &o, Point &p) {
  o << p.location;
  return o;
}

ostream &operator<<(ostream &o, Polymer &p) {
  for (int i = 0; i < p.chain.size(); i++) {
    o << p.chain[i] << ',';
  }
  return o;
}
