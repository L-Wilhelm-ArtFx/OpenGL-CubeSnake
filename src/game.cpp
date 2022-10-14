#pragma once
#include <iostream>

#include "game.h"
#include "mesh.h"
#include "input.h"
#include "glm_includes.h"

GameObj::GameObj() {}

MeshInstance::MeshInstance() : GameObj()
{
}

glm::mat4 GameObj::calcTransform()
{
	auto transform = glm::identity<glm::mat4>();
	transform = glm::translate(transform, this->pos);
	transform = glm::scale(transform, scale * scale2);
	transform = transform * glm::yawPitchRoll(rotation[1], rotation[0], rotation[2]);
	return transform;
}

glm::mat4 GameObj::calcTransformCam()
{
	auto transform = glm::identity<glm::mat4>();
	transform = transform * glm::yawPitchRoll(rotation[1], rotation[0], rotation[2]);
	transform = glm::scale(transform, scale * scale2);
	transform = glm::translate(transform, this->pos);
	return transform;
}

glm::mat4 GameObj::calcTransformT()
{
	auto transform = glm::identity<glm::mat4>();
	transform = glm::translate(transform, this->tPos);
	transform = glm::scale(transform, scale * scale2);
	transform = transform * glm::yawPitchRoll(rotation[1], rotation[0], rotation[2]);
	return transform;
}

glm::mat3 GameObj::calcTransformNorm()
{
	auto transform = glm::identity<glm::mat3>();
	transform = transform * glm::mat3(glm::yawPitchRoll(rotation[1], rotation[0], rotation[2]));
	return transform;
}

void GameScene::snakeStep(glm::vec3 dir)
{
	if (bodySegments.size() == 0)
		return;

	glm::vec3 newPos = bodySegments[0]->pos + dir;

	auto firstSeg = bodySegments[0];
	firstSeg->scale = glm::vec3(1.1f);
	firstSeg->scaleLerping = 0.4f;

	for (auto &seg : bodySegments)
	{
		auto otherPos = seg->pos;
		auto diff = otherPos - newPos;
		if (abs(diff.x) < 0.5f && abs(diff.y) < 0.5f)
		{
			gState = GAME_STATE::THE_SNAKE_FALLS;
			steps = 0;
		}
	}

	if (gState != GAME_STATE::THE_SNAKE_FALLS)
	{

		for (unsigned int i = 0; i < bodySegments.size(); i++)
		{
			auto seg = bodySegments[i];
			glm::vec3 oldPos = seg->pos;
			bodySegments[i]->pos = newPos;
			newPos = oldPos;
		}

		steps++;
	}

	auto seg = bodySegments[0];
	if (seg->pos.x > 12.0f || seg->pos.x < -12.0f || seg->pos.y > 8.0f || seg->pos.y < -8.0f)
	{
		gState = GAME_STATE::THE_SNAKE_FALLS;
		steps = 0;
	}
}

void GameScene::snakeAppend()
{
	glm::vec3 startPos{0.0f};
	if (!bodySegments.empty())
	{
		startPos = bodySegments.back()->pos;
	}
	std::shared_ptr bodySeg = std::make_shared<MeshInstance>();
	cubes.emplace_back(bodySeg);
	bodySegments.emplace_back(bodySeg);
	bodySeg->pos = startPos;
	bodySeg->tPos = startPos;
	bodySeg->scale2 = glm::vec3(0.7f + (1.0f / (1.0f + bodySegments.size() / 3.0f)) * 0.3f);
	bodySeg->scale = bodySeg->scale2 * 0.0f;
	bodySeg->scaleLerping = 0.2f;
	bodySeg->posLerping = 0.1f + 0.4f * (1.0f / (1.0f + bodySegments.size()));

	if (bodySegments.size() == 1)
	{
		bodySeg->color = glm::vec3(1.5f, 1.5f, 1.5f);
	}
	else
	{
		auto color = glm::lerp(
			glm::vec3(0.1f, 1.2f, 1.5f),
			glm::vec3(1.4f, 0.0f, 0.8f),
			glm::vec3(glm::min(1.0f, (bodySegments.size() - 1.0f) / 30.0f)));

		color = glm::lerp(
			color,
			glm::vec3(3.0f, 0.0f, 0.0f),
			glm::vec3(
				glm::min(
					glm::max(
						(bodySegments.size() - 61.0f) / 30.0f,
						0.0f),
					1.0f)));

		bodySeg->color = color;
	}
}

