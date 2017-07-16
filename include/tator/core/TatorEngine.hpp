// Andrew Black 
// May 29, 2017

#ifndef TATOR_CORE_TATORENGINE_HPP
#define TATOR_CORE_TATORENGINE_HPP

#include <GLFW\glfw3.h>
#include "tator/system/TatorException.hpp"
using tator::system::TatorException;

class TatorEngine {
public:
	TatorEngine() {
		if (!glfwInit()) {
			throw TatorException("Unable to initialize GLFW!");
		}
	}
	TatorEngine(TatorEngine const&) = delete;
	void operator=(TatorEngine const&) = delete;
};

#endif


