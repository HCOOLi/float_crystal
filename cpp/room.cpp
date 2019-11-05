#pragma once

#include "room.h"
#include<stack>
#include<tuple>
#include<fstream>
#include <utility>
#include <string>
#include"utils.h"


Room::Room(int x, int y, int z, int type) : lattice(x, y, z), shape(vec{x, y, z}) {
    cout << "construction" << endl;
    cout << "using count_parallel_nearby24" << endl;
    count_parallel = &Room::count_parallel_nearby24;
    initmoves();
    srand(time(NULL));
}

Room::Room(int x, int y, int z,
           vector<vector<double> > Ep, vector<vector<double> > Eb, int type)
        : lattice(x, y, z), shape(vec{x, y, z}) {
    Ep_matrix = std::move(Ep);
    Eb_matrix = std::move(Eb);
    cout << "using count_parallel_nearby24" << endl;
    count_parallel = &Room::count_parallel_nearby24;
    initmoves();
    srand(time(NULL));
    //cout<< time(NULL);
}

void Room::initmoves() {
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

inline vec Room::cal_direction(const vec &point1, const vec &point2) const {
    vec temp;
    for (int i = 0; i < 3; i++) {
        temp[i] = abs(point2[i] - point1[i]) <= 1 ?
                  point2[i] - point1[i] :
                  (point2[i] + 1) % shape[i] - (point1[i] + 1) % shape[i];
    }
    return temp;

}

void Room::input_one_circle(vec init, int length, int direction, int movable) {
    //TODO
}

void Room::input_one_ECC(vec init, int length, int direction, vector<int> type_list, int movable) {//放入一根伸直链
    //cout << __FUNCTION__ << endl;
    try {
        Polymer p;
        p.chain.resize(length);
        int chain_num = polymer_list.size();
        for (int j = 0; j < type_list.size(); j++) {
            int type = type_list[j];

            vec point(init);
            //cout << point << endl;
            point[direction] += j;
            p[j] = set_point(point, chain_num, j, type, movable, 0);
        }
        polymer_list.emplace_back(move(p));
    }
    catch (...) {
        cout << __FUNCTION__ << endl;
        throw;
    }


}

void Room::input_one_FCC(vec init, int length, int direction, int fold_direction, vector<int> type_list, int movable) {
    //cout << __FUNCTION__ << endl;
    try {
        Polymer p;
        p.chain.resize(length);
        //cout << init << endl;
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

            p[j] = set_point(point, chain_num, j, type, movable, 0);
        }
        polymer_list.emplace_back(move(p));
    }
    catch (...) {
        cout << __FUNCTION__ << endl;
        throw;
    }
    //cout << "end"<<endl;
}

void Room::inputECC(int num, int length) {//放入一些伸直链

    vec start_point = shape / 2;
    start_point[1] -= length / 2;

    int sqrt_num = ceil(sqrt(num));
    start_point[0] -= sqrt_num / 2;
    start_point[2] -= sqrt_num / 2;
    for (int i = 0; i < sqrt_num; i++) {
        for (int j = 0; j < sqrt_num; j++) {
            //int type = rand() % 2 + 1;
            if (i * sqrt_num + j < num) {
                vec init{start_point[0] + i, start_point[1] + j, start_point[2]};
                //py_input_one_ECC( init, length,2,type,0);
            } else {
                return;
            }
        }
    }
}

void Room::inputcircle(int num, int length) {


}

void Room::input_stop_chain() {
    try {
        for (int i = 0; i < shape[0] - 1; i++) {
            for (int j = 0; j < 2; j++) {
                vec init = {i, i + j, 0};
                //py_input_one_ECC(init, shape[2], 2, 1,1);
            }
        }


    }
    catch (...) {
        cout << __FUNCTION__ << endl;
        throw;
    }
}

void Room::input_stop_chain2() {
    for (int i = 0; i < shape[0]; i++) {
        vec init = {0, i, 0};
        //py_input_one_ECC(init, shape[2], 2, 1,1);
    }

}


