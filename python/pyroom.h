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

    py::list Ep_getter()const;

    void Eb_setter(py::list Eb_list);

    py::list Eb_getter()const;


    void py_input_one_ECC(int x, int y, int z, int length, int direction, py::list type, int movable);

    void input_one_FCC(int x, int y, int z, int length, int direction, int fold_direction, py::list, int movable);
	void construct_by_pylist(py::list chain_list);

	py::list get_list() const;

    py::object get_result() const {
        return py::object(*results);
    }
    double cal_Ep_py(){
        vector<int> countlist;
        int count = 0;
        for (auto &chain:polymer_list){
            if( count != 0){
                countlist.push_back(count);
            }
            shared_ptr<Point> last_point=nullptr;
            count=0;
            if(!chain.chain.empty()){
                last_point = chain[0];
            }
            int type=last_point->type;


            }
        return 0;
    }

//        count = 0
//        if chain:
//            last_point = chain["c"][0].copy()['p']
//        type = chain["c"][0].copy()["t"]
//        if type == 1:
//
//        this_color = 'any'
//        elif type == 2:
//        this_color = 'grey'
//
//
//        elif type == 3:
//        continue
//# this_color = color.red
//
//# c = curve(color=this_color, radius=0.1)
//        last_point = None
//        if count != 0:
//        countlist.append(count)
//        count = 0
//        else:
//        continue
//
//        for pointinfo in chain["c"]:
//        point = pointinfo['p']
//        type = pointinfo["t"]
//        if type == 1:
//        pass
//# this_color = color.yellow
//        elif type == 2:
//# pass
//        pass
//# this_color = color.blue
//        elif type == 3:
//        continue
//        if point[0] != layer:
//        continue
//        else:
//# sphere(pos=vector(point[0], point[1], point[2]), color=this_color, radius=0.2)
//        if last_point != None:
//        if (self.if_out_of_range(last_point, point)):
//        pass
//        else:
//        last_point = None
//        if last_point != None:
//        if point[1] == last_point[1] and 8 <= min(point[2], last_point[2]) and max(point[2],
//                                                                                   last_point[2]) < 29:
//        if this_color == 'grey':
//        ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="grey")
//        count += 1
//        else:
//        ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="red")
//        count += 1
//        else:
//        if this_color == 'grey':
//        ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="grey")
//        else:
//        ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="blue")
//        last_point = point.copy()
//
//        point2 = None
//        if count != 0:
//        countlist.append(count)
//        count = 0
//        plt.figure()
//        histlist, bins, _ = plt.hist(countlist, bins=myrange(1, 21, 1))
//# print(histlist, bins)
//        plt.close()
//# countlist
//
//        ax2.bar(bins[:-1], np.asarray(histlist) * np.asarray(bins[:-1]))
//        plt.ylim(0, 80)
//        plt.xlim(0, 21)

};
