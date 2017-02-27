// Andrew Black
// February 26, 2017

#ifndef __TATOREXCEPTION_HPP
#define __TATOREXCEPTION_HPP

#include <exception>

#include <string>
	using std::string;

namespace tator {
namespace system {

class TatorException : std::exception {
public:
	TatorException() : std::exception() {}
	TatorException(string str) : std::exception(str.c_str()) {}
	TatorException(const char *msg) : std::exception(msg) {}
	TatorException(const char *msg, int code) : std::exception(msg, code) {}
	TatorException(std::exception &other) : std::exception(other) {}
};

} // system
} // tator

#endif