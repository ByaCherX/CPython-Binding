#include <Python.h>

// Python/C API defination
PyObject* my_add(PyObject* args);
int cpy_interpr();
int cpy_interpr_code_object();
int py_err_handling(const int err_method);