// Andrew Black
// March 08, 2017

#include "tator/graphics/StandardMesh.hpp"
	using tator::graphics::CustomMesh;

CustomMesh::CustomMesh() { }
CustomMesh::~CustomMesh() { }

void CustomMesh::addVertex(CustomMesh::VertexData vd) {
	this->vertices.push_back(vd);
}

void CustomMesh::addFace(CustomMesh::Face face) {
	this->faces.push_back(face);
}

void CustomMesh::buffer() {

}

void CustomMesh::bind() {

}

void CustomMesh::draw() {

}

