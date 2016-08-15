#pragma once

#include "Headers.h" 

/// CLASS NODE 
///
/// The node class acts as an interface for each position on our grid (in squares the size 
/// of 64x64). Each node has properties which define how our pathfinding will work. To encapsulate 
/// all of our properties we use many get and set functions. The properties of each node are as follows: 
///		- The position of our node on the screen. 
///		- The shape (for displaying the node)
///		- The colour of our node.
///		- If the node is walkable or not.
///		- The overall (f) cost of the node relative to the path. 
///		- The cost of moving to the next node in the grid. 
///		- The heuristic cost of the node (this node's distance from the target node). 
///		- The radius of the node relative to it's position (useful for moving toward each node).
///		- The index of the grid's in the x position. 
///		- The index of the grid's in the y position. 
///		- The movement cost of each node. 
///		- The neighbour above this node. 
///		- the neighbour below this node. 
///		- The neighbour to the left of this node. 
///		- the neighbour to the right of this node. 
///		- The parent node of this node (the last node in the path). 
///		- The list of each neighbour for this node. 
///
/// All of these properties are helpful for calculating the best path toward a certain node (or point on screen) and  
/// therefore are contained in this class. 
class Node
{
public:
	Node(int _x, int _y, bool _walkable, const sf::Color _col, int _movementcost);
	~Node();
	void SetDestination(Node* _destination);
	bool IsWalkable();
	float GetFCost();
	float GetHCost();
	float GetGCost();
	void SetHCost(float _hcost);
	void SetGCost(int _gcost);
	void SetFCost(int _fcost);
	sf::Vector2f* GetPosition();
	void Draw(sf::RenderWindow* _window);
	void Node::FindThisNodeInGrid(Node* _grid[12][20]);
	std::vector<Node*>* GetNeighbours();
	void SetGridX(int _gridX);
	void SetGridY(int _gridY);
	int GetGridX();
	int GetGridY();
	int GetMovementCost();
	void SetParent(Node* _parent);
	Node* GetParent();
	void SetColour(sf::Color _colour);
	void SetNeighbourDown(Node* _neighbourdown);
	void SetNeighbourUp(Node* _neighbourup);
	void SetNeighbourLeft(Node* _neighbourleft);
	void SetNeighbourRight(Node* _neighbourright);
	float GetRadius();

private: 
	sf::Vector2f* m_position;
	sf::RectangleShape* m_tile;
	sf::Color m_colour;
	bool m_walkable;
	int m_fcost;
	int m_gcost; 
	float m_hcost; 
	float m_noderadius;
	int m_gridX;
	int m_gridY;
	int m_movementcost;
	Node* m_neighbourup;
	Node* m_neighbourdown;
	Node* m_neighbourleft;
	Node* m_neighbourright;
	Node* m_parent;
	std::vector<Node*>* m_neighbours; 
};

