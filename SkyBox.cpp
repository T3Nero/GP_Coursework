#include "Skybox.h"

void Skybox::init(std::vector<std::string> faces)
{
	float skyboxVertices[] = {
		// positions          
		-100.0f,  100.0f, -100.0f,
		-100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		-100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f
	};

	// skybox VAO

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);

	shader.init("..\\res\\shaderSkybox.vert", "..\\res\\shaderSkybox.frag");
	shader.setInt("skybox", 0);
	noiseTexture.load("..\\res\\noise.png");
	skyTexture.load("..\\res\\sky.jpg");
	lavaTexture.load("..\\res\\lava.jpg");


	counter = 1.0f;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, noiseTexture.getID());

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Skybox texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::draw(Camera* camera) {

	counter = counter - 0.001f;

	glm::mat4 view = glm::mat4(glm::mat3(camera->getView())); // remove translation from the view matrix
	glDepthFunc(GL_LEQUAL);

	shader.Bind();

	shader.setMat4("view", view); // removes translation from view matrix

	shader.setMat4("projection", camera->getProjection()); // projection matrix

	shader.setMat4("model", transform.GetModel()); // model matrix

	shader.setFloat("time", counter / 1000); // how quickly the texture updates

	shader.setFloat("fogDensity", 0.3f);
	shader.setVec3("fogColor", 0.0f, 0.0f, 0.5f);
	shader.setFloat("maxDist", 10.0f);
	shader.setFloat("minDist", 0.0f);

	GLuint t1L = glGetUniformLocation(shader.getID(), "noiseTexture"); //texture 1 location
	GLuint t2L = glGetUniformLocation(shader.getID(), "skyTexture");
	GLuint t3L = glGetUniformLocation(shader.getID(), "lavaTexture");

	//set textures
	glActiveTexture(GL_TEXTURE1); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, noiseTexture.getID());
	glUniform1i(t1L, 1);

	glActiveTexture(GL_TEXTURE2); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, skyTexture.getID());
	glUniform1i(t2L, 2);

	glActiveTexture(GL_TEXTURE3); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, lavaTexture.getID());
	glUniform1i(t3L, 3);

	glBindVertexArray(skyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}