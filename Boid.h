#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Boid
{
private:
	sf::Sprite sprite;
	sf::Texture tex;

public:
	Boid(const char* path, uint32_t width, uint32_t height);
};

