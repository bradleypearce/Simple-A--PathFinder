#pragma once

#include "Headers.h"

class Node; 
class PathFinder;

/// CLASS GRID 
///
/// The grid class is used as a container class for our two dimensional array of nodes. The grid class also has  
/// an update function so a user can define a start and end position for our pathfinding algorithm. We also set the
/// neighbours of each of our nodes here as it's simple to do with a two dimensional array of our nodes. We can also find a node
/// using a position on the screen which means we can find at any position on screen rather than relying only on node positions. 
/// We also handle movement in this class hence the moveunit function. To move nodes properly we use the ifnodeinradius to check 
/// if our unit is in the radius of another node. We also contain our path here that the pathfinder returns so we can move our unit 
/// along the path easily from the update function.
class Grid
{
public:
	Grid(std::vector<Node*>* _nodes);
	~Grid();
	void SetGrid(std::vector<Node*>* _nodes);
	void SetGridNeighbours();
	Node* FindNodeFromPoint(sf::Vector2f* _position);
	int RoundToInt(float _value);	
	void Update(sf::RenderWindow* _window, PathFinder* _pathfinder, sf::CircleShape* _unit, float _deltatime);
	sf::Vector2f MoveUnit(sf::CircleShape* _unit, sf::Vector2f* _nodepos, float _deltatime);
	bool IfInNodeRadius(sf::Vector2f _unitpos, Node* _node);
private:  
	int m_gridsize_x;
	int m_gridsize_y;
	Node* m_startnode;
	Node* m_endnode;
	bool m_nodestartset;
	bool m_endnodeset;
	bool m_findingpath;
	bool m_unitplaced;
	bool m_pathfound;
	int m_pathcounter;
	std::vector<Node*>* m_path;
	Node* m_grid[12][20];
};

