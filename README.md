# CPython-Binding

It uses nanobind to make it simpler to create plugins using the Python/C API. In this way, it reduces the complexity a little and makes the code easier to write.

This project is a plugin for Python with nanobind and an example of how you can embed python into existing C++ software.

> [!Warning]
> All the codes here are written for testing. No guarantee is given, all responsibility is on you.

### What is Python/C API
The Application Programmer’s Interface to Python gives C and C++ programmers access to the Python interpreter at a variety of levels. The API is equally usable from C++, but for brevity it is generally referred to as the Python/C API. There are two fundamentally different reasons for using the Python/C API. The first reason is to write extension modules for specific purposes; these are C modules that extend the Python interpreter. This is probably the most common use. The second reason is to use Python as a component in a larger application; this technique is generally referred to as embedding Python in an application.

Writing an extension module is a relatively well-understood process, where a “cookbook” approach works well. There are several tools that automate the process to some extent. While people have embedded Python in other applications since its early existence, the process of embedding Python is less straightforward than writing an extension.

Many API functions are useful independent of whether you’re embedding or extending Python; moreover, most applications that embed Python will need to provide a custom extension as well, so it’s probably a good idea to become familiar with writing an extension before attempting to embed Python in a real application.

**Example Python/C API**
```cpp
// Opaque Pointer ---|
//  ↓                ↓
PyObject* my_add(PyObject* args) {
    int a, b;  // C++ data types

    // Parse arguments from Python
    if (!PyArg_ParseTuple(args, "ii", &a, &b)) {
        return NULL;
    }

    // Convert C++ data type to python int
    return PyLong_FromLong(a + b);
}
```

We can create this function more simply with nanobind
**Example Nanobind**
```cpp
int my_add(nb::tuple args) {
  int a, b;
  return a + b;
}
```
> nanobind maps the returned C++ data type directly to the Python object.