#include "Entity.hpp"
#include "TileMap.hpp"

int main()
{
	const sf::Vector2i tileSize(16, 16);
	const sf::Vector2i screenSize(40, 30);
	const sf::Vector2i windowSize(tileSize.x * screenSize.x, tileSize.y * screenSize.y);

	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Roguelike Tutorial Part 2");
	window.setVerticalSyncEnabled(true);

	sf::Texture texture;
	texture.loadFromFile("tileset_ascii_16x16.png");

	Map map(screenSize);
	map.setTile(5, 5, { '#', false, false });
	map.setTile(10, 5, { '#', false, false });
	map.setTile(5, 10, { '#', false, false });

	TileMap tileMap(texture, tileSize);
	tileMap.load(map);

	Entity player(texture, tileSize, '@');
	player.setPosition(screenSize.x / 2, screenSize.y / 2);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			else if (event.type == sf::Event::KeyPressed)
			{
				int dx = 0;
				int dy = 0;

				switch (event.key.code)
				{
				case sf::Keyboard::Left:  --dx; break;
				case sf::Keyboard::Right: ++dx; break;
				case sf::Keyboard::Up:    --dy; break;
				case sf::Keyboard::Down:  ++dy; break;
				}

				if (dx != 0 || dy != 0)
				{
					const sf::Vector2i pos = player.getPosition();
					const sf::Vector2i newPos = { pos.x + dx, pos.y + dy };

					if (newPos.x >= 0 && newPos.y >= 0 && newPos.x < screenSize.x && newPos.y < screenSize.y)
					{
						if (map.getTile(newPos.x, newPos.y).passable)
							player.move(dx, dy);
					}
				}
			}
		}

		window.clear();
		tileMap.draw(window);
		player.draw(window);
		window.display();
	}

	return 0;
}
