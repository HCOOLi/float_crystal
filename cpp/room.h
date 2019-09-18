#pragma once
#include<iostream>
#include"vec.h"
#include"polymer.h"
#include"point.h"
#include<set>
#include<stack>

#include<ctime>
using namespace std;

typedef pair<vec, int> Position;

template<typename T>
class Grid {
public:
    vec shape{};
    vector<T> lattice;
    Grid() = default;

    T &operator[](const vec &P) {
        return lattice[P[0] * shape[1] * shape[2] + P[1] * shape[2] + P[2]];
    }

    T operator[](const vec &P) const {
        return lattice[P[0] * shape[1] * shape[2] + P[1] * shape[2] + P[2]];
    }

    Grid(int x, int y, int z) : shape(vec{x, y, z}) {
        lattice.resize(x * y * z);
		for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                for (int k = 0; k < z; k++) {
                    lattice[i * y * z + j * z + k] = nullptr;
                }
            }
		}
	}


};

class Room {
public:

    vec shape{};
	const int dimension = 3;
    Grid<shared_ptr<Point>> lattice;
	vector< vec > moves;
	//parameters
    double Ec0 = 1.0;
    int q = 27;//27的倍数
	vector<vector<double> > Eb_matrix;
	vector<vector<double> > Ep_matrix;

    double (Room::* count_parallel)(vec &, vec &, deque<Position> &, int) const {};

    double (Room::* cal_Eb_func)(vec &, vec &, deque<Position> &, int) const {};


	vector<Polymer> polymer_list;

	Polymer &  get_polymer(int i) {
		return polymer_list[i];
	}

    Room() = default;
    Room(int x, int y, int z, int type = 24);
	Room(int x, int y, int z, vector<vector<double> > Ep, vector<vector<double> > Eb,int type);

	void initmoves();


	shared_ptr<Point> set_point(vec location, int chain_num, int pos_in_chain, int type, int moveable,int true_p);

	//some useful functions
	bool intersect(vec &point1, vec &point2)const;
	int get_side_num(vec & p1, vec & p2) const;
	vec cal_direction(const vec & point1, const vec & point2) const;
	int distance_squre(vec &p1, vec &p2)const {
		vec direction = cal_direction(p2, p1);
		return direction * direction;
	}
	bool canMove(vec &point, vec &direction)const;
	//input chains
	void inputECC(int num, int length);
	void inputcircle(int num, int length);
	void input_stop_chain();
	void input_stop_chain2();
	void input_one_circle(vec init, int length, int direction, int moveable);
	void input_one_ECC(vec init, int length, int direction, vector<int> list, int moveable);
	void input_one_FCC(vec init, int length, int direction, int fold_direction, vector<int>, int moveable);
	

	//move


	void movie(int m, int n, double T);

    void preheat(int m);
	//calculate something
	void lazy_delete_chain(int i) {
		for (auto &p : polymer_list[i].chain) {
			if (lattice[p->location] != p) {
				throw "somethin wrong";
			}
			else {
				lattice[p->location] = nullptr;
				p = nullptr;
			}
		}
		polymer_list[i].length = 0;
		polymer_list[i].chain.clear();

		//cout << polymer_list.size();
	}


	double cal_Ec()const;
	double cal_Ep()const;
	double cal_Eb()const;


	double cal_one_Ec(int)const;
	double cal_one_Ep(int)const;
	double cal_one_Eb(int)const;

	//double cal_dEb(deque<vec> &path)const;


	double cal_dEb_nearby(stack<vec> path);

	double cal_ifline(vec &p1, vec &p2, vec &p3)const;


	//double cal_Eb_point(vec & p, vec & p2) const;

	double cal_Eb_point(vec & p, int type) const;

	double cal_Eb_point(vec & p) const;

	/*double count_parallel_nearby(vec & point1, vec & point2, int i, int j, deque<vec>& que, int cal_type)const;
	double count_parallel_nearby24(vec & point1, vec & point2, int i, int j, const  deque<vec>& que, int cal_type)const;*/
    double count_parallel_nearby24(vec &point1, vec &point2, deque<Position> &que, int cal_type) const;

	//double count_parallel_nearby8(vec & point1, vec & point2, int i, int j, deque<vec>& que, int cal_type)const;
//	double count_parallel_B(vec & point1, vec & point2, deque<vec>& que, int cal_type) const;
	double cal_average_thick()const;





	double cal_Rg()const;
	double cal_h2()const;
	double cal_PSM()const;
	double cal_PSM_point(vec &) const;



	int num_of_polymers()const {
		return polymer_list.size();
	}

    ~Room() = default;

    void stepMove(vec &position, vec &next_position, stack<Position> &path, int true_p);

    stack<Position> repair(stack<Position> &path);

    void localSnakeMove(int i, stack<Position> &path);

    double cal_dEp_nearby(stack<Position> path);

    double cal_dEp(deque<Position> &path) const;

    double cal_dEc(deque<Position> &path) const;

    double cal_dEf(deque<Position> path) const;

    double cal_dEc_nearby(stack<Position> path) const;

    double count_parallel_nearby8(vec &point1, vec &point2, deque<vec> &que, int cal_type) const;

    void save(string filename);

    void load(string filename);
};

