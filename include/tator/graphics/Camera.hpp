// Andrew Black
// February 28, 2017

#ifndef __TATOR_GRAPHICS_CAMERA_HPP
#define __TATOR_GRAPHICS_CAMERA_HPP

#include "tator/graphics/gl.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace tator {
namespace graphics {

class Camera {
public:
	Camera(glm::vec3 position, glm::vec3 forward);
	glm::mat4 getViewMatrix();

	static Camera* fromLookAt(glm::vec3 position, glm::vec3 target);

protected:
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 world_up;
	glm::mat4 view_matrix;

	void update();

};

} // graphics
} // tator

#endif