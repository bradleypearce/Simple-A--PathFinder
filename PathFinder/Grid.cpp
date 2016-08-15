#include "Grid.h"

/// CONSTRUCTOR 
///
/// In the constructor of our grid we only require the list of nodes we want to turn into an array. We then define our 
/// gridsize x and gridsize y variables ( the size of each axis in nodes). We set them to 20 x 12 as that is how our tilemap 
/// is laid out and is predefined as this. The issue here is if our tilemap has 13 columns or 21 rows suddenly then this would 
/// also need to be changed. I have chosen this way to do it as the map doesn't need to change that much. 
/// Afterwards we set the grid (initialise our node array to represent each node in the grid) and then set the neighbours for each
/// node. We then create our path container and set all of our flags to false. We also set the path counter to 0. 
Grid::Grid(std::vector<Node*>* _nodes)
{
	m_gridsize_x = 20;
	m_gridsize_y = 12;
	SetGrid(_nodes);
	SetGridNeighbours();
	m_path = new std::vector<Node*>;
	m_nodestartset = false;
	m_unitplaced = false;
	m_endnodeset = false;
	m_findingpath = false;
	m_pathfound = false;
	m_pathcounter = 0;
}

/// DECONSTRUCTOR
///
/// Here we only clear the path of any nodes and delete the container. 
Grid::~Grid()
{
	m_path->clear();
	delete m_path;
}

/// SETGRID 
///
/// The set grid function take in the list of nodes from our tilemap class and 
/// sets it's position in the grid. The nodes are all sequentially added so each 
/// node in the grid is correctly positioned. We use j and k as indexes to set each 
/// pointer in the grid to the correct node. Then for each node in the list of nodes, 
/// we set the grid position at j and k to the node at i in the list. Everytime we iterate 
/// we add to k and i and check if k has gone over 19, if it has, were at the next column and add to
/// our j index. We also set the gridx and gridy of each node to the values of k and j each time we 
/// initialise an element in our grid. 
void Grid::SetGrid(std::vector<Node*>* _nodes)
{
	int j = 0;
	int k = 0;

	for(unsigned int i = 0; i < _nodes->size(); i++)
	{
		if(k > 19)
		{
			j++;
			k = 0;
		}
		
		m_grid[j][k] = _nodes->at(i);
		_nodes->at(i)->SetGridX(k);
		_nodes->at(i)->SetGridY(j);

		k++;
	}
}

/// FINDNODEFROMPOINT 
///
/// This function finds the node from the position given as a parameter. This is useful for finding which
/// node the user clicked by giving the position of the map. We define our percentX and percentY variables 
/// and then (checking if the position is not off the screen) we assign each percent value to the position
/// on the axis divided by the grid size (in nodes) times by 64 (the size of each node on screen). This gives us
/// a value between 0 and 1 which acts as an overall percentage of the position on the screen. We make sure each percentage
/// indefinately if between 1 and 0 using a couple if statements and then define x and y integers as the closest integer to 
/// our gridsize on each axis multiplied by our percentage for each axis. X and Y then become the node were looking for's index
/// in the grid. We return the node at Y,X index. 
Node* Grid::FindNodeFromPoint(sf::Vector2f* _position)
{
	float percentX;
	float percentY;

	if(_position->y > 0 && _position->y < 720 && _position->x > 0 && _position->x < 1280)
	{
		percentX = _position->x / (m_gridsize_x * 64);
		percentY = _position->y / (m_gridsize_y * 64);
	}
	else 
	{
		return NULL;
	}
	
	if(percentX > 1)
		percentX = 1;
	if(percentX < 0)
		percentX = 0;

	if(percentY > 1)
		percentY = 1;
	if(percentY < 0)
		percentY = 0;

	int x = RoundToInt((m_gridsize_x) * percentX);
	int y = RoundToInt((m_gridsize_y) * percentY);

	return m_grid[y][x];
}

/// ROUNDTOINT 
///
/// To make the findnodefrompoint function work i had to create a simple member function 
/// to round a float to the nearest integer. First of all pass in the float we want to round 
/// and initialise our integral variable. We then set fraction to the return value of modf which
/// returns whatever is left over after taking _value's whole numbers and adding them to our integral 
/// variable. We then evaluate our fraction and add 1 if our fraction is over 0.5. Else we leave integral
/// as is and return integral casted as an int. 
int Grid::RoundToInt(float _value)
{
	double integral; 
	float fraction = (float)modf(_value, &integral);

	if(fraction >= 0.5)
		integral += 1;

	return (int)integral;
}

