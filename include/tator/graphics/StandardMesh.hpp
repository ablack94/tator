// Andrew Black
// March 08, 2017

#ifndef __TATOR_GRAPHICS_STANDARDMESH_HPP
#define __TATOR_GRAPHICS_STANDARDMESH_HPP

#include "tator/graphics/Mesh.hpp"
	using tator::graphics::Mesh;

#include "tator/graphics/gl.hpp"

namespace tator {
namespace graphics {

class CustomMesh : public Mesh {
public:
	class VertexData {
	public:
		GLfloat position[3];
		GLfloat color[4];
		GLfloat tex_coord[2];
	};
	class Face {
	public:
		int vertices[3];
	};
public:
	CustomMesh();
	CustomMesh(std::vector<VertexData> vertices, std::vector<Face> faces);
	~CustomMesh();
	
	void addVertex(VertexData vd);
	void addFace(Face face);

	virtual void buffer() override;
	virtual void bind() override;
	virtual void draw() override;

protected:
	std::vector<VertexData> vertices;
	std::vector<Face> faces;
};

} // graphics
} // tator

#endif