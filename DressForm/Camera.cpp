#include "stdafx.h"
#include "Camera.h"

Camera::Camera() {
	state = FRONT;

	cameraPosition = vec3(0, 0, 10);
	cameraTarget = vec3(0, 0, 0);
	cameraDirection = normalize(cameraPosition - cameraTarget);
	cameraFront = vec3(0, 0, -1);

	vec3 up = vec3(0, 1, 0);
	cameraRight = normalize(cross(up, cameraDirection));
	cameraUp = cross(cameraDirection, cameraRight);

	//view = lookAt(cameraPosition, -cameraDirection, cameraUp);
}

Camera::~Camera() {

}

void Camera::ViewFront() {
	state = FRONT;
	cameraPosition = vec3(0, 0, 10);
	cameraTarget = vec3(0, 0, 0);
	cameraDirection = normalize(cameraPosition - cameraTarget);
	cameraFront = vec3(0, 0, -1);

	vec3 up = vec3(0, 1, 0);
	cameraRight = normalize(cross(up, cameraDirection));
	cameraUp = cross(cameraDirection, cameraRight);

	//view = lookAt(cameraPosition, -cameraDirection, cameraUp);
}

void Camera::ViewRight() {
	state = RIGHT;
	cameraPosition = vec3(-10, 0, 0);
	cameraTarget = vec3(0, 0, 0);
	cameraDirection = normalize(cameraPosition - cameraTarget);
	cameraFront = vec3(1, 0, 0);

	vec3 up = vec3(0, 1, 0);
	cameraRight = normalize(cross(up, cameraDirection));
	cameraUp = cross(cameraDirection, cameraRight);

	//view = lookAt(cameraPosition, -cameraDirection, cameraUp);
}

void Camera::ViewLeft() {
	state = LEFT;
	cameraPosition = vec3(10, 0, 0);
	cameraTarget = vec3(0, 0, 0);
	cameraDirection = normalize(cameraPosition - cameraTarget);
	cameraFront = vec3(-1, 0, 0);

	vec3 up = vec3(0, 1, 0);
	cameraRight = normalize(cross(up, cameraDirection));
	cameraUp = cross(cameraDirection, cameraRight);

	//view = lookAt(cameraPosition, -cameraDirection, cameraUp);
}

void Camera::ViewUp() {
	state = UP;
	cameraPosition = vec3(0, 10, 0);
	cameraTarget = vec3(0, 0, 0);
	cameraDirection = normalize(cameraPosition - cameraTarget);
	cameraFront = vec3(0, -1, 0);

	vec3 up = vec3(0, 0, 1);
	cameraRight = normalize(cross(up, cameraDirection));
	cameraUp = cross(cameraDirection, cameraRight);

	//view = lookAt(cameraPosition, -cameraDirection, cameraUp);
}

void Camera::ZoomIn() {
	switch (state) {
	case FRONT:
		cameraPosition.z -= 0.3;
		break;

	case UP:
		cameraPosition.y -= -0.3;
		break;

	case RIGHT:
		cameraPosition.x += 0.3;
		break;

	case LEFT:
		cameraPosition.x -= 0.3;
		break;

	default:
		break;
	}
}

void Camera::ZoomOut() {
	switch (state) {
	case FRONT:
		cameraPosition.z += 0.3;
		break;

	case UP:
		cameraPosition.y += -0.3;
		break;

	case RIGHT:
		cameraPosition.x -= 0.3;
		break;

	case LEFT:
		cameraPosition.x += 0.3;
		break;

	default:
		break;
	}
}