/// UPDATE 
///
/// For our update function we take in our window (so our mouse coordinates are relative to the size of the screen), 
/// our pathfinder class, the unit we want to control and delta time (for smoother movement of our unit. We then define
/// a reference to our currentnode as the node at the position of the mouse (using our find node from point function). We then
/// begin a outer and inner loop at the size of our gridsize x and y values (12 and 20). We then check if our current node is 
/// node any of the other nodes in the grid and if they're walkable and if the start and end nodes aren't set. If so we colour the tile 
/// as the default colour the tile is dependant on their movement cost. We then check which node the mouse is over and if it's the node at 
/// i and j we set the colour to yellow. We then check if the start node has been set and if so we check if it's the same node on the grid 
/// and set the colour to yellow so it stays highlighted when set. We then check if the start node is set and then highlight the node with the mouse
/// on as red. We then check if the end node has been set and if so, keep it highlighted as red. Then we check if current is not at node i and j and 
/// if the start and end nodes are not set. If so we colour the tile as the default colour the tile is dependant on their movement cost. Then we check if the
/// path is set and if so we colour in each node in the path blue. After the loops have ended, we check if the mouse has been pressed and if the start node
/// hasn't been set, if not, we set the startnodeset flag to true and define our startnode variable as the node we clicked on. We the check the same for our end node 
/// and if the end node is set we set the end node set flag to true and set the end node to the current node. We then check if the path has already been found and if so
/// we check if the mouse has been clicked again and if the current node is walkable. If so we find a new path relative to the unit's current position and the end node's current position. 
/// We also set our path counter to 0 as well so it moves toward the correct node in the path. If a path has not already been found, we define a new path at the start and end node's position. 
/// If we've right clicked we reset our start node set, our end node set, we clear the path and set pathfound to false and our path counter to 0 so we reset everything. Then we check if our path
/// is not empty we create a flag position reset, we check if the unit has been placed and the unit is not in the radius of the end node. If so we set the position of our unit to the start node's position 
/// We then set positionreset to false and unitplaced to true. Then we check if the unit has been placed and the position has not been reset. If both of these are true we set the position of our unit to 
/// our moveunit function. Then we check if our unit is in the radius of the current node along the path, if so we increment our pathcounter. If we've reached the end of the path we move toward the end node 
/// and if the unit is in the radius of the end node we clear the path container and set unit placed to false. As a final check if the path is empty we set unit placed to false. 
void Grid::Update(sf::RenderWindow* _window, PathFinder* _pathfinder, sf::CircleShape* _unit, float _deltatime)
{
	sf::Vector2f mousepos(sf::Mouse::getPosition((*_window)));
	Node* current = FindNodeFromPoint(&mousepos);

	for(int i = 0; i < 12; i++)
	{
		for(int j = 0; j < 20; j++)
		{
			if(m_grid[i][j] == current && m_grid[i][j]->IsWalkable() && !m_nodestartset && !m_endnodeset)
			{
				current->SetColour(sf::Color::Yellow);
			}

			if(m_grid[i][j] == m_startnode && m_grid[i][j]->IsWalkable() && m_nodestartset && !m_endnodeset)
			{
				m_startnode->SetColour(sf::Color::Yellow);
			}

			if(current != m_endnode && m_nodestartset && current != NULL && current->IsWalkable())
			{
				current->SetColour(sf::Color::Red);
			}

			if(m_grid[i][j] == m_endnode && m_grid[i][j]->IsWalkable() && m_nodestartset && m_endnodeset)
			{
				m_endnode->SetColour(sf::Color::Red);
			}

			if(current != m_grid[i][j] && m_startnode != m_grid[i][j] && m_endnode != m_grid[i][j] && m_grid[i][j]->IsWalkable())
			{
				if(m_grid[i][j]->GetMovementCost() == 10)
				{
					m_grid[i][j]->SetColour(sf::Color::White);
				}

				if(m_grid[i][j]->GetMovementCost() == 30)
				{
					m_grid[i][j]->SetColour(sf::Color::Green);
				}

				if(m_grid[i][j]->GetMovementCost() == 5)
				{
					m_grid[i][j]->SetColour(sf::Color::Cyan);
				}
			}

			for(unsigned int k = 0; k < m_path->size(); k++)
			{
				if(m_path->at(k) == m_grid[i][j] && m_path != NULL)
				{
					m_grid[i][j]->SetColour(sf::Color::Blue);
				}
			}
		}
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && current != NULL && current->IsWalkable() &&!m_nodestartset)
	{
		m_nodestartset = true;
		m_startnode = current;
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && m_nodestartset && current != NULL && current->IsWalkable() && m_startnode != current && !m_endnodeset)
	{
		m_endnodeset = true;
		m_endnode = current;
	}

	if(m_pathfound && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && current != NULL && current->IsWalkable())
	{
		m_endnode = current; 
		sf::Vector2f unitpos = _unit->getPosition();
		m_path = _pathfinder->FindPath(&unitpos, m_endnode->GetPosition()); 
		m_pathcounter = 0;
	}

	if(m_nodestartset && m_endnodeset && !m_pathfound)
	{
		m_path = _pathfinder->FindPath(m_startnode->GetPosition(), m_endnode->GetPosition());
		m_pathfound = true;
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		m_nodestartset = false;
		m_endnodeset = false;
		m_path->clear();
		m_pathfound = false;
		m_pathcounter = 0;
	}

	if(!m_path->empty())
	{
		bool positionreset = false;

		if(!m_unitplaced && !IfInNodeRadius(_unit->getPosition(), m_path->back()))
		{
			_unit->setPosition(m_startnode->GetPosition()->x, m_startnode->GetPosition()->y);
			m_unitplaced = true;
			positionreset = true;
		}
		
		if(m_unitplaced && !positionreset)
		{
			sf::Vector2f unitpos = MoveUnit(_unit, m_path->at(m_pathcounter)->GetPosition(), _deltatime);
			_unit->setPosition(unitpos.x, unitpos.y);
			
			if(IfInNodeRadius(_unit->getPosition(), m_path->at(m_pathcounter)) && m_path->at(m_pathcounter) != m_path->back())
			{
				m_pathcounter++;
			}

			if(m_path->at(m_pathcounter) == m_path->back())
			{
				MoveUnit(_unit, m_endnode->GetPosition(), _deltatime);
				
				if(IfInNodeRadius(_unit->getPosition(), m_endnode))
				{
					m_path->clear();
				}
			}
		}
	}
}

