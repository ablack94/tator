// Andrew Black
// February 28, 2017

#ifndef __TATOR_GRAPHICS_CAMERA_HPP
#define __TATOR_GRAPHICS_CAMERA_HPP

//#include "tator/graphics/gl.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace tator {
namespace graphics {

class Camera {
public:
	Camera(glm::vec3 position, glm::vec3 forward) {
		this->position = position;
		this->forward = forward;
		this->update();
	}

	glm::mat4 getViewMatrix() {
		return this->view_matrix;
	}

	static Camera fromLookAt(glm::vec3 position, glm::vec3 target) {
		return Camera(position, (target - position));
	}

	glm::vec3 getPosition() { return this->position; }
	glm::vec3 getForward() { return this->forward; }
	glm::vec3 getUp() { return this->up; }
	glm::vec3 getRight() { return this->right; }

protected:
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 world_up;
	glm::mat4 view_matrix;

	void update() {
		this->forward = glm::normalize(this->forward);
		this->right = glm::normalize(glm::cross(this->forward, glm::vec3(0, 1, 0)));
		this->up = glm::normalize(glm::cross(this->right, this->forward));
		this->view_matrix = glm::lookAt(this->position, this->position + this->forward, this->up);
	}

};

} // graphics
} // tator

#endif