#include<string>
#include <boost/python.hpp>
#include"pyroom.h"

namespace py=boost::python;



BOOST_PYTHON_MODULE(crystal)
{
	py::class_<Point>("Point");

	py::class_<Polymer>("Polymer")
		//.def("get_list", &Polymer::get_list)
		;
	py::class_<pyroom>("pyroom", py::init<int, int, int, py::list, py::list,int >())
		.def("inputECC", &pyroom::inputECC)
		.def("input_one_ECC", &pyroom::input_one_ECC)
		.def("input_one_FCC", &pyroom::input_one_FCC)
		.def("input_stop_chain",&pyroom::input_stop_chain)
		.def("input_stop_chain2", &pyroom::input_stop_chain2)
		//.def("cal_thick_by_point",&pyroom::cal_thick_by_point)
		.def("construct_by_pylist",&pyroom::construct_by_pylist)
		//.def("cal_thickness", &pyroom::cal_thickness)
		.def("movie", &pyroom::movie)
		.def("preheat", &pyroom::preheat)
		.def("delete_chain",&pyroom::lazy_delete_chain)
		.def("get_list",&pyroom::get_list)
		.def("cal_Ec",&pyroom::cal_Ec)
		.def("cal_Ep",&pyroom::cal_Ep)
		.def("ca_Eb",&pyroom::cal_Eb)
		/*.def("get_result",&pyroom::get_result)*/
		//.def("get_polymer",&pyroom::get_polymer, py::return_internal_reference<>())
		.def("get_num_of_polymers",&pyroom::num_of_polymers)
		;

}