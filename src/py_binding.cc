#include <nanobind/nanobind.h>

// Python Header
#include <Python.h>

namespace nb = nanobind;

// Example Python/C API
PyObject* my_add(PyObject* args) {
    int a, b;

    // Parse arguments from Python
    if (!PyArg_ParseTuple(args, "ii", &a, &b)) {
        return NULL;
    }

    return PyLong_FromLong(a + b);
}

// Python High Level Interpreter 
int cpy_interpr() {
    // Initialize Python Interpreter
    Py_Initialize();

    // Run Python Code
    const char* code = "a = 10; b = 5; c = a + b";
    PyObject* globals = PyDict_New();
    PyObject* locals = PyDict_New();

    PyObject* result = PyRun_StringFlags(
        code,
        Py_file_input,
        globals,
        locals,
        NULL
    );
    if (result) 
    {
        PyObject* z_value = PyDict_GetItemString(locals, "c");
        if (z_value) 
        {
            long value = PyLong_AsLong(z_value);
            printf("[cpy] z = %ld\n", value);
        }
        Py_DECREF(result);
    } else {
        PyErr_Print(); // if error exists print directly
    }

    // Clear Memory
    Py_DECREF(globals);
    Py_DECREF(locals);

    // Finalize Python Interpreter
    // It directly closes the python interpreter.
    //Py_Finalize(); 
    return 0;
}

// Python High Level Layer with Python Code Object
int cpy_interpr_code_object() {
    // Initialize Python Interpreter
    Py_Initialize();

    // Python Code
    const char* code = R"(
x = 42
y = x + 10
z = y * 2
)";
    // filename is important when debugging is required
    PyObject* filename = PyUnicode_FromString("<embedded>"); 
    int start = Py_file_input;

    // Code Compiling
    PyObject* code_obj = Py_CompileStringObject(
        code, 
        filename, 
        start, 
        NULL, 
        0
    );
    if (code_obj) {
        PyObject* globals = PyEval_GetGlobals(); //PyDict_New();
        PyObject* locals = PyEval_GetLocals(); //PyDict_New();

        // Run Compiled Code
        PyObject* result = PyEval_EvalCode(code_obj, globals, locals);
        if (result) {
            PyObject* z_value = PyDict_GetItemString(locals, "z");
            if (z_value) {
                long z = PyLong_AsLong(z_value);
                printf("[cpy] z = %ld\n", z);
            }
            Py_DECREF(result);
        }
        else {
            PyErr_Print();
        }

        Py_DECREF(code_obj);
        Py_DECREF(globals);
        Py_DECREF(locals);
    }
    else {
        PyErr_Print();
    }

    Py_DECREF(filename);

    return 0;
}

int pyerr1() {
    PyObject* pModule = PyImport_ImportModule("non_existent_module"); // Python/C API
    //nb::module_ pModuleNB = nb::module_::import_("nmodule");
    if (!pModule) {
        PyErr_Print();  // Printing Error message
        return -1;      // exit with error code
    }
    return 1;
}

// Python Error Handling
int py_err_handling(const int err_method = 1) {
    try {
        nb::object file = nb::module_::import_("io")
            .attr("open")("swapfile.bin");
        nb::object text = file.attr("read")();
        nb::print(text);
        file.attr("close");
    }
    catch (nb::python_error& e) {
        if (e.matches(PyExc_FileNotFoundError) && err_method <= 2) {
            // -> Method 1
            if (err_method == 1)
                throw e;

            // -> Method 2 (Python API like)
            PyErr_SetString(PyExc_FileNotFoundError, "swapfile.bin not found !!");
            throw nb::python_error();
        }
        else if (e.matches(PyExc_PermissionError)) {
            // -> Method 3
            nb::raise_from(e, e.type(), "File permission error with %s", "Arg..");
            nb::print("swapfile.bin found but not accessible");  // no print
        }
        else {
            throw;
        }
    }
    return 1;
}




