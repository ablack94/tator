// Andrew Black
// March 04, 2017

#include "tator/graphics/gl.hpp"
	using tator::graphics::GlObject;
	using tator::graphics::GlBoundObject;

#include "tator/system/TatorException.hpp"
	using tator::system::TatorException;

/*
GlObject
*/

GlObject::GlObject() {
	this->id = 0;
	this->has_id = false;
}

GlObject::~GlObject() {
	if(this->has_id)
		this->destroy();
}

bool GlObject::hasId() {
	return (this->has_id);
}

bool GlObject::hasId(GLuint* id) {
	if (this->has_id && id != NULL)
		*id = this->id;
	return this->has_id;
}

GLuint GlObject::getId() {
	if (this->has_id)
		return this->id;
	else
		throw TatorException("GlObject is not bound to anything!");
}

void GlObject::bind() {
	// Do nothing
}

void GlObject::unbind() {
	// Do nothing
}

void GlObject::destroy() {
	this->has_id = false;
}

void GlObject::setId(GLuint id) {
	if (this->has_id)
		throw TatorException("You must call destroy() before setting a new ID!");
	this->id = id;
	this->has_id = true;
}

/*
GlBoundObject
*/

GlBoundObject::GlBoundObject(std::initializer_list<GlObject*> objects) {
	for (auto obj : objects) {
		obj->bind();
		this->objects.push_back(obj);
	}
}

GlBoundObject::~GlBoundObject() {
	for (auto obj : this->objects) {
		obj->unbind();
	}
}


