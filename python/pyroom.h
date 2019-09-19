#pragma once
#include"boost/python.hpp"
#include"../cpp/room.h"
#include<map>

using namespace std;

namespace py = boost::python;

template<typename T>
vector<T> pylist2vector(py::list list);

template<typename T>
vector<vector<T>> pylist2matrix(py::list listoflist);

template<typename T>
py::list vector2pylist(vector<T> matrix);
template <typename T>
py::list vector2pylist(vector<vector<T>> matrix);
template <typename key,typename value>
map<key,value> pydict2map(py::dict list);




class pyroom :public Room {
public:
	py::list * results;

	pyroom(int x, int y, int z, py::list Ep_list, py::list Eb_list, int type);

    void Ep_setter(py::list Ep_list);

    py::list Ep_getter();

    void Eb_setter(py::list Eb_list);

    py::list Eb_getter();


    void py_input_one_ECC(int x, int y, int z, int length, int direction, py::list type, int movable);

    void input_one_FCC(int x, int y, int z, int length, int direction, int fold_direction, py::list, int movable);
	void construct_by_pylist(py::list chain_list);

	py::list get_list() const;

    py::object get_result() const {
        return py::object(*results);
    }
};
