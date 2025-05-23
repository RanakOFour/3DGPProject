#include <stdexcept>
#include <glm/ext.hpp>

#include "Game/Game.h"
#include <memory>

#undef main

int main()
{
	std::shared_ptr<Game> L_game = std::make_shared<Game>();
	L_game->GetWindow()->SetGame(L_game);
	L_game->GetScene()->SetGame(L_game);
	L_game->LoadGame();
	
	L_game->Play();
}