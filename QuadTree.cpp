#include "QuadTree.h"

QuadTree::QuadTree(float startingSize, glm::vec2 startingPoint = glm::vec2(0))
{
	root.size = startingSize;
	root.center = startingPoint;
}

void QuadTree::draw(sf::RenderWindow* window, sf::RectangleShape* rect)
{
	root.draw(window, rect);
}

