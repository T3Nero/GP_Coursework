#pragma once
#include <vector>
#include <GL\glew.h>
#include "stb_image.h"
#include "Shader.h"
#include <iostream>
#include "Camera.h"
#include "Mesh.h"
#include "transform.h"
#include "Texture.h"


class Skybox
{
public:
	void init(std::vector<std::string> faces);
	void draw(Camera* camera);

	unsigned int textureID;
	unsigned int skyboxVAO;
	unsigned int skyboxVBO;

	Shader shader;
	Transform transform;

	Texture noiseTexture;
	Texture skyTexture;
	Texture lavaTexture;

	Mesh ball;
private:
	float counter;

};

