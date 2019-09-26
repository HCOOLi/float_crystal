#pragma once
#include<iostream>
#include"vec.h"
#include"polymer.h"
#include"point.h"
#include<set>
#include<stack>

#include<memory>
using namespace std;



class Polymer {//a polymer 
public:
	vector<shared_ptr< Point>> chain;

    Polymer() noexcept = default;;
	Polymer(const Polymer & p) {
		chain = p.chain;
	};

    Polymer &operator=(const Polymer &p) = default;

    Polymer &operator=(Polymer &&p) noexcept {
		chain = move(p.chain);
		p.chain.clear();
		return *this;
	}

    Polymer(Polymer &&p) noexcept {
		chain = move(p.chain);

		p.chain.clear();
	};



	shared_ptr< Point>& operator[](int i) { return chain[i]; }
	shared_ptr< Point> operator[](int i) const { return chain[i]; }

	Point &  get_polymer(int i) {
		return *chain[i];
	}
};
ostream & operator<<(ostream & o, Polymer& p);
//py::dict get_list()const {
//	py::dict json;
//	py::list chain_list;
//	for (int i = 0; i < length; i++) {
//		chain_list.append(chain[i]->get_list());
//	}
//	json["chain"] = chain_list;
//	json["type"] = this->type;
//	return json;
//}