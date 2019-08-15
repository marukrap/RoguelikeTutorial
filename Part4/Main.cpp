#include "Entity.hpp"
#include "Dungeon.hpp"
#include "TileMap.hpp"
#include "Fov.hpp"

int main()
{
	const sf::Vector2i tileSize(16, 16);
	const sf::Vector2i screenSize(80, 45);
	const sf::Vector2i windowSize(tileSize.x * screenSize.x, tileSize.y * screenSize.y);

	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Field of view");
	window.setVerticalSyncEnabled(true);

	sf::Texture texture;
	texture.loadFromFile("tileset_ascii_16x16.png");

	Dungeon dungeon(screenSize);
	dungeon.generate();

	Entity player(texture, tileSize, '@');
	player.setPosition(dungeon.findPassableTile());

	Fov fov;
	fov.initialize(dungeon, 10);
	fov.compute(player.getPosition());

	TileMap tileMap(texture, tileSize);
	tileMap.load(dungeon);

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
						{
							player.move(dx, dy);

							// Recompute fov
							fov.compute(player.getPosition());
							tileMap.load(dungeon);
						}
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
