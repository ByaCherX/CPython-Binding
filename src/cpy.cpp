#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/unique_ptr.h>
#include "py_binding.h"

// Python header
#include <Python.h>

#include <iostream>
#include <string>
#include <array>
#include <vector>

// Utils library
#include "utils.h"

namespace nb = nanobind;
using namespace nb::literals;
constexpr auto txt_nrt = "\033[31m[nano_runtime]:\033[0m ";

// Example of using python module and handling
nb::list use_os_module() {
	// From Python load os module
	nb::module_ os = nb::module_::import_("os");

	// os.getcwd() call: get current directory
	nb::object cwd = os.attr("getcwd")();
	std::cout << "Current Directory: " << nb::cast<std::string>(cwd) << std::endl;

	// os.listdir() call: listing files
	nb::object files = os.attr("listdir")(".");
	auto files_c = nb::cast<nb::list>(files);
	return files_c;
}

// Global Memory Block
struct MemoryBlock {
	explicit MemoryBlock(size_t size = 16) {
		MemoryBlock::size = size;
		std::cout << "Memory Block Created" << std::endl;
	}
	~MemoryBlock() { 
		printf("\nClearing Memory Block size:%zi", size);
	}

	/* Adds a Python object to the vector named mem. 
	If a size is given, the mem vector length cannot exceed the specified size. 
	*/
	void addObject(nb::object py_object) {
		if (mem.size() < MemoryBlock::size) {
			mem.push_back(py_object);
		}
		else {
			std::cout << "Memory heap is full | size: "<< size << std::endl;
		}
	};

public:
	std::vector<nb::object> mem;  // * memory block
	size_t size;
};

/* It is not safe to define the value globally in this way. */
std::array<float, 8> arr = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8 };

void add_arr(int num) {
	for (auto iter = arr.begin(); iter != arr.end(); ++iter) {
		*iter += num;
	}
}
void print_arr() {
	for (const float &element : arr) {
		std::cout << element << " ";
	}
}

// Python object pointer and modify <<<
void modify_value(int* ptr) {
	std::cout << ptr << std::endl;
	if (ptr) {
		*ptr += 10;  // De�eri 10 art�r
	}
}

NB_MODULE(cpy, m) {
	// Using nanobind with CPython API
	m.def("add", [](nb::tuple args) -> nb::object {
		PyObject* result = my_add(args.ptr());
		if (!result) {
			throw std::runtime_error("Failed to execute my_add_api");
		}
		return nb::steal<nb::object>(result);
	});
	m.def("tuple_test", [](std::string el) -> nb::tuple {
		PyObject* t;
		
		t = PyTuple_New(3);
		PyTuple_SetItem(t, 0, PyLong_FromLong(1L));
		PyTuple_SetItem(t, 1, PyLong_FromLong(2L));
		PyTuple_SetItem(t, 2, PyUnicode_FromString(el.c_str()));
		return nb::steal<nb::tuple>(t);
	});
	m.def("nb_with_pyobject", []() -> nb::list {
		PyObject *list;

		//> tuple = Py_BuildValue("(iis)", 1, 2, "three");
		list = Py_BuildValue("[iis]", 1, 2, "three");

		nb::list m_list = nb::steal<nb::list>(list);
		m_list.attr("insert")(0,"value_1");
		PyList_Insert(list, 0, Py_BuildValue("s", "value_2"));
		PyList_Insert(m_list.ptr(), -1, Py_BuildValue("s", "value_3"));

		// is equal: (m_list.ptr() == list)
		
		return m_list;
	});

	m.def("add_arr", &add_arr, "num"_a = 1);
	m.def("print_arr", &print_arr);
	m.attr("version") = "1.0.0";
	m.def("modify_value", &modify_value, "Get Pointer and modify");
	m.def("use_os", &use_os_module);

	nb::class_<Core>(m, "Core")
		.def(nb::init<>(), "Default constructor")
		.def(nb::init<const char*>(), "Constructor with string input")
		.def(nb::init<const Core&>(), "Constructor Core&")
		.def(nb::init<Core&&>(), "Constructor Core&&")
		.def("core_move", &Core::Core_move, "Moving ownership")
		.def("print", &Core::Print, "Print the data")
		.def("__repr__",
			[](const Core& c) { return "<my_ext.Core at " + c.ref + " >"; })
		.def("__build", [](const Core& core) -> nb::object {
			auto callback = [ref_name = core.ref] {
				nb::print(nb::str("{}: created to C memory").format(ref_name));
			};
			return nb::cpp_function(callback);
			})
		.def_rw("ref", &Core::ref);

	nb::class_<MemoryBlock>(m, "MemoryBlock")
		.def(nb::init<>(), "Default constructor")
		.def(nb::init<size_t>(), "Heap Limited constructor")
		.def("add", &MemoryBlock::addObject)
		.def("__repr__", [](const MemoryBlock& memBlock) {
			return "non_initialized";
			})
		.def_ro("mem", &MemoryBlock::mem)
		.def_ro("size", &MemoryBlock::size);

	m.def("pydef", &pydef);
	
	m.def("cpy_interpr", &cpy_interpr);
	m.def("cpy_interpr_code_object", &cpy_interpr_code_object);

	m.def("ref_count", [](nb::object& a) {
		return Py_REFCNT(a.ptr());
		});
	m.def("py_err_handling", &py_err_handling);
}