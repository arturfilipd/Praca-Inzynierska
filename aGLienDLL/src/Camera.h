#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "Entity.h"

class __declspec(dllexport) Camera {
private:	
	float x, y, z;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	float cameraAngle;
	void calculateAngles();

public :
	Camera(float x, float y, float z);
	void SetPosition(float x, float y, float z);
	void setAngle(float a);
	glm::mat4 GetViewMatrix ();
	void Follow(float px, float py);
	void Follow(Entity*);
};