void GameScene::start()
{

	// Quite a silly approach to generate a cube mesh 👀

	printf("Generating Cube\n");

	std::vector<float> vertices;
	// vertices.reserve(6 * 4);

	std::vector<unsigned int> indices;
	// indices.reserve(6 * 6);

	{
		unsigned int vertIx = 0;

		auto genCubeQuad = [](unsigned int &vertIx,
							  std::vector<float> &vertices, std::vector<unsigned int> &indices,
							  glm::vec3 rotation)
		{
			glm::vec3 v[]{
				glm::vec3(-.5f, -.5f, 0.5f),
				glm::vec3(0.5f, -.5f, 0.5f),
				glm::vec3(0.5f, 0.5f, 0.5f),
				glm::vec3(-.5f, 0.5f, 0.5f)};

			glm::vec3 normal{0.0f, 0.0f, 1.0f};

			auto rotationMatrix = glm::mat3(glm::yawPitchRoll(rotation[1], rotation[0], rotation[2]));

			normal = rotationMatrix * normal;

			for (glm::vec3 &f : v)
			{
				f = rotationMatrix * f;
				vertices.emplace_back(f[0]);
				vertices.emplace_back(f[1]);
				vertices.emplace_back(f[2]);

				vertices.emplace_back(normal[0]);
				vertices.emplace_back(normal[1]);
				vertices.emplace_back(normal[2]);
			}

			indices.emplace_back(vertIx + 0);
			indices.emplace_back(vertIx + 1);
			indices.emplace_back(vertIx + 2);

			indices.emplace_back(vertIx + 2);
			indices.emplace_back(vertIx + 3);
			indices.emplace_back(vertIx + 0);
			vertIx += 4;
		};

		genCubeQuad(vertIx,
					vertices, indices,
					glm::vec3(0.0f, 0.0f, 0.0f));
		genCubeQuad(vertIx,
					vertices, indices,
					glm::vec3(glm::pi<float>(), 0.0f, 0.0f));
		genCubeQuad(vertIx,
					vertices, indices,
					glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
		genCubeQuad(vertIx,
					vertices, indices,
					glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f));
		genCubeQuad(vertIx,
					vertices, indices,
					glm::vec3(0.0f, glm::half_pi<float>(), 0.0f));
		genCubeQuad(vertIx,
					vertices, indices,
					glm::vec3(0.0f, -glm::half_pi<double>(), 0.0f));
	}

	printf("Loading Cube Mesh\n");

	cubeMesh.load(vertices, indices);

	const char *vertexShaderSource = "#version 440 core\n"
									 "layout (location = 0) in vec3 aPos;\n"
									 "layout (location = 1) in vec3 aNorm;\n"
									 "out vec3 fNorm;\n"
									 "uniform mat4 objectTransform;\n"
									 "uniform mat4 perspective;\n"
									 "uniform mat4 viewTransform;\n"
									 "uniform mat3 normMatrix;\n"
									 "void main()\n"
									 "{\n"
									 "   vec3 pos = aPos;"
									 "   vec4 screenPos = perspective * viewTransform * objectTransform * vec4(pos.x, pos.y, pos.z, 1.0);\n"
									 "	 gl_Position = screenPos;\n"
									 "	 fNorm = normMatrix * aNorm;\n"
									 "}\0";

	const char *fragmentShaderSource = "#version 440 core\n"
									   "in vec3 fNorm;\n"
									   "out vec4 FragColor;\n"
									   "uniform vec3 uColor;\n"
									   "void main()\n"
									   "{\n"
									   "	vec3 color = uColor;\n"
									   "	color *= max(0.0f, dot(fNorm, normalize(vec3(0.1f, 0.2f, 0.8f))));\n"
									   "	FragColor = vec4(color, 1.0f);\n"
									   "}\0";
	printf("Loading Shader\n");

	cubeShader.load(vertexShaderSource, fragmentShaderSource);

	uniObjectTransform = glGetUniformLocation(cubeShader.getProgramId(), "objectTransform");
	uniPerspective = glGetUniformLocation(cubeShader.getProgramId(), "perspective");
	uniViewTransform = glGetUniformLocation(cubeShader.getProgramId(), "viewTransform");
	uniNormMatrix = glGetUniformLocation(cubeShader.getProgramId(), "normMatrix");
	uniColor = glGetUniformLocation(cubeShader.getProgramId(), "uColor");

	for (int x = -12; x <= 12; x++)
	{
		for (int y = -8; y <= 8; y++)
		{
			auto inst = std::make_shared<MeshInstance>();
			inst->pos = glm::vec3(x, y, 0.0f);
			inst->scale = glm::vec3(0.0f);
			inst->scale2 = glm::vec3(0.34f);
			inst->scaleLerping = 0.05f;
			inst->color = glm::vec3(0.2f, 0.6f, 1.0f);
			cubes.emplace_back(inst);
		}
	}

	camera.pos[2] = -11.0f;

	snakeAppend();
}

void GameScene::stop()
{
	cubeMesh.destroy();
	cubeShader.destroy();
}

