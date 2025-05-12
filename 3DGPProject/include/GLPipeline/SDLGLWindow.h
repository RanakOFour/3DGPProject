#ifndef SDLGLWINDOW_H
#define SDLGLWINDOW_H

#include "glm/ext.hpp"

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

class Game;
class SDLGLWindow
{
    SDL_Window* m_Window;
    std::weak_ptr<Game> m_Game;

    int m_Width;
    int m_Height;

    glm::vec3 m_MouseDeltas;
    bool RMBDown;

    public:
    bool m_Quit;

    SDLGLWindow(const char* _title, int _width, int _height);
    ~SDLGLWindow();

    void SetGame(std::shared_ptr<Game> _game);
    const Uint8* Update();
    void inline Show() {SDL_GL_SwapWindow(m_Window);};
    glm::vec3 GetMouseInput() { return m_MouseDeltas; };
};

#endif