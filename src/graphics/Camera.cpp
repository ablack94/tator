// Andrew Black
// February 28, 2017

#include "tator/graphics/Camera.hpp"
	using tator::graphics::Camera;

// Constructor(s)
Camera::Camera(glm::vec3 position, glm::vec3 forward) {
	this->position = position;
	this->forward = forward;
	this->update();
}

Camera* Camera::fromLookAt(glm::vec3 position, glm::vec3 target) {
	return new Camera(position, glm::normalize(position - target));
}


glm::mat4 Camera::getViewMatrix() {
	return this->view_matrix;
}

void Camera::update() {
	this->forward = glm::normalize(this->forward);
	this->right = glm::normalize(glm::cross(this->forward, glm::vec3(0, 1, 0)));
	this->up = glm::normalize(glm::cross(this->right, this->forward));
	this->view_matrix = glm::lookAt(this->position, this->position + this->forward, this->up);
}