void GameScene::update()
{
	double totalSeconds = frame / 60.0;

	// Game Update

	if (gState == GAME_STATE::IDLE || gState == GAME_STATE::PLAYING)
	{

		if (stepCooldownCurrent > 0)
		{
			stepCooldownCurrent--;
		}

		glm::vec3 dir{0.0f};
		if (GInput::isPressed(GLFW_KEY_W) || GInput::isPressed(GLFW_KEY_UP))
		{
			dir.y = 1.0f;
		}
		else if (GInput::isPressed(GLFW_KEY_S) || GInput::isPressed(GLFW_KEY_DOWN))
		{
			dir.y = -1.0f;
		}
		else if (GInput::isPressed(GLFW_KEY_A) || GInput::isPressed(GLFW_KEY_LEFT))
		{
			dir.x = -1.0f;
		}
		else if (GInput::isPressed(GLFW_KEY_D) || GInput::isPressed(GLFW_KEY_RIGHT))
		{
			dir.x = 1.0f;
		}
		bool hasDirection = abs(dir.x) + abs(dir.y) > 0.0f;
		if (hasDirection && -dir != dirCached)
		{
			dirCached = dir;
			gState = GAME_STATE::PLAYING;
		}

		if ((hasDirection || GAME_STATE::PLAYING) && stepCooldownCurrent == 0)
		{
			if (gState == GAME_STATE::PLAYING)
			{
				if (steps % 2 == 0)
				{
					snakeAppend();
				}
			}
			snakeStep(dirCached);
			if (gState == GAME_STATE::IDLE)
			{
				gState = GAME_STATE::PLAYING;
			}
			stepCooldown = glm::max(10.0f, round(25.0f - bodySegments.size() * 0.66f));
			stepCooldownCurrent = stepCooldown;
		}
	}
	else if (gState == GAME_STATE::THE_SNAKE_FALLS)
	{
		if (bodySegments.size() > 1)
		{
			if (snakeDeathCooldown > 0)
			{
				snakeDeathCooldown--;
			}
			else
			{
				auto back = bodySegments.back();
				auto found = std::find_if(
					cubes.begin(),
					cubes.end(),
					[&](std::shared_ptr<MeshInstance> const &p)
					{ return &*p == &*back; });
				if (found != cubes.end())
				{
					cubes.erase(found);
				}
				bodySegments.pop_back();
				for(unsigned int i = 1;i < bodySegments.size();i++){
					auto seg = bodySegments[i];

					seg->scale2 = seg->scale = glm::vec3(0.75f + 0.16f * cos( 0.6f * (bodySegments.size() - i - 1)));
					seg->scaleLerping = 0.5f;
				}
				snakeDeathCooldown = 2;
			}
		}
		else
		{
			snakeDeathCooldown = 0;
			gState = GAME_STATE::IDLE;
			dirCached = glm::vec3(0.0f);
			auto seg = bodySegments[0];
			if (seg->pos.x > 12.0f || seg->pos.x < -12.0f || seg->pos.y > 8.0f || seg->pos.y < -8.0f)
			{
				seg->pos = glm::vec3(0.0f);
			}
		}
	}

	auto camTarget = bodySegments[0];
	if (gState == GAME_STATE::THE_SNAKE_FALLS)
	{
		camTarget = bodySegments.back();
	}

	float yRot = -(atan2(camTarget->pos.z - camera.pos.z, camTarget->pos.x - camera.pos.x) - glm::half_pi<float>());
	yRot *= 0.5f;
	camera.rotation.y = camera.rotation.y + (yRot - camera.rotation.y) * 0.06f;

	float xRot = -(atan2(camTarget->pos.z - camera.pos.z, camera.pos.y - camTarget->pos.y) - glm::half_pi<float>());
	xRot *= 0.5f;
	camera.rotation.x = camera.rotation.x + (xRot - camera.rotation.x) * 0.06f;

	// Render

	glm::vec3 clearCol = glm::lerp(
		glm::vec3(0.03f, 0.05f, 0.12f),
		glm::vec3(0.06f, 0.13f, 0.2f),
		glm::vec3(((float)sin(totalSeconds) + 1.0f) * 0.5f));
	glClearColor(clearCol[0], clearCol[1], clearCol[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto persp = glm::perspectiveFov(glm::half_pi<float>(), (float)width, (float)height, 0.005f, 100.0f);

	cubeShader.use();

	auto viewTransform = camera.calcTransformCam();
	glUniformMatrix4fv(uniViewTransform, 1, false, glm::value_ptr(viewTransform));
	glUniformMatrix4fv(uniPerspective, 1, false, glm::value_ptr(persp));

	for (auto c : cubes)
	{
		c->rotation.x = totalSeconds + c->pos.x * 0.2f;
		c->rotation.y = totalSeconds * 1.3f + c->pos.y * 0.2f;

		c->scale = c->scale + (c->scale2 - c->scale) * c->scaleLerping;
		c->tPos = c->tPos + (c->pos - c->tPos) * c->posLerping;

		auto objectTransform = c->calcTransformT();
		auto normTransform = c->calcTransformNorm();

		glUniformMatrix4fv(uniObjectTransform, 1, false, glm::value_ptr(objectTransform));
		glUniformMatrix3fv(uniNormMatrix, 1, false, glm::value_ptr(normTransform));
		glUniform3fv(uniColor, 1, glm::value_ptr(c->color));
		cubeMesh.draw();
	}

	frame++;
}