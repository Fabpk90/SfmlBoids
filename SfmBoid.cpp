// SfmBoid.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <execution>
#include <random>


#include "Boid.h"

constexpr int BOID_SIZE = 10;
constexpr float PERCEPTION_RADIUS = 100.0f;

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

glm::vec2 getRandom2DDirection()
{
	static std::random_device rd; // obtain a random number from hardware
	static std::mt19937 gen(rd()); // seed the generator

	static std::uniform_real_distribution<> x(0, 1);
	static std::uniform_real_distribution<> y(0, 1);
	
	return glm::vec2(x(gen), y(gen));
}

void alignBoid( std::vector<Direction>& directionsBoidNeigh, Velocity& vel)
{
	
	glm::vec2 avgDirection{};

	for(auto& dir : directionsBoidNeigh)
	{
		avgDirection += dir.member;
	}

	avgDirection /= directionsBoidNeigh.size();

	//steering
	vel.member = avgDirection - vel.member;
}


int main()
{
	entt::registry registry;
	
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	window.setFramerateLimit(60);

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	
	std::uniform_int_distribution<> widthRandom(0, 800 - 16);
	std::uniform_int_distribution<> heightRandom(0, 600 - 16);
	
	for (int i = 0; i < BOID_SIZE; ++i)
	{
		const auto entity = registry.create();
		registry.emplace<Position>(entity, glm::vec2(widthRandom(gen), heightRandom(gen)));
		registry.emplace<Velocity>(entity, glm::vec2(1.0f));
		registry.emplace<Orientation>(entity, 0.f);
		registry.emplace<Direction>(entity, getRandom2DDirection());
	}

	sf::Texture tex;
	tex.loadFromFile("boidImg.png");

	sf::Sprite sprite(tex);
	sprite.setScale(0.25f, 0.25f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed
				|| sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		window.clear();

		const auto boidSim = registry.view<Position, Velocity, Orientation, Direction>();
		std::for_each(std::execution::par, boidSim.begin(), boidSim.end(), [&](const auto ent)
		{
			Position& pos = boidSim.get<Position>(ent);
			Velocity& vel = boidSim.get<Velocity>(ent);
			
			std::vector<Direction> boidsNearDirection;

			for(auto entity : boidSim)
			{
				if(entity == ent) continue;
				
				Position& otherPos = boidSim.get<Position>(entity);
				
				
				if(glm::length(otherPos.member - pos.member) < PERCEPTION_RADIUS)
				{
					auto& otherDir = boidSim.get<Direction>(entity);
					boidsNearDirection.push_back(otherDir);
				}
			}

			alignBoid(boidsNearDirection, vel);
		});


		boidSim.each([&](const auto ent, const auto& pos, const auto& velocity, const auto& ori, const auto& dir)
		{
			sprite.setPosition(pos.member.x, pos.member.y);
			sprite.setRotation(ori.x);

			window.draw(sprite);
		});
		
		window.display();
	}
}