inline shared_ptr<Point>
Room::set_point(vec location, int chain_num, int pos_in_chain, int type, int movable, int true_p) {
    //cout << __FUNCTION__ << endl;
    try {
        //cout << "makePoint" << "ended" << endl;
        if (lattice[location]) {
            cout << location;
            throw string("error");
        }
        shared_ptr<Point> temp(new Point(location, chain_num, pos_in_chain, type, movable, true_p));

        lattice[location] = temp;
        return temp;
    }
    catch (...) {
        cout << __FUNCTION__ << endl;
        throw;
    }


}

bool Room::intersect(vec &point1, vec &point2) const {//是否交叉
    vec direction = point2 - point1;

    if (direction * direction == 3) {

        vec p1{point2[0], point1[1], point1[2]};
        vec p2{point2[0], point2[1], point1[2]};
        vec p3{point1[0], point2[1], point1[2]};
        vec p4{point1[0], point2[1], point2[2]};
        vec p5{point1[0], point1[1], point2[2]};
        vec p6{point2[0], point1[1], point2[2]};
        return get_side_num(p1, p4) != -1 || get_side_num(p2, p5) != -1 || get_side_num(p3, p6) != -1;
    } else if (direction * direction == 2) {
        if (point1[0] == point2[0]) {
            vec p1{point1[0], point2[1], point1[2]};
            vec p2{point2[0], point1[1], point2[2]};
            return get_side_num(p1, p2) != -1;
        } else if (point1[1] == point2[1]) {
            vec p1{point2[0], point1[1], point1[2]};
            vec p2{point1[0], point2[1], point2[2]};
            return get_side_num(p1, p2) != -1;
        } else if (point1[2] == point2[2]) {
            vec p1{point2[0], point1[1], point1[2]};
            vec p2{point1[0], point2[1], point2[2]};
            return get_side_num(p1, p2) != -1;
        }
    }
    return false;

}

int Room::get_side_num(vec &p1, vec &p2) const {//是否有键接关系，是返回链的标号，否，返回-1
    try {
        shared_ptr<Point> a = lattice[p1];
        if (a == nullptr || a->true_position != 0) return -1;
        shared_ptr<Point> b = lattice[p2];
        if (b == nullptr || b->true_position != 0) return -1;
        if (a->chain_num == b->chain_num) {
            if (abs(a->pos_in_chain - b->pos_in_chain) == 1)
                return a->chain_num;
        }
    }
    catch (...) {
        cout << __FUNCTION__ << endl;
        throw;
    }
    return -1;

}

bool Room::canMove(vec &point, vec &direction) const {//能否移动
    try {
        shared_ptr<Point> p = lattice[point];
        if (p->movable == 1) {
            return false;
        }
        vec p_next = (point + direction) % shape;
        if (lattice[p_next] == nullptr) {
            if (!intersect(point, p_next))
                return true;
        }
        return false;
    }
    catch (...) {
        cout << __FUNCTION__ << endl;
        throw;
    }
}

void Room::stepMove(vec &position, vec &next_position, stack<pair<vec, int>> &path, int true_p) {

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
        shared_ptr<Point> temp = lattice[position];
        lattice[position] = nullptr;
        temp->location = next_position;
        temp->true_position = true_p;
        lattice[next_position] = temp;

    }
}

