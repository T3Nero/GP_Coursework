#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <string>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "SkyBox.h"
#include "Camera.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

	float counter;

private:

	void initSystems();
	void processInput();
	inline void gameLoop();

	void drawGame();
	void drawStripeMesh();
	void drawGeoMesh();
	void drawEMapMesh();
	void drawSkybox();
	void drawNoise();

	
	void linkStripeShader();
	void linkGeoShader();
	void linkEMapping();
	void linkNoise();

	void enableAlphaChannel(); // enables alpha channel (opaque value) - useful for vanishing objects over time 

	Display _gameDisplay;
	GameState _gameState;

	Mesh ball;
	Mesh geoBall;

	Camera myCamera;

	Texture bricksTexture;
	Texture waterTexture;
	Texture normalMapping;
	Texture noiseTexture;
	Texture skyTexture;
	Texture lavaTexture;


	Shader fogShader;
	Shader toonShader;
	Shader mixShader;
	Shader stripeShader;
	Shader bumpShader;
	Shader geoShader;
	Shader eMappingShader;

	Skybox skybox;

	Transform transform;
};

