#include "Game.hpp"

#include <locale>
#include <iostream>

int main()
{
	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale());

	Game game;
	game.run();

	return 0;
}
