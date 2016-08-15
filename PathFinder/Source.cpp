#include "Headers.h"

/// MAIN 
///
/// Here we set up our window, our unit and set the fill colour of the shape to magenta. 
/// then we load our tilemap and call our setnode list function so we've instantiated our
/// nodes. Then we define our grid by passing in our nodelist from the tilemap. Then define our
/// pathfinder class, passing in our grid. We also need to know how much time has passed so we set up 
/// a clock and a last frame and a deltatime variable. 
/// Enter the main loop where we define an event e and define our deltatime and last frame time variables. 
/// We check if the window has been closed by polling window events and checking if the window has closed. 
/// We then clear the window every frame, draw our tilemap, update our grid, draw the unit to screen and then display
/// whatever is in our buffer. If the window closes, the program ends.
int main()
{
	sf::RenderWindow window(sf::VideoMode(1280,720), "PathFinder");
	sf::CircleShape unit(25.f);
	unit.setFillColor(sf::Color::Magenta);
	TileMap Tilemap("./tilemap.txt"); 
	Tilemap.SetNodeList();
	Grid grid(Tilemap.GetNodeList());
	PathFinder pathfinder(&grid);
	sf::Clock clock;
	float lastframetime = 0;
	float deltatime = 0;

	while(window.isOpen())
	{
		sf::Event e; 
		
		deltatime = clock.getElapsedTime().asSeconds() - lastframetime;
		lastframetime = clock.getElapsedTime().asSeconds();

		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
			{
				window.close(); 
			}
		}
		
		window.clear(sf::Color::White);
		Tilemap.Draw(&window);
		grid.Update(&window, &pathfinder, &unit, deltatime);
		window.draw(unit);
		window.display(); 
	}

	return 0; 
}