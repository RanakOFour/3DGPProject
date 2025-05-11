#ifndef GAMELOADER_H
#define GAMELOADER_H


#include "Resource/ObjectPool.h"
#include "GLPipeline/Texture.h"
#include "GLPipeline/Model.h"

#include <string>

class Game;
class Scene;
class GameLoader
{
    private:
    static ObjectPool<Texture> m_TexturePool;
    static ObjectPool<Model> m_ModelPool;

    static void ReadVector(std::string& _vector, std::vector<float>& _out);
    static void LoadTexture(std::string& _texPath);
    static void LoadModel(std::string& _modelPath);
    static std::shared_ptr<Scene> LoadScene(std::string& _scenePath);
    static void CreateEntity(std::string& _info);

    public:
    // Will load scenes and such out from file
    static void LoadGame(std::string& _scenePath, Game& _game);
    static std::weak_ptr<Model> GetModel(std::string& _path);
    static std::weak_ptr<Texture> GetTexture(std::string& _path);
};

#endif