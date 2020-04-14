#include "AStarFinding.h"

AStarFinding::AStarFinding()
{
	this->m_isStartGoalInitialized = false;
	this->m_isGoalFounded = false;
	this->m_isGoalReached = false;
}

void AStarFinding::findPath(Vector2D currentNode, Vector2D goalNode)
{
	if (!this->m_isStartGoalInitialized)
	{
		for (AStarSearchCell* openCell : this->m_openList)
		{
			delete openCell;
		}
		this->clearOpenList();

		for (AStarSearchCell* visitedCell : this->m_visitedList)
		{
			delete visitedCell;
		}
		this->clearVisitedList();

		for (Vector2D* step : this->m_shortestPath)
		{
			delete step;
		}
		this->clearPathToGoal();

		AStarSearchCell start;
		start.m_xCoord = currentNode.x;
		start.m_yCoord = currentNode.y;

		AStarSearchCell goal;
		goal.m_xCoord = goalNode.x;
		goal.m_yCoord = goalNode.y;

		this->setStartAndGoal(start, goal);

		this->m_isStartGoalInitialized = true;
	}

	if (this->m_isStartGoalInitialized)
	{
		this->continuePath();
	}
}

Vector2D AStarFinding::nextPathPosition(GameObject* gameObject)
{
	int index = 1;
	Vector2D nextCell;
	nextCell.x = this->m_shortestPath[this->m_shortestPath.size() - index]->x;
	nextCell.y = this->m_shortestPath[this->m_shortestPath.size() - index]->y;

	if (!this->m_isGoalReached && gameObject->getMapCellCoords() == nextCell)
	{
		if (this->m_shortestPath.size() > 1)
		{
			this->m_shortestPath.pop_back();
		}
		else
		{
			this->m_isGoalReached = true;
		}
	}

	return nextCell;
}

void AStarFinding::clearOpenList()
{
	this->m_openList.clear();
}

void AStarFinding::clearVisitedList()
{
	this->m_visitedList.clear();
}

void AStarFinding::clearPathToGoal()
{
	this->m_shortestPath.clear();
}

bool AStarFinding::isGoalFounded()
{
	return this->m_isGoalFounded;
}

bool AStarFinding::isGoalReached()
{
	return this->m_isGoalReached;
}

void AStarFinding::setStartAndGoal(AStarSearchCell start, AStarSearchCell goal)
{
	this->m_startCell = new AStarSearchCell(start.m_xCoord, start.m_yCoord, nullptr);
	this->m_startCell->m_g = 0;
	this->m_startCell->m_h = this->m_startCell->getManhattanDistance(&goal);

	this->m_goalCell = new AStarSearchCell(goal.m_xCoord, goal.m_yCoord, &goal);

	this->m_openList.push_back(this->m_startCell);
}

void AStarFinding::isInOpenedList(int x, int y, float newCost, AStarSearchCell* parent)
{
	// TODO Check collisions
	// If the tile is not transitable, return
	/*if (CELL_BLOCKED)
	{
		return;
	}*/

	int id = y * (25 * 16 * 5) + x;

	// If it is already visited, return
	for (AStarSearchCell* visitedList : this->m_visitedList)
	{
		if (id == visitedList->m_id)
		{
			return;
		}
	}

	AStarSearchCell* newChild = new AStarSearchCell(x, y, parent);
	newChild->m_g = newCost;
	newChild->m_h = parent->getManhattanDistance(this->m_goalCell);


	for (AStarSearchCell* openCell : this->m_openList)
	{
		if (id == openCell->m_id)
		{
			float newF = newChild->m_g + newCost + openCell->m_h;

			if (newF < openCell->getF())
			{
				// Replace with the better one
				openCell->m_g = newChild->m_g + newCost;
				openCell->parent = newChild;
			}
			else {
				// If the F is not better
				delete newChild;
				return;
			}
		}
	}

	this->m_openList.push_back(newChild);
}

AStarSearchCell* AStarFinding::getNextCell()
{
	float bestF = 999999.0f;
	int bestCellIndex = -1;

	AStarSearchCell* nextCell = nullptr;

	for (unsigned int i = 0; i < this->m_openList.size(); i++)
	{
		AStarSearchCell* openCell = this->m_openList[i];

		if (openCell->getF() < bestF)
		{
			bestF = openCell->getF();
			bestCellIndex = i;
		}
	}

	if (bestCellIndex >= 0)
	{
		nextCell = this->m_openList[bestCellIndex];
		this->m_visitedList.push_back(nextCell);
		this->m_openList.erase(this->m_openList.begin() + bestCellIndex);
	}

	return nextCell;
}

void AStarFinding::continuePath()
{
	const int POSIBLE_MAP_MOVES = 4;

	for (int i = 0; i < POSIBLE_MAP_MOVES; i++)
	{
		if (this->m_openList.empty())
		{
			return;
		}

		AStarSearchCell* currentCell = this->getNextCell();

		if (currentCell->m_id == m_goalCell->m_id)
		{
			m_goalCell->parent = currentCell->parent;

			AStarSearchCell* getPath;

			for (getPath = m_goalCell; getPath != nullptr; getPath = getPath->parent)
			{
				this->m_shortestPath.push_back(new Vector2D(getPath->m_xCoord, getPath->m_yCoord));
			}

			this->m_isGoalFounded = true;

			return;
		}
		else
		{
			// Right side
			this->isInOpenedList(currentCell->m_xCoord + 1, currentCell->m_yCoord, currentCell->m_g + 1, currentCell);
			// Left side
			this->isInOpenedList(currentCell->m_xCoord - 1, currentCell->m_yCoord, currentCell->m_g + 1, currentCell);
			// Top side
			this->isInOpenedList(currentCell->m_xCoord, currentCell->m_yCoord + 1, currentCell->m_g + 1, currentCell);
			// Bottom side
			this->isInOpenedList(currentCell->m_xCoord, currentCell->m_yCoord - 1, currentCell->m_g + 1, currentCell);
			// To calculate diagonals, add/subtract appropriate x/y
			//this->isInOpenedList(currentCell->m_xCoord + 1, currentCell->m_yCoord - 1, currentCell->m_g + 1, currentCell);

			for (unsigned int i = 0; i < this->m_openList.size(); i++)
			{
				if (currentCell->m_id == this->m_openList[i]->m_id)
				{
					this->m_openList.erase(this->m_openList.begin() + i);
				}
			}
		}
	}
}
