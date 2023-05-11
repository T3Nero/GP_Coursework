#include "MainGame.h"


MainGame::MainGame()
{
	_gameState = GameState::PLAY;
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}


void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 

	ball.loadModel("..\\res\\Ball.obj");
	geoBall.loadModel("..\\res\\Ball.obj");

	bricksTexture.load("..\\res\\bricks.jpg"); 
	waterTexture.load("..\\res\\water.jpg");
	normalMapping.loadNormals("..\\res\\normal.jpg");
	noiseTexture.load("..\\res\\noise.png");
	skyTexture.load("..\\res\\sky.jpg");
	lavaTexture.load("..\\res\\lava.jpg");

	stripeShader.init("..\\res\\stripeShader.vert", "..\\res\\stripeShader.frag");
	geoShader.initGeoShader("..\\res\\shaderGeoText.vert", "..\\res\\shaderGeoText.frag", "..\\res\\shaderGeoText.geom");
	eMappingShader.init("..\\res\\eMapping.vert", "..\\res\\eMapping.frag");

	vector<string> skyboxFaces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};
	skybox.init(skyboxFaces);

	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}

void MainGame::linkStripeShader()
{
	enableAlphaChannel();
	stripeShader.setMat4("normalMatrix", transform.GetModel());
	glm::mat4 modelView = myCamera.getView() * transform.GetModel();

	stripeShader.setMat4("modelViewMatrix", modelView);
	stripeShader.setMat4("projectionMatrix", myCamera.getProjection());
}


void MainGame::linkGeoShader()
{
	// stop explosion after set time
	if(counter > -14)
	{
		geoShader.setFloat("time", counter);
	}

	GLuint diffuseID = glGetUniformLocation(geoShader.getID(), "diffuseT");

	// sets textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bricksTexture.getID());
	glUniform1i(diffuseID, 0);
}

void MainGame::linkEMapping()
{
	enableAlphaChannel();

	eMappingShader.Bind();
	//eMappingShader.setVec3("camPos", myCamera.getPos());
	eMappingShader.setInt("skybox", 1);
	eMappingShader.setMat4("model", transform.GetModel());
	eMappingShader.setFloat("time", counter / 1000);
	eMappingShader.setFloat("fogDensity", 0.3);
	eMappingShader.setVec3("fogColor", 0.0f, 0.0f, 0.5f);
	eMappingShader.setFloat("maxDist", 10.0f);
	eMappingShader.setFloat("minDist", 0.0f);

	GLuint t1L = glGetUniformLocation(eMappingShader.getID(), "noiseTexture"); //texture 1 location
	GLuint t2L = glGetUniformLocation(eMappingShader.getID(), "skyTexture");
	GLuint t3L = glGetUniformLocation(eMappingShader.getID(), "lavaTexture");


	//set textures
	glActiveTexture(GL_TEXTURE2); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, noiseTexture.getID());
	glUniform1i(t1L, 2);

	glActiveTexture(GL_TEXTURE3); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, skyTexture.getID());
	glUniform1i(t2L, 3);

	glActiveTexture(GL_TEXTURE4); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, lavaTexture.getID());
	glUniform1i(t2L, 4);

	GLuint dID = glGetUniformLocation(eMappingShader.getID(), "diffuse"); // bind shader bricksTexture

	//set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTexture.getID());
	glUniform1i(dID, 0);
}

void MainGame::enableAlphaChannel()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.1f, 0.1f, 0.1f, 1.0f); //sets our background colour

	drawStripeMesh(); // stripe shader
	drawGeoMesh(); // geo shader (explosion)
	drawEMapMesh(); // environment mapping (reflections)

	myCamera.MoveRight(0.001);
	myCamera.setLook(geoBall.getSpherePos());

	counter = counter - 0.001f;

	drawSkybox();
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
}

void MainGame::drawStripeMesh()
{
	transform.SetPos(glm::vec3(-2.0, 1.5, 0.0));
	transform.SetRot(glm::vec3(0.0, counter, 0.0));
	transform.SetScale(glm::vec3(0.5, 0.5, 0.5));

	ball.updateSphereData(*transform.GetPos(), 0.62f);

	stripeShader.Bind();

	linkStripeShader();

	stripeShader.Update(transform, myCamera);

	ball.draw();
}

void MainGame::drawGeoMesh()
{
	transform.SetPos(glm::vec3(0.0, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, counter, 0.0));
	transform.SetScale(glm::vec3(0.5, 0.5, 0.5));

	geoShader.Bind();

	linkGeoShader();

	geoShader.Update(transform, myCamera);

	geoBall.draw();
}

void MainGame::drawEMapMesh()
{
	linkEMapping();

	transform.SetPos(glm::vec3(2.0, -1.0, 0.0));
	transform.SetRot(glm::vec3(0.0, counter, 0.0));
	transform.SetScale(glm::vec3(0.8, 0.8, 0.8));

	eMappingShader.Update(transform, myCamera);

	ball.draw();
	ball.updateSphereData(*transform.GetPos(), 0.62f);
}

void MainGame::drawSkybox()
{
	enableAlphaChannel();
	skybox.draw(&myCamera);
}
