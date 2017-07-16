
#ifndef _TATOR_CORE_TYPEID_HPP
#define _TATOR_CORE_TYPEID_HPP

#include <atomic>

namespace tator {
namespace core {

typedef uint64_t TypeId;

namespace detail {

	class _TypeIdGlobals {
	public:
		static _TypeIdGlobals& getInstance() {
			static _TypeIdGlobals instance;
			return instance;
		}
		std::atomic<TypeId> next_id;
	private:
		_TypeIdGlobals() = default;
		_TypeIdGlobals(_TypeIdGlobals const&) = delete;
		void operator=(_TypeIdGlobals const&) = delete;
	};
}

template <typename T>
class type_id {
public:
	type_id() = default;
	TypeId getTypeId() {
		static TypeId id = detail::_TypeIdGlobals::getInstance().next_id++;
		return id;
	}

	// Cheat to get a static method that returns the same value
	static TypeId TypeId() {
		type_id<T> t;
		return t.getTypeId();
	}
};

} // core
} // tator

#endif