/// MOVEUNIT
///
/// This function takes in our unit (a circleshape we want to move), the position we want to move toward and the amount 
/// of time between this frame and last frame has passed. We return the vector between our unit's position and our node's position
/// which is our current position plus our node'sposition minus our unit's position. We times this vector by 5 as an arbitrary speed value and then 
/// multiply this by deltatime so our movement appears smooth. 
sf::Vector2f Grid::MoveUnit(sf::CircleShape* _unit, sf::Vector2f* _nodepos, float _deltatime)
{
	return _unit->getPosition() + ((*_nodepos) - _unit->getPosition()) * 5.0f * _deltatime;
}

/// IFNODEINRADIUS 
///
/// This function checks if our node is in the radius of our node's position by getting the x component of both
/// our unit's position and our node's position and our corresponding y component and adding the difference between them 
/// (squared) and checking if that value is less than the radius of our node squared. If this is true when the function returns true
/// as the position of the unit is in the area of the node. Otherwise it'll return false. 
bool Grid::IfInNodeRadius(sf::Vector2f _unitpos, Node* _node)
{
	if( std::pow((_unitpos.x - _node->GetPosition()->x), 2) + std::pow((_unitpos.y - _node->GetPosition()->y), 2) < 
		std::pow(_node->GetRadius(),2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/// SETGRIDNEIGHBOURS 
///
/// This function loops around each node in the grid so the outer loop of 12 and and inner loop of 20 
/// and for each index, it checks if i or j is the maximum i or j can be and if so, it will only set value neighbours. 
/// So i is 0 then it won't have an above neighbour and an example. We do this for each node to make sure were not setting null neighbours. 
/// For each neighbour set we call the corresponding setneighbour function to set the correct reference to the neighbouring node. 
void Grid::SetGridNeighbours()
{
	for(int i = 0; i < 12; i++)
	{
		for(int j = 0; j < 20; j++)
		{
			if(i == 0) 
			{
				m_grid[i][j]->SetNeighbourDown(m_grid[i + 1][j]);
					
				if(j == 0)
				{
					m_grid[i][j]->SetNeighbourRight(m_grid[i][j + 1]);
				}
				else if(j == 19)
				{
					m_grid[i][j]->SetNeighbourLeft(m_grid[i][j - 1]);
				}
				else
				{
					m_grid[i][j]->SetNeighbourRight(m_grid[i][j+1]);
					m_grid[i][j]->SetNeighbourLeft(m_grid[i][j-1]);
				}
			}
			else if(i == 11)
			{
				m_grid[i][j]->SetNeighbourUp(m_grid[i-1][j]);
					
				if(j == 0)
				{
					m_grid[i][j]->SetNeighbourRight(m_grid[i][j+1]);
				}
				else if(j == 19)
				{
					m_grid[i][j]->SetNeighbourLeft(m_grid[i][j-1]);
				}
				else
				{
					m_grid[i][j]->SetNeighbourRight(m_grid[i][j+1]);
					m_grid[i][j]->SetNeighbourLeft(m_grid[i][j-1]);
				}
			}
			else
			{
				m_grid[i][j]->SetNeighbourUp(m_grid[i-1][j]);
				m_grid[i][j]->SetNeighbourDown(m_grid[i+1][j]);
					
				if(j == 0)
				{
					m_grid[i][j]->SetNeighbourRight(m_grid[i][j+1]);
				}
				else if(j == 19)
				{
					m_grid[i][j]->SetNeighbourLeft(m_grid[i][j-1]);
				}
				else
				{
					m_grid[i][j]->SetNeighbourRight(m_grid[i][j+1]);
					m_grid[i][j]->SetNeighbourLeft(m_grid[i][j-1]);
				}
			}
		}
	}
}
