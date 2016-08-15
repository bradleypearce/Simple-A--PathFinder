#include "PathFinder.h"

/// CONSTRUCTOR 
///
/// Our constructor sets the passed in reference to grid to our member grid reference.  
/// This allows us to access functions of grid from within our pathfinder class. 
PathFinder::PathFinder(Grid* _grid)
{
	this->m_grid = _grid;
}

/// FINDPATH 
///
/// This function is our pathfinding algorithm which mimics A*. To start we take in two position, our start 
/// and end positions we want to get to. We start by finding the corresponding node to that position by using 
/// the grid's findnodefrompoint function. We then define two lists of nodes, an open list and a closed list. 
/// We then push our start node in to our open list. For safety we check if the targetnode is not walkable, if so we
/// return null before doing anything. Otherwise, we set a flag pathfound to false and loop while our open list isn't 
/// empty. We define our current node as the first node in the open list and check if the fcost of every other node is 
/// is less than our current node's or if the f cost is the same and the hcost of the other nodes is less than our current node's. 
/// If either of these are true we set the current node to the openlist node at i. Then we iterate through the openlist untill we 
/// find which of these nodes is the current node. Once we find that node, we push the current node into the closed list and erase 
/// the currentnode from the openlist. Then we check if our currentnode is the endnode. If so we set pathfound to be true, we return the 
/// result of retracepath. Otherwise, we create a list of nodes called neighbours and set them to our current node's neighbours. Then for each 
/// neighbour we check if the node is walkable or if it's already in the closed list, if so we go to the next neighbour, or we set the g, h and f 
/// cost of each neighbour and set the parent of each neighbour to the current node. Then we check if the neighbour is not already in the open list 
/// and if not we push the neighbour back into the open list. Then continue with the loop. If no path is found or our openlist becomes empty we return 
/// null as no path could be found. 
std::vector<Node*>* PathFinder::FindPath(sf::Vector2f* _startpos, sf::Vector2f* _targetpos)
{
	Node* startnode = m_grid->FindNodeFromPoint(_startpos);
	Node* targetnode = m_grid->FindNodeFromPoint(_targetpos);

	std::vector<Node*> openlist;
	std::vector<Node*> closedlist;

	openlist.push_back(startnode);

	if(!targetnode->IsWalkable())
	{
		return NULL;
	}

	bool pathfound = false;

	while(openlist.size() > 0)
	{
		Node* currentnode = openlist.at(0);
		
		for(unsigned int i = 1; i < openlist.size(); i++)
		{
			if(openlist.at(i)->GetFCost() < currentnode->GetFCost() || openlist.at(i)->GetFCost() == currentnode->GetFCost() &&
				openlist.at(i)->GetHCost() < currentnode->GetHCost())
			{
				currentnode = openlist.at(i);
			}
		}

		for(unsigned int i = 0; i < openlist.size(); i++)
		{
			if(openlist.at(i) == currentnode)
			{
				closedlist.push_back(openlist.at(i));
				openlist.erase(openlist.begin() + i);
			}
		}

		if(currentnode == targetnode)
		{
			pathfound = true;

			return RetracePath(startnode, targetnode);
		}

		std::vector<Node*>* neighbours = currentnode->GetNeighbours(); 

		for(unsigned int i = 0; i < neighbours->size(); i++)
		{
			if(!neighbours->at(i)->IsWalkable() || IsNodeInList(&closedlist, neighbours->at(i)))
			{
				continue;
			}

			int CostToNeighbour = currentnode->GetGCost() + GetManhattanDistance(currentnode, neighbours->at(i));

			if(CostToNeighbour < neighbours->at(i)->GetGCost() || !IsNodeInList(&openlist, neighbours->at(i)))
			{
				neighbours->at(i)->SetGCost(CostToNeighbour);
				neighbours->at(i)->SetHCost((GetManhattanDistance(targetnode, neighbours->at(i))));
				neighbours->at(i)->SetFCost(neighbours->at(i)->GetGCost() + neighbours->at(i)->GetHCost());
				neighbours->at(i)->SetParent(currentnode);

				if(!IsNodeInList(&openlist, neighbours->at(i)))
				{
					openlist.push_back(neighbours->at(i));
				}
			}
		}
	}
	
	return NULL;
}

/// RETRACEPATH 
///
/// Here we create a new path reference to an new path and set our currentnode to our endnode we passed in. 
/// Then while the current node isn't the start node, we push the current node back into our path container and
/// set our currentnode to the current node's parent. Tracing our path back to the start node. Then we call the reverse function
/// so our path is in the correct order from start to finish and then return the path container.
std::vector<Node*>* PathFinder::RetracePath(Node* _startnode, Node* _endnode)
{
	std::vector<Node*>* path = new std::vector<Node*>; 

	Node* currentnode = _endnode;

	while(currentnode != _startnode)
	{
		path->push_back(currentnode);
		currentnode = currentnode->GetParent();
	}
	
	std::reverse(path->begin(), path->end());

	return path;
}

/// GETMANHATTANDISTANCE 
///
/// For our manhattandistance we get the absolute value of a's grid x position 
/// minus b's grid x position. We also get the same for y and then return the 
/// movementcost to b multiplied by dstX + dstY. Giving us an admissible distance 
/// to b.
float PathFinder::GetManhattanDistance(Node* _nodeA, Node* _NodeB)
{
	int dstX = std::abs(_nodeA->GetGridX() - _NodeB->GetGridX());
	int dstY = std::abs(_nodeA->GetGridY() - _NodeB->GetGridY());
	
	return (_NodeB->GetMovementCost() * (dstX + dstY));
}

/// GETEUCLIDEANDISTANCE 
///
/// Using the principles of pythagoras, we return the cost of movement of b multiplied by the square root of 
/// a's grid x - b's grid x (squared) plus the same for the y axis. Returning the euclidean distance from b. 
/// This function was included as an experiment to see how it would affect the algorithm's performance in terms
/// of the amount of nodes it would evaluate before retrieving the optimal path. After experimentation it's conclusive
/// that the euclidean distance caused the algorithm to evaluate more nodes before returning the best path, therefore not 
/// the ideal heuristic to use. 
float PathFinder::GetEuclideanDistance(Node* _nodeA, Node* _NodeB)
{
	return _NodeB->GetMovementCost() * (std::sqrt(std::pow(_nodeA->GetGridX() - _NodeB->GetGridX(), 2) + 
		std::pow(_nodeA->GetGridY() - _NodeB->GetGridY(), 2)));
}

/// ISNODEINLIST 
///
/// Simple function to check if the node current is in the list passed in. 
/// We simply iterate over the list and if the list at index i is equal to current 
/// then we return true, else we return false. 
bool PathFinder::IsNodeInList(std::vector<Node*>* _list, Node* _current)
{
	for(int i = 0; i < _list->size(); i++)
	{
		if(_list->at(i) == _current)
		{
			return true;
		}
	}
	
	return false;
}
