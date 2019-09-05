#include<boost/python.hpp>
#include"pyroom.h"
using namespace std;

namespace py = boost::python;

pyroom::pyroom(int x, int y, int z, py::list Ep, py::list Eb, int type) : Room(x, y, z, type) {
    Ep_matrix.resize(py::len(Ep));
    cout << py::len(Ep);
    for (int i = 0; i < py::len(Ep); i++) {
        py::list Ep_array = py::extract<py::list>(Ep[i]);
        Ep_matrix[i].resize(py::len(Ep_array));
        for (int j = 0; j < py::len(Ep_array); j++) {
            try {
                Ep_matrix[i][j] = py::extract<double>(Ep_array[j]);
            }
            catch (exception &e) {
                cout << e.what();
                Ep_matrix[i][j] = py::extract<int>(Ep_array[j]);
            }
        }
    }
    Eb_matrix.resize(py::len(Eb));
    for (int i = 0; i < py::len(Eb); i++) {
        py::list Eb_array = py::extract<py::list>(Eb[i]);
        Eb_matrix[i].resize(py::len(Eb_array));
        for (int j = 0; j < py::len(Eb_array); j++) {
            Eb_matrix[i][j] = py::extract<double>(Eb_array[j]);
        }
    }
    results = new py::list();
    cout << "construction complete" << endl;
    //cout<< time(NULL);
}


template<typename T>
vector<T> pylist2vector(py::list list) {
    vector<T> result;
	for (int i = 0; i < py::len(list); i++) {
        T x = py::extract<T>(list[i]);
		result.push_back(x);
	}
	return result;


}
//map<string,double> pydict2map(py::dict list) {
//	vector<int> result;
//	for (int i = 0; i < py::len(list); i++) {
//		int x = py::extract<int>(list[i]);
//		result.push_back(x);
//	}
//	return result;
//
//}

void pyroom::construct_by_pylist(py::list chain_list) {

	polymer_list.clear();
	lattice = Grid(shape[0], shape[1], shape[2]);
	for (int i = 0; i < py::len(chain_list); i++) {
		py::dict chain_dict = py::extract<py::dict>(chain_list[i]);
		py::list chain = py::extract<py::list>(chain_dict["chain"]);
		int type = py::extract<int>(chain_dict["type"]);
		Polymer p;
		p.chain.resize(py::len(chain));
		p.length = py::len(chain);
		int chain_num = polymer_list.size();
		for (int j = 0; j < py::len(chain); j++) {
			py::dict point_in_list = py::extract<py::dict>(chain[j]);
			py::list position = py::extract<py::list>(point_in_list["position"]);
			//{}
			vec point;
			for (int k = 0; k < py::len(position); k++) {
				int x = py::extract<int>(position[k]);
				point[k] = x;
			}
			int type = py::extract<int>(point_in_list["type"]);
            p[j] = set_point(point, chain_num, j, type, 0, 0);


		}
		polymer_list.emplace_back(move(p));
	}

}




void pyroom::input_one_ECC(int x, int y, int z, int length, int direction, py::list type, int moveable)
{
    vector<int> typearray = pylist2vector<int>(type);
	//cout << __FUNCTION__ << endl;
	//cout << typearray.size() << ',' << py::len(type) << endl;
	Room::input_one_ECC(vec{ x,y,z }, length, direction, typearray, moveable);
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
