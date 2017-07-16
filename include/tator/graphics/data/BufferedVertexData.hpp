// Andrew Black
// July 09, 2017

#pragma once
#ifndef HG_TATOR_GRAPHICS_DATA_BUFFEREDVERTEXDATA_HPP
#define HG_TATOR_GRAPHICS_DATA_BUFFEREDVERTEXDATA_HPP

namespace tator {
namespace graphics {
namespace data {

class AbstractBufferedVertexDataDispatcher;

class BufferedVertexData {
public:
	virtual ~BufferedVertexData() = 0;
	virtual void accept(AbstractBufferedVertexDataDispatcher &dispatcher) = 0;
};

} // data
} // graphics
} // tator

#endif