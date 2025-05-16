#include "Game/Game.h"
#include "GLPipeline/SDLGLWindow.h"
#include "Game/Scene.h"
#include <memory>

Game::Game()
{
    m_Window = std::make_unique<SDLGLWindow>("MyGame", 1000, 800);
    m_Scene = std::make_shared<Scene>();
}

Game::~Game()
{

}

void Game::LoadGame()
{
    const char* L_playerPath = "./resources/models/curuthers.obj";
    const char* L_playerTexPath = "./resources/textures/Whiskers_diffuse.png";

    std::shared_ptr<Model> L_model = std::make_shared<Model>(L_playerPath);
    std::shared_ptr<Texture> L_tex = std::make_shared<Texture>(L_playerTexPath);

    // Create main scene
    std::shared_ptr<GameEntity> L_playerPtr = std::make_shared<GameEntity>(L_model, L_tex, glm::vec3(5.0f), glm::vec3(3.0f, 2.0f, 3.0f), false);
    
    m_Scene->AddEntity(L_playerPtr);
    m_Scene->SetPlayer(L_playerPtr);

    const char* L_boxPath = "./resources/models/cube.obj";
    std::shared_ptr<Model> L_cube = std::make_shared<Model>(L_boxPath);

    std::shared_ptr<GameEntity> L_floor = std::make_shared<GameEntity>(glm::vec3(0.0f), glm::vec3(1000.0f, 1.0f, 1000.0f), true);
    m_Scene->AddEntity(L_floor);

    std::shared_ptr<GameEntity> L_cubeEnt = std::make_shared<GameEntity>(glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(1.0f), false);
    m_Scene->AddEntity(L_cubeEnt);

    m_Scene->AddElement("./resources/textures/untitled.png", glm::vec2(10, 10), glm::vec2(300, 100));

    std::shared_ptr<GameEntity> L_sparkCube = std::make_shared<GameEntity>(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(1.0f), false);
    std::shared_ptr<ShaderProgram> L_sparkShader = std::make_shared<ShaderProgram>("./resources/shaders/sparks/vert.vs", "./resources/shaders/sparks/frag.fs");
    L_sparkCube->SetShader(L_sparkShader);

    m_Scene->AddEntity(L_sparkCube);
}

void Game::Play()
{
    Uint64 L_oldTime = SDL_GetTicks();
    Uint64 L_newTime;
    float L_deltaTime;

    do
    {
        L_newTime = SDL_GetTicks();
        L_deltaTime = (float)(L_newTime - L_oldTime) / 1000.0f; 
        // Get key inputs from window
        const Uint8* L_inputKeys = m_Window->Update();

        printf("Delta: %f\n", L_deltaTime);
        m_Scene->Update(L_deltaTime, L_inputKeys);

        m_Window->Show();

        std::system("clear");
        L_oldTime = L_newTime;
    } while(!m_Window->m_Quit);
    // ^ Checks at earliest possible moment
}