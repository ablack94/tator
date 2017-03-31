// Andrew Black
// March 08, 2017

#ifndef __TATOR_SYSTEM_CORE
#define __TATOR_SYSTEM_CORE

namespace tator {
namespace system {

class TatorCore {
public:
	static void initialize();
private:
	TatorCore(); // can't be instantiated
};

}
}

#endif