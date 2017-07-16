// Andrew Black
// July 09, 2017

#pragma once
#ifndef HG_TATOR_GRAPHICS_DATA_ABSTRACTBUFFEREDVERTEXDATADISPATCHER_HPP
#define HG_TATOR_GRAPHICS_DATA_ABSTRACTBUFFEREDVERTEXDATADISPATCHER_HPP

#include "BufferedVertexData.hpp"
using tator::graphics::data::BufferedVertexData;

namespace tator {
namespace graphics {
namespace data {

class AbstractBufferedVertexDataDispatcher {
public:
	virtual void dispatch(BufferedVertexData &bvd) = 0;
};

} // data
} // graphics
} // tator


#endif