void Room::localSnakeMove(int i, stack<pair<vec, int>> &path) {//移动
    Polymer &polymer = polymer_list[i];
    int length = polymer.chain.size();

    if (length == 0) return;
    if (polymer[0]->movable == 1) return;
    int start_point = rand() % length;
    shared_ptr<Point> pol_iter = polymer[start_point];

    vec p1, p2;

    int m_rand = rand() % this->q;
    int true_p = m_rand % (this->q / moves.size());
    m_rand /= this->q / moves.size();
    vec direction(moves[m_rand]);
    if (direction == vec{0, 0, 0}) {
//	    printf("no move");
        //TODO
        stepMove((*pol_iter).location, (*pol_iter).location, path, true_p);
        return;
    }
    vec p_next;
    if (canMove((*pol_iter).location, direction)) {
        p_next = ((*pol_iter).location + direction) % shape;
        if (start_point > 0 && start_point < length - 1) {
            if (distance_squre(p_next, polymer[start_point + 1]->location) > dimension &&
                distance_squre(p_next, polymer[start_point - 1]->location) > dimension) {
                return;
            }
        }
        p1 = (*pol_iter).location;
        p2 = (*pol_iter).location;
        stepMove((*pol_iter).location, p_next, path, true_p);

    }
    else {
        return;
    }
    int j;
    for ( j = start_point - 1; j > -1; j--) {
        if (distance_squre(polymer[j]->location, polymer[j + 1]->location) > dimension) {
            try {
                if (lattice[p1] == nullptr) {
                    vec t1 = polymer[j]->location;
                    stepMove(t1, p1, path, rand() % (this->q / moves.size()));
                    p1 = t1;
                } else {
                    cout << "???????1";
                }
            }
            catch (...) {
                cout << "P1 i:" << j << endl;
            }
        } else {
            break;
        }
    }

    if(j>=0&&intersect(polymer[j]->location, polymer[j + 1]->location)){
        repair(path);
        return;
    }
    //repair then return;
    for (j = start_point + 1; j < length; j++) {
        if (distance_squre(polymer[j]->location, polymer[j - 1]->location) > dimension) {
            try {
                if (lattice[p2] == nullptr) {
                    vec t2 = polymer[j]->location;
                    stepMove(t2, p2, path, rand() % (this->q / moves.size()));
                    p2 = t2;
                } else {
                    cout << "???????2" << endl;
                }
            }
            catch (...) {
                cout << "p2 i:" << j;
            }
        } else {
            break;
        }
        if(j<length&&intersect(polymer[j]->location, polymer[j - 1]->location)){
            repair(path);
            return;
        }
    }
}

void Room::movie(int m, int n, double T) {//主循环
//	double Ec = cal_Ec()*Ec0;
    double Ep = cal_Ep();
//	double Eb = cal_Eb();
    double E = Ep;
    /*double a = cal_Ec() + cal_Ep();*/
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < polymer_list.size(); j++) {

            stack<pair<vec, int> > path;
            this->localSnakeMove(j, path);
            //TODO if   repair
            if (path.empty()) {
                continue;
            }
            double dEc = cal_dEc_nearby(path) * Ec0;
            double dEp = cal_dEp_nearby(path);
//			double dEb = cal_dEb_nearby(path);
            //double dEf = cal_dEf(path);
            double dE = dEp;

            if (dE >= 0) {
                E += dE;
//				Ec += dEc;
                Ep += dEp;
//				Eb += dEb;
            } else {
                auto a = randfloat();
                if (a < exp(dE / T)) {
                    //cout <<a;
                    E += dE;
//					Ec += dEc;
                    Ep += dEp;
//					Eb += dEb;
                } else {
                    repair(path);
                }
            }
        }
        if (i % n == 0) {
            printf("%f\t%f\t%f\t%f\n", 0.0, Ep, 0.0, E);
            //TODO
        }
    }
}

void Room::preheat(int m) {//预热，不判断能量

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < polymer_list.size(); j++) {

            stack<pair<vec, int >> path;
            this->localSnakeMove(j, path);
            //TODO if ???????repair??
            if (path.empty()) {
                continue;
            }

        }


    }

}

