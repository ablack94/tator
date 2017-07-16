// Andrew Black
// July 04, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_IMPL_OPENGL_DETAIL_OPENGLOBJECTMANAGER_HPP
#define HG_TATOR_GRAPHICS_IMPL_OPENGL_DETAIL_OPENGLOBJECTMANAGER_HPP

#include "./OpenGLObject.hpp"
using tator::graphics::impl::opengl::detail::OpenGLObject;

#include <GL/glew.h>

#include <vector>
#include <memory>

namespace tator {
namespace graphics {
namespace impl {
namespace opengl {
namespace detail {

class OpenGLObjectManager {
public:

protected:
	std::vector<std::unique_ptr<OpenGLObject>> objects;
};

} // detail
} // opengl
} // impl
} // graphics
} // tator

#endif