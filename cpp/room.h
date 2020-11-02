#pragma once
#include <iostream>
#include "vec.h"
#include "polymer.h"
#include "point.h"
#include <set>
#include <stack>
#include "ConnectedComponent.h"

#include <ctime>
using namespace std;
#ifdef TRUE_POSITION 
typedef pair<vec, int> Position;
#else
typedef vec Position;
#endif
template <typename T>
class Grid
{
public:
	vec shape{};
	vector<T> lattice;
	Grid() = default;

	T &operator[](const vec &P)
	{
		return lattice[P[0] * shape[1] * shape[2] + P[1] * shape[2] + P[2]];
	}

	T operator[](const vec &P) const
	{
		return lattice[P[0] * shape[1] * shape[2] + P[1] * shape[2] + P[2]];
	}

	Grid(int x, int y, int z) : shape(vec{x, y, z})
	{
		lattice.resize(x * y * z);
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				for (int k = 0; k < z; k++)
				{
					lattice[i * y * z + j * z + k] = nullptr;
				}
			}
		}
	}
};

class Room
{
public:
	vec shape{};
	const int dimension = 3;
	Grid<Point *> lattice;
	vector<vec> moves;
	//parameters
	double Ec0 = 1.0;
	#ifdef TRUE_POSITION
	int q = 27; //27的倍数
	#endif
	vector<vector<double>> Eb_matrix;
	vector<vector<double>> Ep_matrix;
	vector<vector<double>> Ee2e_matrix;

	double cal_Ee2e() ;
	double count_e2e(const vec &,const vec &) ; //TODO
	double cal_dEe2e(deque<Position> &path) ;
	double cal_dEe2e_nearby(stack<Position> path) ;

	double (Room::*count_parallel)(const vec &,const vec &, deque<Position> &, int) const {};

	double (Room::*cal_Eb_func)(const vec &,const vec &, deque<Position> &, int) const {};

	vector<Polymer> polymer_list;

	Polymer &get_polymer(int i)
	{
		return polymer_list[i];
	}

	Room() = default;
	Room(int x, int y, int z, int type = 24);
	Room(int x, int y, int z, vector<vector<double>> Ep, vector<vector<double>> Eb,vector<vector<double>> Ee2e, int type);

	void initmoves();
	//some useful functions
	bool intersect(const vec &point1,const vec &point2) const;
	int get_chain_num(const vec &p1,const vec &p2) const;
	vec cal_direction(const vec &point1, const vec &point2)const{
    vec temp;
    for (int i = 0; i < 3; i++)
    {
        temp[i] = abs(point2[i] - point1[i]) <= 1 ? point2[i] - point1[i] : (point2[i] + 1) % shape[i] - (point1[i] + 1) % shape[i];
    }
    return temp;
	}
	int distance_squre(const vec &p1,const vec &p2) const
	{
		vec direction = cal_direction(p2, p1);
		return direction * direction;
	}
	bool canMove(const vec &point,const vec &direction) const;
	//input chains
	void inputECC(int num, int length);
	void inputcircle(int num, int length);
	void input_stop_chain();
	void input_stop_chain2();
	void input_one_circle(vec init, int length, int direction, int moveable);
	void input_one_ECC(vec init, int length, int direction, vector<int> list, int moveable);
	void input_one_FCC(vec init, int length, int direction, int fold_direction, vector<int>, int moveable);

	//move

	void movie(int m, int n, double T,string path);

	void preheat(int m,int n);
	void init_queue(vector<array<int,2>> &queue);
	//calculate something
	void lazy_delete_chain(int i)
	{
		for (auto &p : polymer_list[i].chain)
		{
			if (lattice[p.location] != &p)
			{
				throw "somethin wrong";
			}
			else
			{
				lattice[p.location] = nullptr;
			}
		}

		polymer_list[i].chain.clear();
	}

	double cal_Ec() const;
	double cal_one_Ec(int) const;
	double cal_dEc(deque<Position> &path) const;
	double cal_dEf(deque<Position> &path) const;
	double cal_dEc_nearby(stack<Position> path) const;
	double cal_ifline(const vec &p1,const  vec &p2,const vec &p3) const;

	double cal_Ep() const;
	double cal_one_Ep(int) const;
	double cal_dEp_nearby(stack<Position> path);
	double cal_dEp(deque<Position> &path) const;
	double cal_dEb_nearby(stack<vec> path);

	double cal_Eb() const;
	double cal_one_Eb(int) const;
	double cal_Eb_point(const vec &p, int type) const;

	double cal_Eb_point(const vec &p) const;

	//double cal_dEb(deque<vec> &path)const;

	//double cal_Eb_point(vec & p, vec & p2) const;

	/*double count_parallel_nearby(vec & point1, vec & point2, int i, int j, deque<vec>& que, int cal_type)const;
	double count_parallel_nearby24(vec & point1, vec & point2, int i, int j, const  deque<vec>& que, int cal_type)const;*/
	double count_parallel_nearby24(const vec &point1,const vec &point2, deque<Position> &que, int cal_type) const;

	//double count_parallel_nearby8(vec & point1, vec & point2, int i, int j, deque<vec>& que, int cal_type)const;
	//	double count_parallel_B(vec & point1, vec & point2, deque<vec>& que, int cal_type) const;
	double cal_average_thick() const;

	double cal_Rg() const;
	double cal_h2() const;
	double cal_PSM() const;
	double cal_PSM_point(const vec &) const;

	int num_of_polymers() const
	{
		return polymer_list.size();
	}

	~Room() = default;
#ifdef TRUE_POSITION
	void stepMove(vec &position, vec &next_position, stack<Position> &path, int true_p);
#else
	void stepMove(vec &position, vec &next_position, stack<Position> &path);

#endif
	stack<Position> repair(stack<Position> &path);

	void localSnakeMove(int i,int st, stack<Position> &path);

	// double Room::count_parallel_nearby4(vec &point1, vec &point2,
	// 									deque<Position> &que, int cal_type) const;
	double count_parallel_nearby8(vec &point1, vec &point2, deque<Position> &que, int cal_type) const;

	void save(string filename) const;

	void load(string filename);

	int get_max_nucleus(int layer);

	int Room::get_h2(int n  )
	int get_max_straight_length ();
	double get_average_straight_length ();



	
	double cal_if_inter(int i) const;

	int cal_crystallinity(int q) const;
};
