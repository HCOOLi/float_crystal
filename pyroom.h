#pragma once
#include"boost/python.hpp"
#include"../cpp/room.h"
#include<map>

using namespace std;

namespace py = boost::python;


class pyroom :public Room {
public:
	py::list * results;

	pyroom(int x, int y, int z, py::list Ep, py::list Eb,int type);

	void input_one_ECC(int x, int y, int z, int length, int direction, py::list, int moveable);
	void input_one_FCC(int x, int y, int z, int length, int direction, int fold_direction, py::list, int moveable);
	void construct_by_pylist(py::list chain_list);
	py::list cal_thick_by_point()const;
	py::list cal_thickness()const;
	py::list get_list() const {
		py::list a;
		for (int i = 0; i < polymer_list.size(); i++) {
			
			py::dict json;
			py::list chain_list;
				for (int j = 0; j < polymer_list[i].length; j++) {
					
						py::dict dic;
						py::list py_position;
						for (int k = 0; k < 3; k++) {
							py_position.append(polymer_list[i].chain[j]->location[k]);
						}
						dic["p"] = py_position;
						dic["t"] = polymer_list[i].chain[j]->type;
						dic["m"] = polymer_list[i].chain[j]->moveable;
					
					chain_list.append(dic);
				}
			json["c"] = chain_list;
			json["t"] = polymer_list[i].type;
	
			a.append(json);
		}

			//
		return a;
	}
	//py::object get_result() const {

	//	return py::object(*results);
	//}
};