void Room::save(string filename) {
    ofstream file(filename, ios::out | ios::trunc);// 覆盖
    file << "# " << "shape " << this->shape[0] << '\t' << this->shape[1] << '\t' << this->shape[2] << endl;
    file << "# " << "Ep ";
    for (auto Ep_list:Ep_matrix) {
        for (auto Ep:Ep_list) {
            file << Ep << '\t';
        }
    }
    file << endl;
    file << "# " << "Eb ";
    for (auto Eb_list:Eb_matrix) {
        for (auto Eb:Eb_list) {
            file << Eb << '\t';
        }
    }
    file << endl;
    file << "# " << "nums " << this->polymer_list.size() << endl;

    for (auto &p : polymer_list) {
        for (shared_ptr<Point> &point:p.chain) {
            file << point->location[0] << '\t' << point->location[1] << '\t' << point->location[2] << '\t'
                 << point->type << '\t' << point->movable << '\t' << point->true_position << endl;
        }
        file << "####" << endl;
    }

    file.close();
}

void Room::load(string filename) {
    ifstream file(filename, ios::in);

    string temp;
    int chain_num = 0, pos_in_chain = 0;
    Polymer p;
    while (getline(file, temp)) {
        if (temp[0] == '#') {
            if (temp == "####") {
                polymer_list[chain_num] = p;
                chain_num++;
                pos_in_chain = 0;
                p = Polymer();
            }
            int pos = temp.find("shape");
            if (pos != -1) {
                auto data_str = temp.substr(pos + 6);
                auto result = split(data_str, '\t');

                if (result.size() == 3) {
                    sscanf_s(data_str.c_str(), "%d%d%d", &this->shape[0], &this->shape[1], &this->shape[2]);
                    lattice = Grid<shared_ptr<Point>>(shape[0], shape[1], shape[2]);
                    continue;
                } else {
                    printf("shape is not 3d\n");
                    throw "shape is not 3d\n";
                };

            }
            pos = temp.find("Ep");
            if (pos != -1) {
                auto data_str = temp.substr(pos + 3);
                auto result = split(data_str, '\t');

                if (result.size() == 9) {
                    Ep_matrix.resize(3);
                    for (auto &x:Ep_matrix) {
                        x.resize(3);
                    }
                    sscanf_s(data_str.c_str(),
                             "%lf%lf%lf%lf%lf%lf%lf%lf%lf",
                             &this->Ep_matrix[0][0], &this->Ep_matrix[0][1], &this->Ep_matrix[0][2],
                             &this->Ep_matrix[1][0], &this->Ep_matrix[1][1], &this->Ep_matrix[1][2],
                             &this->Ep_matrix[2][0], &this->Ep_matrix[2][1], &this->Ep_matrix[2][2]);
                    continue;
                } else if (result.size() == 4) {
                    Ep_matrix.resize(2);
                    for (auto &x:Ep_matrix) {
                        x.resize(2);
                    }
                    sscanf_s(data_str.c_str(),
                             "%lf%lf%lf%lf",
                             &this->Ep_matrix[0][0], &this->Ep_matrix[0][1],
                             &this->Ep_matrix[1][0], &this->Ep_matrix[1][1]);
                    continue;

                } else {
                    printf("Ep_matrix is not squre,num=%d\n", result.size());
                    for (auto &x:result) {
                        cout << "Ep" << x;
                    }
                    cout << endl;
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
            int x, y, z, t, m, t_p;
            sscanf_s(temp.c_str(), "%d%d%d%d%d%d", &x, &y, &z, &t, &m, &t_p);

            p.chain.push_back(set_point(vec{x, y, z}, chain_num, pos_in_chain, t, m, t_p));
            pos_in_chain++;


        }

    }

    file.

            close();

}

double Room::cal_Ec() const {
    double num = 0;
    for (const auto &p : polymer_list) {
        int length = p.chain.size();
        for (int j = 2; j < length; j++) {
            num += cal_ifline(p[j - 2]->location,
                              p[j - 1]->location, p[j]->location);
        }
    }

    return -num;

}

double Room::cal_dEp(deque<pair<vec, int>> &path) const {


    double num = 0;
    vec v1, v2;
    auto iter = path.begin();
    if (iter != path.end()) v1 = (*iter).first; else return num;
    iter++;
    while (iter != path.end()) {
        v2 = (*iter).first;
        num += ((this->*count_parallel))(v1, v2, path, 1);
        v1 = v2;
        iter++;
    }
#ifdef DEBUG
    cout<<__FUNCTION__<<num<<endl;
#endif
    return num;
}

double Room::cal_dEc(deque<pair<vec, int> > &path) const {

    double num = 0;
    vec v1, v2, v3;
    auto iter = path.begin();
    if (iter != path.end()) v1 = (*iter).first; else return num;
    iter++;
    if (iter != path.end()) v2 = (*iter).first; else return num;
    iter++;
    while (iter != path.end()) {
        v3 = (*iter).first;
        num += cal_ifline(v1, v2, v3);
        v1 = v2;
        v2 = v3;
        iter++;
    }
    return -num;
}

double Room::cal_dEf(deque<pair<vec, int>> path) const {
    double num = 0;
    vec v1, v2;
    auto iter = path.begin();
    if (iter != path.end()) v1 = (*iter).first; else return num;
    iter++;
    while (iter != path.end()) {
        v2 = (*iter).first;
        num += ((this->*count_parallel))(v1, v2, path, 1);
        v1 = v2;
        iter++;
    }
    return num;
}

double Room::cal_one_Ec(int i) const {//has some bugs
    double num = 0;
    for (const auto &p : polymer_list) {
        int length = p.chain.size();
        for (int j = 2; j < length; j++) {
            num += cal_ifline(p.chain[j - 2]->location,
                              p.chain[j - 1]->location, p.chain[j]->location);
        }
    }
    return -num;
}

double Room::cal_dEc_nearby(stack<pair<vec, int>> path) const {

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
    vec v1 = new_path[0].first;
    vec v2 = new_path.back().first;
    shared_ptr<Point> p1 = lattice[v1];
    shared_ptr<Point> p2 = lattice[v2];
    int chain_num = p1->chain_num;
    int length = polymer_list[chain_num].chain.size();
    if ((p1->pos_in_chain) > (p2->pos_in_chain)) {
        for (int i = p1->pos_in_chain + 1, j = 0; i < length && j < 2; i++, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_front(pair_data);
            old_path.push_front(pair_data);
        }
        for (int i = p2->pos_in_chain - 1, j = 0; i >= 0 && j < 2; i--, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_back(pair_data);
            old_path.push_back(pair_data);
        }


    } else if (p1->pos_in_chain == p2->pos_in_chain) {
        for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 2; i--, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_front(pair_data);
            old_path.push_front(pair_data);
        }
        for (int i = p2->pos_in_chain + 1, j = 0; i < length && j < 2; i++, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_back(pair_data);
            old_path.push_back(pair_data);
        }
    } else {
        for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 2; i--, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_front(pair_data);
            old_path.push_front(pair_data);
        }
        for (int i = p2->pos_in_chain + 1, j = 0; i < length && j < 2; i++, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_back(pair_data);
            old_path.push_back(pair_data);
        }

    }

    double dEc = cal_dEc(new_path) - cal_dEc(old_path);
    return dEc;


}

