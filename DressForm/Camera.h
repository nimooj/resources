#pragma once

#include "./include/glm/glm.hpp"
#include "./include/glm/gtc/matrix_transform.hpp"
using  namespace glm;

#define FRONT 0
#define UP 1
#define RIGHT 2
#define LEFT 3

class Camera {
	public:
		Camera();
		~Camera();

		void ViewFront();
		void ViewRight();
		void ViewLeft();
		void ViewUp();

		void ZoomIn();
		void ZoomOut();

		int state;
		vec3 cameraPosition;
		vec3 cameraTarget;
		vec3 cameraDirection;
		vec3 cameraUp;
		vec3 cameraRight;
		vec3 cameraFront;

		mat4 view;
};