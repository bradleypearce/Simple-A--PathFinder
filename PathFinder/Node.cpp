#include "Node.h"

/// CONSTRUCTOR 
/// 
/// For our constructor we pass in our the x and y position of this node, whether the node is walkable or not, 
/// the colour we want our node to be and the cost of moving to this node. We then instantiate a new position and 
/// contain our x and y coordinates here. We define our tile as a rectangle shape the size of 64x64 and set the position
/// of our tile to the x and y coordinates we passed into position. We initialise our gcost, hcost and fcost to zero. We set 
/// the colour our tile to the colour we passed in. Set the walkable variable to the passed in walkable flag. We set the radius length
/// to 42. We create a new vector of references to nodes for our neighbours and set the movement cost of our nodes to the passed in movementcost. 
Node::Node(int _x, int _y, bool _walkable, const sf::Color _col, int _movementcost)
{
	m_position = new sf::Vector2f((float)_x, (float)_y);
	m_tile = new sf::RectangleShape(sf::Vector2f(64,64));
	m_tile->setPosition(sf::Vector2f(m_position->x, m_position->y));
	m_gcost = 0; 
	m_fcost = 0; 
	m_hcost = 0;
	m_tile->setFillColor(_col);
	m_walkable = _walkable;
	m_noderadius = 42;
	m_neighbours = new std::vector<Node*>;
	m_movementcost = _movementcost;
}

/// DECONSTRUCTOR 
///
/// Here we delete our position pointer, our tile shape. 
/// We also clear our neighbours list and delete the container of 
/// neighbours. Note we don't delete our neighbour nodes as they are
/// deleted in our tilemaps deconstructor. If we delete the neighbours here
/// we will get exception errors when deconstructing our tilemap. 
Node::~Node()
{
	delete m_position;
	delete m_tile;
	m_neighbours->clear();
	delete m_neighbours;
}

/// ISWALKABLE
///
/// Returns the walkable flag, true if walkable, false if not.
bool Node::IsWalkable()
{
	return m_walkable;
}

/// GETFCOST
///
/// Returns the fitness cost of this node. 
float Node::GetFCost()
{
	return (float)m_fcost;
}

/// GETFCOST
///
/// Returns the heuristic cost of this node (the distance from this node to the target node). 
float Node::GetHCost()
{
	return m_hcost;
}

/// GETGCOST
///
/// Returns the cost of moving to this tile from a neighbouring tile. 
float Node::GetGCost()
{
	return (float)m_gcost;
}

/// SETGCOST 
///
/// Overwrites the current gcost of this node to the passed in gcost.
void Node::SetGCost(int _gcost)
{
	m_gcost = _gcost;
}

/// SETHCOST 
///
/// Overwrites the current hcost of this node to the passed in hcost.
void Node::SetHCost(float _hcost)
{
	m_hcost = _hcost;
}

/// SETFCOST 
///
/// Overwrites the current fcost of this node to the passed in fcost.
void Node::SetFCost(int _fcost)
{
	m_fcost = _fcost;
}

/// DRAW
///
/// Draws the tile to screen using a passed in render window.
void Node::Draw(sf::RenderWindow* _window)
{
	_window->draw(*(m_tile));
}

/// GETPOSITION 
///
/// Returns a reference to the position on screen.
sf::Vector2f* Node::GetPosition()
{
	return m_position;
}

/// GETRADIUS
///
/// Returns the radius of the node. 
float Node::GetRadius()
{
	return m_noderadius;
}

/// GETNEIGHBOURS
///
/// Returns a reference to the list of neighbouring nodes.
std::vector<Node*>* Node::GetNeighbours()
{
	return m_neighbours;
}

/// SETNEIGHBOURDOWN
///
/// This takes in a node to be set as the neighbour below this node. 
/// We set the reference of the below neighbour to the passed in node and 
/// then add the neighbour to the list of neighbours. 
void Node::SetNeighbourDown(Node* _neighbourdown)
{
	m_neighbourdown = _neighbourdown; 
	m_neighbours->push_back(m_neighbourdown);
}

/// SETNEIGHBOURUP
///
/// This takes in a node to be set as the neighbour above this node. 
/// We set the reference of the above neighbour to the passed in node and 
/// then add the neighbour to the list of neighbours. 
void Node::SetNeighbourUp(Node* _neighbourup)
{
	m_neighbourup = _neighbourup;
	m_neighbours->push_back(m_neighbourup);
}

/// SETNEIGHBOURLEFT
///
/// This takes in a node to be set as the neighbour to the left of this node. 
/// We set the reference of the neighbour to the passed in node and 
/// then add the neighbour to the list of neighbours. 
void Node::SetNeighbourLeft(Node* _neighbourleft)
{
	m_neighbourleft = _neighbourleft;
	m_neighbours->push_back(m_neighbourleft);
}

/// SETNEIGHBOURRIGHT
///
/// This takes in a node to be set as the neighbour to the right of this node. 
/// We set the reference of the neighbour to the passed in node and 
/// then add the neighbour to the list of neighbours. 
void Node::SetNeighbourRight(Node* _neighbourright)
{
	m_neighbourright = _neighbourright;
	m_neighbours->push_back(m_neighbourright);
}

/// SETGRIDX 
///
/// Sets the x position of this node in the grid to the passed in value. 
void Node::SetGridX(int _gridX)
{
	m_gridX = _gridX;
}

/// SETGRIDY
///
/// Sets the y position of this node in the grid to the passed in value. 
void Node::SetGridY(int _gridY)
{
	m_gridY = _gridY;
}

/// GETGRIDX
///
/// Returns the x position of this node in the grid.
int Node::GetGridX()
{
	return m_gridX;
}

/// GETMOVEMENTCOST
///
/// Returns the cost of movement to this node. 
int Node::GetMovementCost()
{
	return m_movementcost;
}

/// GETGRIDY 
///
/// Returns the y position of this node in the grid. 
int Node::GetGridY()
{
	return m_gridY;
}

/// SETPARENT
///
/// Sets the parent of this node to the passed in node. 
void Node::SetParent(Node* _parent)
{
	m_parent = _parent;
}

/// GETPARENT
///
/// Returns a reference to the parent node of this node. 
Node* Node::GetParent()
{
	return m_parent;
}

/// SETCOLOUR
///
/// Sets the colour of the tile to the passed in colour. 
void Node::SetColour(sf::Color _colour)
{
	m_tile->setFillColor(_colour);
}