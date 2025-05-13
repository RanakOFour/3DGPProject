#include "Game/Game.h"
#include "GLPipeline/SDLGLWindow.h"
#include "Game/Scene.h"
#include <memory>

Game::Game()
{
    m_Window = std::make_unique<SDLGLWindow>("MyGame", 1000, 800);
    m_Scene = std::make_shared<Scene>(glm::vec3(1000.0f), 50);
    LoadGame();
}

Game::~Game()
{

}

void Game::LoadGame()
{
    const char* L_playerPath = "./resources/curuthers/curuthers.obj";
    const char* L_playerTexPath = "./resources/curuthers/Whiskers_diffuse.png";
    // Create main scene
    std::shared_ptr<GameEntity> L_playerPtr = std::make_shared<GameEntity>(L_playerPath, L_playerTexPath, glm::vec3(0.0f));
    
    m_Scene->AddEntity(L_playerPtr);

    std::shared_ptr<GameEntity> L_floor = std::make_shared<GameEntity>(glm::vec3(0.0f), glm::vec3(1000.0f, 0.1f, 1000.0f));
    m_Scene->AddEntity(L_floor);
}

void Game::Play()
{
    Uint64 L_oldTime = SDL_GetTicks64();
    Uint64 L_newTime;
    float L_deltaTime;

    do
    {
        L_newTime = SDL_GetTicks64();
        L_deltaTime = (float)(L_newTime - L_oldTime) / 1000.0f; 
        // Get key inputs from window
        const Uint8* L_inputKeys = m_Window->Update();

        m_Scene->Update(L_deltaTime, L_inputKeys);

        m_Window->Show();

        std::system("clear");
        L_oldTime = L_newTime;
    } while(!m_Window->m_Quit);
    // ^ Checks at earliest possible moment
}