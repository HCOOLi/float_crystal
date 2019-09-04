#pragma once
#include<iostream>
#include"vec.h"
#include"polymer.h"
#include"point.h"
#include<set>
#include<stack>

#include<ctime>
using namespace std;

class Grid {
public:
    vec shape{};
	vector<vector< vector<shared_ptr< Point> > > > lattice;
    Grid() = default;

	shared_ptr< Point>  & operator[](const vec &P) {
		return lattice[P[0]][P[1]][P[2]];
	}
	shared_ptr< Point>   operator[](const vec &P)const {
		return lattice[P[0]][P[1]][P[2]];
	}
	void thread_yz(int i, int y, int z);
	Grid(int x, int y, int z) :shape(vec{ x,y,z })
	{
		lattice.resize(x);
		for (int i = 0; i < x; i++) {
			thread_yz(i, y, z);
		}
	}


};

class Room {
public:

	const vec shape;
	const int dimension = 3;
	Grid lattice;
	vector< vec > moves;
	//parameters
	const double Ec0 = 1.0;
	vector<vector<double> > Eb_matrix;
	vector<vector<double> > Ep_matrix;
	double (Room:: * count_parallel)(vec &, vec&, deque<vec>&, int) const;
	double (Room:: * cal_Eb_func)(vec &, vec&, deque<vec>&, int) const;

	vector<Polymer> polymer_list;

	Polymer &  get_polymer(int i) {
		return polymer_list[i];
	}

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
	void stepMove(vec &position, vec &next_position, stack<vec> & path);
	void localSnakeMove(int i, stack<vec > &path);
	void movie(int m, int n, double T);

    void preheat(int m);
	stack<vec> repair(stack<vec > &path);
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

	double cal_dEp(deque<vec> &path)const;
	double cal_dEc(deque<vec> &path)const;
	double cal_dEf(deque<vec> path) const;
	//double cal_dEb(deque<vec> &path)const;

	double cal_dEc_nearby(stack<vec> path)const;
	double cal_dEp_nearby(stack<vec> path);
	double cal_dEb_nearby(stack<vec> path);

	double cal_ifline(vec &p1, vec &p2, vec &p3)const;


	//double cal_Eb_point(vec & p, vec & p2) const;

	double cal_Eb_point(vec & p, int type) const;

	double cal_Eb_point(vec & p) const;

	/*double count_parallel_nearby(vec & point1, vec & point2, int i, int j, deque<vec>& que, int cal_type)const;
	double count_parallel_nearby24(vec & point1, vec & point2, int i, int j, const  deque<vec>& que, int cal_type)const;*/
	double count_parallel_nearby24(vec & point1, vec & point2,  deque<vec>& que, int cal_type) const;
	double count_parallel_nearby12(vec & point1, vec & point2, deque<vec>& que, int cal_type) const;
	double count_parallel_nearby8(vec & point1, vec & point2, deque<vec>& que, int cal_type) const;
	double count_parallel_nearby4(vec & point1, vec & point2, deque<vec>& que, int cal_type) const;
	//double count_parallel_nearby8(vec & point1, vec & point2, int i, int j, deque<vec>& que, int cal_type)const;
	double count_parallel_B(vec & point1, vec & point2, deque<vec>& que, int cal_type) const;
	double cal_average_thick()const;





	double cal_Rg()const;
	double cal_h2()const;
	double cal_PSM()const;
	double cal_PSM_point(vec &) const;

	vector<int> cal_thickness() const;

	vector<int> cal_thick_by_point() const;

	//load&save
	void save();
	void load();
	//python 

	int num_of_polymers()const {
		return polymer_list.size();
	}

    ~Room() = default;
};

