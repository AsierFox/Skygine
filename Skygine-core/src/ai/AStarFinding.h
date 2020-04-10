#pragma once

#include <vector>

#include"../physics/Vector2D.h"
#include "AStarSearchCell.h"

class AStarFinding
{
	bool m_isStartGoalInitialized;
	bool m_isGoalFounded;

	AStarSearchCell* m_startCell;
	AStarSearchCell* m_goalCell;

	std::vector<AStarSearchCell*> m_openList;
	std::vector<AStarSearchCell*> m_visitedList;
	std::vector<Vector2D*> m_shortestPath;

public:

	AStarFinding();

	void findPath(Vector2D currentNode, Vector2D goalNode);
	Vector2D nextPathPosition();
	void clearOpenList();
	void clearVisitedList();
	void clearPathToGoal();

private:

	void setStartAndGoal(AStarSearchCell start, AStarSearchCell goal);
	// Check if the current cell is in the open list
	void isInOpenedList(int x, int y, float newCost, AStarSearchCell* parent);
	AStarSearchCell* getNextCell();
	// Searches through the world, finding the target position using G and H
	void continuePath();
};
