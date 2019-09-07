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

    void py_input_one_ECC(int x, int y, int z, int length, int direction, py::list type, int moveable);

    void input_one_FCC(int x, int y, int z, int length, int direction, int fold_direction, py::list, int movable);
	void construct_by_pylist(py::list chain_list);

	py::list get_list() const {
        py::list result;
        for (const auto &polymer : polymer_list) {
			
			py::dict json;
			py::list chain_list;
            for (int j = 0; j < polymer.length; j++) {
					
						py::dict dic;
						py::list py_position;
						for (int k = 0; k < 3; k++) {
                            py_position.append(polymer.chain[j]->location[k]);
						}
						dic["p"] = py_position;
                dic["t"] = polymer.chain[j]->type;
                dic["m"] = polymer.chain[j]->movable;
					
					chain_list.append(dic);
				}
			json["c"] = chain_list;
            json["t"] = polymer.type;

            result.append(json);
		}

			//
        return result;
	}
	//py::object get_result() const {

	//	return py::object(*results);
	//}
};
