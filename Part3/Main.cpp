#include "Entity.hpp"
#include "Dungeon.hpp"
#include "TileMap.hpp"

int main()
{
	const sf::Vector2i tileSize(16, 16);
	const sf::Vector2i screenSize(80, 45);
	const sf::Vector2i windowSize(tileSize.x * screenSize.x, tileSize.y * screenSize.y);

	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Procedural Dungeon Generation");
	window.setVerticalSyncEnabled(true);

	sf::Texture texture;
	texture.loadFromFile("tileset_ascii_16x16.png");

	Dungeon dungeon(screenSize);
	dungeon.generate();

	TileMap tileMap(texture, tileSize);
	tileMap.load(dungeon);

	Entity player(texture, tileSize, '@');
	player.setPosition(dungeon.findPassableTile());

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

				// Regenerate dungeon
				case sf::Keyboard::Space:
					dungeon.generate();
					tileMap.load(dungeon);
					player.setPosition(dungeon.findPassableTile());
					break;
				}

				if (dx != 0 || dy != 0)
				{
					const sf::Vector2i pos = player.getPosition();
					const sf::Vector2i newPos = { pos.x + dx, pos.y + dy };

					if (newPos.x >= 0 && newPos.y >= 0 && newPos.x < screenSize.x && newPos.y < screenSize.y)
					{
						if (dungeon.getTile(newPos.x, newPos.y).passable)
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
