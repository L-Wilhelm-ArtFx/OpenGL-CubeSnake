#pragma once
#include <memory>

#include "glm_includes.h"
#include "mesh.h"
#include "shader.h"

enum GAME_STATE
{
	IDLE = 0,
	PLAYING = 1,
	THE_SNAKE_FALLS = 2,
};

class GameObj
{
public:
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 tPos = glm::vec3(0.0f);
	float posLerping = 1.0f;

	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 scale2 = glm::vec3(1.0f);
	float scaleLerping = 0.0f;
	glm::vec3 rotation = glm::vec3(0.0f);
	GameObj();
	glm::mat4 calcTransform();
	glm::mat4 calcTransformT();
	glm::mat4 calcTransformCam();
	glm::mat3 calcTransformNorm();
};

class MeshInstance : public GameObj
{
public:
	glm::vec3 color = glm::vec3(1.0f);
	MeshInstance();
};

class GameScene
{
public:
	GameObj camera;
	GameObj light;
	std::vector<std::shared_ptr<MeshInstance>> cubes;
	int width, height;

	ShaderProgram cubeShader;
	Mesh cubeMesh;
	GLuint uniObjectTransform;
	GLuint uniPerspective;
	GLuint uniViewTransform;
	GLuint uniNormMatrix;
	GLuint uniColor;

	GAME_STATE gState = GAME_STATE::IDLE;

	unsigned long long frame = 0;
	unsigned int stepCooldownCurrent = 0;
	unsigned int stepCooldown = 15;
	unsigned int snakeDeathCooldown = 0;
	unsigned int steps = 0;

	glm::vec3 dirCached = glm::vec3(0.0f);

	std::vector<std::shared_ptr<MeshInstance>> bodySegments;

	void start();
	void stop();
	void update();
	void snakeStep(glm::vec3 dir);
	void snakeAppend();
};
