#pragma once

#include "Headers.h"

class Grid;

/// PATHFINDER 
///
/// Our pathfinder class is where we make use of our grid class 
/// to find the optimal path to a desired node in the grid whilst avoiding 
/// nodes which take longer to move across or nodes that you can walk over altogether. 
/// We also have admissable distance evaluation functions so we can get the appropriate 
/// cost of movement to each node. The findpath function is where our A* algorithm is 
/// done. The function returns a list of nodes which our grid can then make use of to move
/// our units along it. 
class PathFinder
{
public:
	PathFinder(Grid* _grid);
	std::vector<Node*>* FindPath(sf::Vector2f* _startpos, sf::Vector2f* _targetpos);
	bool IsNodeInList(std::vector<Node*>* _list, Node* _current);
	float GetManhattanDistance(Node* _nodeA, Node* _NodeB);
	float GetEuclideanDistance(Node* _nodeA, Node* _NodeB);
	std::vector<Node*>* RetracePath(Node* _startnode, Node* _endnode);

private: 
	Grid* m_grid;
};

