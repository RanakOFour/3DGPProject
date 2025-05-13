#ifndef GAME_H
#define GAME_H

#include "GLPipeline/SDLGLWindow.h"
#include "Game/Scene.h"
#include <memory>
#include <vector>

class Game
{
    private:
    std::unique_ptr<SDLGLWindow> m_Window;
    std::shared_ptr<Scene> m_Scene;

    void LoadGame();

    public:
    Game();
    ~Game();

    void Play();
    std::unique_ptr<SDLGLWindow>& GetWindow() { return m_Window; };
    std::shared_ptr<Scene> GetScene() { return m_Scene; };
};

#endif
