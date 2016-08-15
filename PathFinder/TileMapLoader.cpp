#include "TileMapLoader.h"

/// CONSTRUCTOR 
///
/// The constructor for our tilemap class reads the data from our 
/// tilemap. To do this we use the ifstream library. We say that if the file is open
/// and were not at the end of the file, we get each line from the the file and stream the 
/// string to value (using the space as a delimeter to avoid white space values). Then we say 
/// if the length of the value string is more than zero, we take in our xx string as the start of
/// the string and find the ',' in the string and stop there. This gives us the x value of the first
/// int we want to read. We do the same for yy except for the end is 1 character after the ','. Then we
/// Then we check in two for loops if the value at i and j (in the xx and yy string respectively) are not digits and 
/// if so we break from the loop. Then we conditionally assign x to the our xx and y to yy strings as long as i and j are the
/// length of our strings, otherwise we assign x and y to -1. Then we push back a vector of our x and y values. This gets done 
/// for each node in our tilemap defined in our tilemap text file. Then we push back the vector of our x and y values to our tempmap container. 
/// Then we push back that vector into our map (a vector of vectors). This gives us a two dimensional container which we can use to define our grid. 
TileMap::TileMap(std::string _maplocation)
{
	m_tile = new sf::RectangleShape(sf::Vector2f(64,64));
	m_walkabletile = new sf::RectangleShape(sf::Vector2f(64,64));
	m_nodelist = new std::vector<Node*>; 

	m_tile->setFillColor(sf::Color::Black);
	m_walkabletile->setFillColor(sf::Color::White);

	std::ifstream file(_maplocation);
	
	if(file.is_open()) 
	{
		while(!file.eof())
		{
			std::string str;
			std::string value;

			std::getline(file,str); 
			std::stringstream stream(str);
		
			while(std::getline(stream, value, ' '))
			{
				if(value.length() > 0)
				{
					std::string xx = value.substr(0 , value.find(','));
					std::string yy = value.substr(value.find(',') + 1);

					int x, y, i, j; 

					for(i = 0; i < xx.length(); i++)
					{
						if(!isdigit(xx[i]))
							break;
					}

					for(j = 0; j < yy.length(); j++)
					{
						if(!isdigit(yy[j]))
						{
							break;
						}
					}

					x = (i == xx.length()) ? atoi(xx.c_str()) : -1; 
					y = (j == xx.length()) ? atoi(yy.c_str()) : -1; 

					m_tempmap.push_back(sf::Vector2i(x,y));
				}
			}

			m_map.push_back(m_tempmap); 
			m_tempmap.clear();
		}
	}
}

/// DECONSTRUCTOR
///
/// Here we simply delete things we added to the heap such as 
/// our tile, walkable tile and everything in our nodelist. 
TileMap::~TileMap()
{
	delete m_tile;
	delete m_walkabletile;

	for(int i = 0; i < m_nodelist->size(); i++)
	{
		delete m_nodelist->at(i);
	}

	delete m_nodelist;
}

/// DRAW
///
/// This function iterates through each node in the nodelist
/// and calls the node's draw function. This requires the window used
/// to render our objects to be passed in. This makes drawing each node 
/// easier and requires only one function call in main to get working. 
void TileMap::Draw(sf::RenderWindow* _window)
{
	for(int i = 0; i < m_nodelist->size(); i++)
	{
		m_nodelist->at(i)->Draw(_window);
	}
}

/// SETNODELIST
///
/// This function uses the previously filled map container to instantiate 
/// our nodes for use in our grid. We use a outer for loop (for each container in our map)
/// and the inner loop (for each element in each container in our map). We then check if the value
/// of each vector is not -1 on either. If they aren't, we check the value of each and define a new 
/// node and a corresponding colour for each to differentiate them. We also define the movement cost of
/// each node here dependant on these values. 0,0 means not walkable, 0,1 means normal walkable node (with a 
/// cost of movement as 10). 1,1 means a bigger cost of movement (movement cost set as 30) and finally 1,0 is
/// set with a movement cost of 5 (a faster node). To be more efficient we clear the map now were not going to use the
/// values anymore. 
void TileMap::SetNodeList()
{
	for(int i = 0; i < m_map.size(); i++)
	{
		for(int j = 0; j < m_map.at(i).size(); j++)
		{
			if(m_map[i][j].x != -1 && m_map[i][j].y != -1)
			{
				if(m_map[i][j].x == 0 && m_map[i][j].y == 0)
				{
					m_nodelist->push_back(new Node(j * 64, i * 64, FindIfTileIsWalkable(i,j), sf::Color::Black, 0));
				}

				if(m_map[i][j].x == 0 && m_map[i][j].y == 1)
				{
					m_nodelist->push_back(new Node(j * 64, i * 64, FindIfTileIsWalkable(i,j), sf::Color::White, 10));
				}

				if(m_map[i][j].x == 1 && m_map[i][j].y == 1)
				{
					m_nodelist->push_back(new Node(j * 64, i * 64, FindIfTileIsWalkable(i,j), sf::Color::Green, 30));
				}

				if(m_map[i][j].x == 1 && m_map[i][j].y == 0)
				{
					m_nodelist->push_back(new Node(j * 64, i * 64, FindIfTileIsWalkable(i,j), sf::Color::Cyan, 5));
				}

			}
		}
	}

	m_map.clear();
}

/// FINDIFTILEISWALKABLE 
///
/// This function takes in the indices of our map and checks if either the x or y value 
/// is 0. If both of them are 0 then the function returns false (meaning it's not walkable). 
/// Else the function returns true. This makes differentiating walkable nodes easier to 
/// identify. 
bool TileMap::FindIfTileIsWalkable(int _mapindex, int _innermapindex)
{
	if(m_map[_mapindex][_innermapindex].x == 0 && m_map[_mapindex][_innermapindex].y == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/// GETNODELIST
///
/// This function returns our nodelist so we can  
/// pass this list over and use these references to our nodes 
/// elsewhere in our program. 
std::vector<Node*>* TileMap::GetNodeList()
{
	return m_nodelist;
}



