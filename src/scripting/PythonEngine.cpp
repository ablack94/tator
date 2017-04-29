// Andrew Black
// April 29, 2017

#include <Python.h>

#include "tator/scripting/PythonEngine.hpp"
using tator::scripting::PythonEngine;

PythonEngine::PythonEngine() { }
PythonEngine::~PythonEngine() {
	Py_Finalize();
}

void PythonEngine::initialize() {
	Py_Initialize();
}

void registerModule(std::string name, py::module &m) {
	py::dict modules(PyImport_GetModuleDict(), true);
	modules[name] = m.ptr();
}