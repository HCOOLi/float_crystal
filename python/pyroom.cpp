#include<boost/python.hpp>
#include <utility>
#include"pyroom.h"
using namespace std;

namespace py = boost::python;



template<typename key, typename value>
map<key, value> pydict2map(py::dict list) {
    map<key,value> result;
    throw "NOT done!";
    return result;
//	for (int i = 0; i < py::len(list); i++) {
//		int x = py::extract<int>(list[i]);
//		result.push_back(x);
//	}
//	return result;

}

template<typename T>
vector<T> pylist2vector(py::list list) {//TODO
    vector<T> result;
    for (int i = 0; i < py::len(list); i++) {
        T x = py::extract<T>(list[i]);
        result.push_back(x);
    }
    return result;
}

template<typename T>
vector<vector<T>> pylist2matrix(py::list listoflist) {//TODO
    vector<vector<T>> result;
    result.resize(py::len(listoflist));
    for (int i = 0; i < py::len(listoflist); i++) {
        py::list list  = py::extract<py::list>(listoflist[i]);
        result[i].resize(py::len(list));
        for(int j=0;j<py::len(list);j++){
            T data  = py::extract<T>(list[j]);
            result[i][j]=data;
        }

    }

    return result;
}

template<typename T>
py::list vector2pylist(vector<vector<T>> matrix) {//TODO
    py::list* result=new(py::list);
    for(auto row:matrix){
        py::list temp;
        for(auto data:row){
            temp.append(data);
        }
        result->append(temp);
    }
    return *result;
}

template<typename T>
py::list vector2pylist(vector<T> matrix) {//TODO
    py::list *result=new(py::list);
    for(auto x:matrix){
        result->append(x);
    }
    return *result;
}


pyroom::pyroom(int x, int y, int z, py::list Ep_list, py::list Eb_list, int type) : Room(x, y, z, type) {

    Ep_matrix=pylist2matrix<double>(Ep_list);
    Eb_matrix=pylist2matrix<double>(Eb_list);
    results = new py::list();
    cout << "construction complete" << endl;
    //cout<< time(NULL);
}



void pyroom::construct_by_pylist(py::list chain_list) {

	polymer_list.clear();
    lattice = Grid<shared_ptr<Point>>(shape[0], shape[1], shape[2]);
	for (int i = 0; i < py::len(chain_list); i++) {
		py::dict chain_dict = py::extract<py::dict>(chain_list[i]);
		py::list chain = py::extract<py::list>(chain_dict["c"]);
		Polymer p;
		p.chain.resize(py::len(chain));
		int chain_num = polymer_list.size();
		for (int j = 0; j < py::len(chain); j++) {
			py::dict point_in_list = py::extract<py::dict>(chain[j]);
			py::list position = py::extract<py::list>(point_in_list["p"]);
			vec point;
			for (int k = 0; k < py::len(position); k++) {
				int x = py::extract<int>(position[k]);
				point[k] = x;
			}
			int type = py::extract<int>(point_in_list["t"]);
            int movable = py::extract<int>(point_in_list["m"]);
            p[j] = set_point(point, chain_num, j, type, movable, 0);


		}
		polymer_list.emplace_back(move(p));
	}

}


void pyroom::py_input_one_ECC(int x, int y, int z, int length, int direction, py::list type, int movable) {
    //cout << __FUNCTION__ << endl;
    //cout << typearray.size() << ',' << py::len(type) << endl;
    vector<int> typearray = pylist2vector<int>(std::move(type));
    auto temp = vec{x, y, z};
    Room::input_one_ECC(temp, length, direction, typearray, movable);
    //cout << "input finished" << endl;

}

void pyroom::input_one_FCC(int x, int y, int z, int length, int direction, int fold_direction, py::list type, int moveable)
{
    vector<int> typearray = pylist2vector<int>(type);
	//cout << __FUNCTION__ << endl;
	//cout << typearray.size() << ',' << py::len(type) << endl;
	Room::input_one_FCC(vec{ x,y,z }, length, direction, fold_direction, typearray, moveable);
	//cout << "input finished"<< endl;
}

void pyroom::Ep_setter(py::list Ep_list) {

    Ep_matrix=pylist2matrix<double>(Ep_list);
    cout<<"Ep_matrix:"<<endl;
    for(auto row:Ep_matrix) {
        for (auto data:row) {
            cout << data << '\t';
        }
        cout << endl;
    }
}

void pyroom::Eb_setter(py::list Eb_list) {
    Ep_matrix=pylist2matrix<double>(Eb_list);
    cout<<"Eb_matrix:"<<endl;
    for(auto row:Eb_matrix) {
        for (auto data:row) {
            cout << data << '\t';
        }
        cout << endl;
    }

}

py::list pyroom::get_list() const {
    py::list result;
    for (const auto &polymer : polymer_list) {

        py::dict json;
        py::list chain_list;
        for (int j = 0; j < polymer.chain.size(); j++) {

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

        result.append(json);
    }

    //
    return result;
}

py::list pyroom::Ep_getter() const {
    return vector2pylist(Ep_matrix);
}

py::list pyroom::Eb_getter() const {
    return vector2pylist(Eb_matrix);
}
