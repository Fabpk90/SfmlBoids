#pragma once
#include <vector>
#include <entt/entt.hpp>
#include <glm/vec2.hpp>


class Grid
{
public:
	struct Node
	{
		std::vector<entt::entity> enttInCell;
	};

private:
	std::vector<Node> grid;
	float sizeCell;

public:
	Grid(float sizeCell);

	std::vector<entt::entity> getEntitiesNear(glm::vec2 position);
	void addEntityAt(glm::vec2 position, entt::entity entt);
};

