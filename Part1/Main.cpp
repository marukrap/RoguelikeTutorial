#include <SFML/Graphics.hpp>

int main()
{
	constexpr int tileWidth = 16;
	constexpr int tileHeight = 16;

	constexpr int screenWidth = 40;
	constexpr int screenHeight = 30;

	constexpr int windowWidth = tileWidth * screenWidth;
	constexpr int windowHeight = tileHeight * screenHeight;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Hello, Roguelike!");
	window.setVerticalSyncEnabled(true);

	sf::Texture texture;
	texture.loadFromFile("tileset_ascii_16x16.png");

	int playerX = screenWidth / 2;
	int playerY = screenHeight / 2;
	
	sf::Sprite player(texture);
	player.setTextureRect(sf::IntRect(0, 4 * tileWidth, tileWidth, tileHeight));
	player.setPosition(playerX * tileWidth, playerY * tileHeight);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Left:
					--playerX;
					break;

				case sf::Keyboard::Right:
					++playerX;
					break;

				case sf::Keyboard::Up:
					--playerY;
					break;

				case sf::Keyboard::Down:
					++playerY;
					break;
				}

				player.setPosition(playerX * tileWidth, playerY * tileHeight);
			}
		}

		window.clear();
		window.draw(player);
		window.display();
	}

	return 0;
}
