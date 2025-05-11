#include <stdexcept>
#include <glm/ext.hpp>

#include "Game/Game.h"
#include <memory>
//#include "Game/GameLoader.h"

#undef main

int main()
{
	std::shared_ptr<Game> L_game = std::make_shared<Game>();

	L_game->Play();
}