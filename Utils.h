#pragma once
#include <glm/vec2.hpp>
constexpr int BOID_COUNT = 10000;
constexpr int BOID_SIZE = 32;
constexpr float PERCEPTION_RADIUS = 50.0f;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

struct Position
{
	glm::vec2 member;
};

struct Velocity
{
	glm::vec2 member;
};

struct Orientation
{
	float x;
};

struct Direction
{
	glm::vec2 member;
};