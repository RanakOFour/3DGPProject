#ifndef SDLGLWINDOW_H
#define SDLGLWINDOW_H

#include "glm/ext.hpp"

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

struct MouseInfo
{
    glm::vec3 deltas;
    glm::ivec2 position;
    bool RMBDown;
    bool LMBDown;
};

class Game;
class SDLGLWindow
{
    SDL_Window* m_Window;
    std::weak_ptr<Game> m_Game;

    int m_Width;
    int m_Height;

    MouseInfo m_MouseInfo;

    public:
    bool m_Quit;

    SDLGLWindow(const char* _title, int _width, int _height);
    ~SDLGLWindow();

    void SetGame(std::shared_ptr<Game> _game);
    const Uint8* Update();
    void inline Show() {SDL_GL_SwapWindow(m_Window);};
    MouseInfo GetMouseInfo() { return m_MouseInfo; };
    glm::vec3 GetMouseInput() { return m_MouseInfo.deltas; };
    float GetAspectRatio() { return m_Width / m_Height; };
    float GetWidth() { return m_Width; };
    float GetHeight() { return m_Height; };
};

#endif
