#pragma once
#include <entt/entity/fwd.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>


#include "Utils.h"



class QuadTree
{
public:
	struct Node
	{
		std::vector<entt::entity> entities;
		std::vector<Node> children;

		float size;
		glm::vec2 center;

		void draw(sf::RenderWindow* window, sf::RectangleShape* rect)
		{
			if (children.empty())
			{
				rect->setSize(sf::Vector2f(size, size));
				rect->setPosition(center.x, center.y);

				window->draw(*rect);
			}
			else
			{
				for (auto& child : children)
				{
					child.draw(window, rect);
				}
			}
		}
	};
	
private:
	Node root;
	
public:
	QuadTree(float startingSize, glm::vec2 startingPoint);

	void draw(sf::RenderWindow* window, sf::RectangleShape* rect);

	void add(entt::entity* ent, Position& pos);
};