double Room::cal_dEp_nearby(stack<pair<vec, int>> path) {
//    printf(__FUNCTION__);

    if (path.empty()) return 0.0;
    deque<pair<vec, int>> new_path, old_path;

    stack<pair<vec, int>> do_path(path);

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

    vec v1 = new_path[0].first;
    vec v2 = new_path.back().first;
    shared_ptr<Point> p1 = lattice[v1];
    shared_ptr<Point> p2 = lattice[v2];
    int chain_num = p1->chain_num;
    int length = polymer_list[chain_num].chain.size();
    if (p1->pos_in_chain > p2->pos_in_chain) {
        for (int i = p1->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_front(pair_data);
            old_path.push_front(pair_data);
        }
        for (int i = p2->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_back(pair_data);
            old_path.push_back(pair_data);
        }

    } else if (p1->pos_in_chain == p2->pos_in_chain) {
        for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_front(pair_data);
            old_path.push_front(pair_data);
        }
        for (int i = p1->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_back(pair_data);
            old_path.push_back(pair_data);
        }
    } else {
        for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_front(pair_data);
            old_path.push_front(pair_data);
        }
        for (int i = p2->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
            auto pair_data = make_pair(polymer_list[chain_num][i]->location,
                                       polymer_list[chain_num][i]->true_position);

            new_path.push_back(pair_data);
            old_path.push_back(pair_data);
        }

    }

    double dEp1;
    try {
        dEp1 = cal_dEp(new_path);
    }
    catch (...) {
        cout << "new_path" << endl;
        cout << __FUNCTION__ << endl;
        throw;
    }
    stack<pair<vec, int>> re = repair(do_path);

    double dEp2;
    dEp2 = cal_dEp(old_path);

    try {

    }
    catch (...) {
        cout << "old_path";
    }
    repair(re);

    return dEp1 - dEp2;


}

