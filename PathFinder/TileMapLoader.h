#pragma once

#include "Headers.h"

class Node;

/// CLASS TILEMAP
/// 
/// This class is the basis for which our Nodes get initialised based on the data read in 
/// from a simple file. We also draw our nodes from here from a simple list we compile when
/// instantiating each node. 
class TileMap
{
public:
	TileMap(std::string _maplocation);
	~TileMap();
	void Draw(sf::RenderWindow* _window);
	void SetNodeList();
	bool FindIfTileIsWalkable(int _mapindex, int _innermapindex);
	std::vector<Node*>* GetNodeList();
	void SetGrid(std::vector<Node*> _nodelist);

private: 
	std::vector<std::vector<sf::Vector2i>> m_map;
	std::vector<sf::Vector2i> m_tempmap;
	sf::RectangleShape* m_tile;
	sf::RectangleShape* m_walkabletile;
	std::vector<Node*>* m_nodelist;
};

