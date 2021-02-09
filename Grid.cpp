#include "Grid.h"
#include "Utils.h"

Grid::Grid(float sizeCell) : sizeCell(sizeCell)
{
	grid.resize((WINDOW_WIDTH / sizeCell) * (WINDOW_HEIGHT / sizeCell));
}

std::vector<entt::entity> Grid::getEntitiesNear(glm::vec2 position)
{
	position /= sizeCell;
	int index = (int)position.y * (int)position.x;
	std::vector<entt::entity> nodes;

	nodes.insert(nodes.end(), grid[index].enttInCell.begin(), grid[index].enttInCell.end());
	
	return nodes;
}

void Grid::addEntityAt(glm::vec2 position, entt::entity entt)
{
	position /= sizeCell;
	int index = (int)position.y * (int)position.x;
	grid[index].enttInCell.push_back(entt);
}
