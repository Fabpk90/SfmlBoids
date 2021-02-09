// SfmBoid.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <execution>
#include <random>


#include "Boid.h"
#include "Grid.h"
#include "Utils.h"

glm::vec2 getRandom2DDirection()
{
	static std::random_device rd; // obtain a random number from hardware
	static std::mt19937 gen(rd()); // seed the generator

	static std::uniform_real_distribution<> x(-1, 1);
	static std::uniform_real_distribution<> y(-1, 1);
	
	return glm::vec2(x(gen), y(gen));
}

void alignBoid( std::vector<Direction>& directionsBoidNeigh, Velocity& vel)
{
	if(directionsBoidNeigh.empty()) return;
	
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
	Grid grid(BOID_SIZE * 5);
	
	entt::registry registry;
	
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Boids");
	//window.setFramerateLimit(60);

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	
	std::uniform_int_distribution<> widthRandom(0, 800 - 16);
	std::uniform_int_distribution<> heightRandom(0, 600 - 16);
	
	for (int i = 0; i < BOID_COUNT; ++i)
	{
		const auto entity = registry.create();
		glm::vec2 randomPos(widthRandom(gen), heightRandom(gen));
		registry.emplace<Position>(entity, randomPos);
		registry.emplace<Velocity>(entity, glm::vec2(1.0f));
		registry.emplace<Orientation>(entity, 0.f);
		registry.emplace<Direction>(entity, getRandom2DDirection());

		grid.addEntityAt(randomPos, entity);
	}

	sf::Texture tex;
	tex.loadFromFile("boidImg.png");

	sf::Sprite sprite(tex);
	sprite.setScale(0.25f, 0.25f);

	sf::Clock clock;
	sf::Font font;
	font.loadFromFile("Roboto.ttf");
	
	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		window.clear();

		const auto boidSim = registry.view<Position, Velocity, Orientation, Direction>();
		std::for_each(std::execution::par, boidSim.begin(), boidSim.end(), [&](const auto ent)
		{
			Position& pos = boidSim.get<Position>(ent);
			Velocity& vel = boidSim.get<Velocity>(ent);
			
			std::vector<Direction> boidsNearDirection;

			/*for(auto entity : boidSim)
			{
				if(entity == ent) continue;
				
				Position& otherPos = boidSim.get<Position>(entity);
				
				if(glm::length(otherPos.member - pos.member) < PERCEPTION_RADIUS)
				{
					auto& otherDir = boidSim.get<Direction>(entity);
					boidsNearDirection.push_back(otherDir);
				}
			}*/

			auto neighbor = grid.getEntitiesNear(pos.member);

			for(auto& ent : neighbor)
			{
				auto& otherDir = boidSim.get<Direction>(ent);
				boidsNearDirection.push_back(otherDir);
			}

			alignBoid(boidsNearDirection, vel);

			pos.member += vel.member;

			if (pos.member.x < 0)
				pos.member.x = window.getSize().x - 16.0f;
			else if (pos.member.x > window.getSize().x)
				pos.member.x = 0;

			if (pos.member.y < 0)
				pos.member.y = window.getSize().y - 16.0f;
			else if (pos.member.y > window.getSize().y)
				pos.member.y = 0;
		});


		boidSim.each([&](const auto ent, const auto& pos, const auto& velocity, const auto& ori, const auto& dir)
		{
			sprite.setPosition(pos.member.x, pos.member.y);
			sprite.setRotation(ori.x);

			window.draw(sprite);
		});

		auto time = clock.restart();
		float framerate = 1 / time.asSeconds();
		

		text.setString(std::to_string(framerate));
		window.draw(text);
		
		window.display();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed
				|| sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}
	}
}