//double Room::cal_dEb_nearby(stack<pair<vec,int>> path)
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
//	double Eb1 = cal_Eb_point(first_point_new, 2) + cal_Eb_point(last_point_old, 2);
//	stack<pair<vec,int>> re = repair(do_path);
//	double Eb2 = cal_Eb_point(last_point_old, 2) + cal_Eb_point(first_point_new, 2);
//	repair(re);
//	return Eb1 - Eb2;
//
//}

double Room::cal_Ep() const {
    double num = 0;
    deque<Position> a;

    for (const auto &p : polymer_list) {
        int length = p.chain.size();
        for (int j = 1; j < length; j++) {

            num += ((this->*count_parallel))(p[j - 1]->location, p[j]->location, a, 1);

        }
    }

    return num / 2.0;
}

//double Room::cal_Eb() const
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
        num += (this->*count_parallel)(polymer[j - 1]->location, polymer[j]->location, a, 0);
    }
    return num;
}

double Room::cal_one_Eb(int) const {
    return 0.0;
}

//TODO
double Room::count_parallel_nearby24(vec &point1, vec &point2,
                                     deque<pair<vec, int>> &que, int cal_type) const {

    double num_self = 0, num_others = 0;
    int chain_num;
    if (lattice[point1] == nullptr)
        throw "NULL";
    if (lattice[point1]->true_position != 0 || lattice[point2]->true_position != 0) {
//        cout<<"true_position!=0"<<endl;
//	    throw "true_position!=0";
        return 0;
    }
    chain_num = lattice[point1]->chain_num;
    int type1 = max(lattice[point1]->type, lattice[point2]->type);
    vec p1, p2;
    vec direction = cal_direction(point1, point2);


    //cout << direction<<endl;
    for (auto &direc : moves) {
        if ((direc == direction) || if_opposite(direc, direction) || (direc == vec{0, 0, 0})) {
            //cout << vec{ x,y,z };
            continue;
        }
        p1 = (point1 + direc) % shape;
        p2 = (point2 + direc) % shape;
        int result = get_side_num(p1, p2);
        if (result == -1) { continue; }
        else {
            int type2 = max(lattice[p1]->type, lattice[p2]->type);

            //cout << type1 << type2;

            double Ep_cross = Ep_matrix[type1][type2];
            if (result == chain_num) {

                if (find_in_que(que, make_pair(p1, lattice[p1]->true_position)) &&
                    find_in_que(que, make_pair(p2, lattice[p2]->true_position))) {
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
        //cout << num_others << ',' << num_self << endl;
    } else {
        return num_others + num_self;
    }
}

//double Room::count_parallel_nearby12(vec &point1, vec &point2,
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
//					if (find_in_que(que, p1) && find_in_que(que, p2)) {
//						num_self += 0.5;
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
//double Room::count_parallel_nearby8(vec &point1, vec &point2,
//	deque<vec> & que, int cal_type)const {//TODO
//
//	double num_self = 0, num_others = 0;
//	int chain_num;
//	if (lattice[point1] == nullptr)
//		throw "NULL";
//	chain_num = lattice[point1]->chain_num;
//	vec p1(point1), p2(point2);
//	vec dir = cal_direction(p1, p2);
//	int i = 0; j = 0;
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
//					if (find_in_que(que, p1) && find_in_que(que, p2)) {
//						num_self += 0.5;
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

//double Room::count_parallel_nearby8(vec &point1, vec &point2,
//	deque<vec> & que, int cal_type)const {
//
//	double num_self = 0, num_others = 0;
//	int chain_num;
//	chain_num = lattice[point1]->chain_num;
//	int type1 = lattice[point1]->type;
//	vec p1, p2;
//	vec direction = cal_direction(point1, point2);
//
//	for (auto &direc : moves) {
//		if (0==direc * direction) {
//			//cout << vec{ x,y,z };
//			p1 = (point1 + direc) % shape;
//			p2 = (point2 + direc) % shape;
//			int result = get_side_num(p1, p2);
//			if (result == -1) { continue; }
//			else {
//				int type2 = lattice[p1]->type;
//				double Ep_cross = Ep_matrix[type1][type2];
//				if (result == chain_num) {
//
//					if (find_in_que(que, p1) && find_in_que(que, p2)) {
//						num_self += 0.5*Ep_cross;
//					}
//					else {
//						num_self += Ep_cross;
//					}
//				}
//				else {
//					num_others += Ep_cross;
//				}
//		}
//		
//		}
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


double Room::cal_average_thick() const {
    return 0.0;
}

//double Room::count_parallel_B(vec &point1, vec &point2,
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
//		if ((direc == direction) || ((direc + direction) == vec{ 0,0,0 })) {
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
//					if (find_in_que(que, p1) && find_in_que(que, p2)) {
//						num_self += 0.5*pow(this->b2a, i);
//					}
//					else {
//						num_self += 1 * pow(this->b2a, i);
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



double Room::cal_Rg() const// ?????????
{
    throw "NOT DONE!";
    double num = 0;
    for (auto &p : polymer_list) {
        //vec center = p.get_center();

    }

    /*for p in polymer_list :
    center = np.mean(p.chain, axis = 0)
    centers = np.asarray([center] * p.length)
    rgb = p.chain - centers
    return np.mean(rgb*rgb)*dimension*/
    return 0.0;
}

double Room::cal_h2() const// ????????
{
    throw "NOT DONE!";
    double num = 0;
    for (auto &p : polymer_list) {
        num += distance_squre(p.chain[0]->location, p.chain.back()->location);
    }
    return num;
}

stack<pair<vec, int>> Room::repair(stack<pair<vec, int>> &path) {
    stack<pair<vec, int>> no_use;
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

inline double Room::cal_ifline(vec &p1, vec &p2, vec &p3) const {
    if (cal_direction(p1, p2) == cal_direction(p2, p3))
        return 0;
    return 1;
}

double Room::cal_Eb_point(vec &p, int type) const {
    vec point;//some bugs
    int i, j;
    i = lattice[p] == nullptr ? 0 : lattice[p]->type;
    double sum = 0;
    for (auto &direc : moves) {
        point = (p + direc) % shape;
        j = lattice[point] == nullptr ? 0 : lattice[point]->type;
        //cout << i << ',' << j << endl;
        sum += this->Eb_matrix[i][j];
    }
    //cout << "---dEp=" << sum << "---" << endl;
    return sum;
}

double Room::cal_Eb_point(vec &p) const {
    vec point;//some bugs
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
            cal_PSM_point(point->location);
        }
    }
    return 0.0;
}

double Room::cal_PSM_point(vec &p) const {
    return 0.0;
}


ostream &operator<<(ostream &o, Point &p) {
    o << p.location;
    return o;
}

ostream &operator<<(ostream &o, Polymer &p) {
    for (int i = 0; i < p.chain.size(); i++) {
        o << *(p.chain[i]) << ',';
    }
    return o;
}








