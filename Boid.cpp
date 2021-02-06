#include "Boid.h"

Boid::Boid(const char* path, uint32_t width, uint32_t height)
{
	tex;
	tex.loadFromFile(path);

	sprite.setTexture(tex);
}
