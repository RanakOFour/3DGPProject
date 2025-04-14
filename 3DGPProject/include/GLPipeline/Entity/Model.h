#ifndef MODEL_H

#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <vector>

struct Vertex
{
  Vertex();

  glm::vec3 position;
  glm::vec2 texcoord;
  glm::vec3 normal;
};
  
struct Face
{
  Vertex a;
  Vertex b;
  Vertex c;
  glm::vec3 normal;
  void CalculateNormal()
  {
    glm::vec3 ac = c.position - a.position;
    glm::vec3 ab = b.position - a.position;
    
    normal = glm::cross(ac, ab);
  };
};  

class Model
{
  std::vector<Face> m_faces;
  GLuint m_vaoId;
  GLuint m_vboId;
  bool m_dirty;

  void SplitStringWhitespace(const std::string& _input,
    std::vector<std::string>& _output);

  void SplitString(const std::string& _input, char _splitter,
    std::vector<std::string>& _output);

  public:
   
  Model();
  Model(const std::string& _path);

  Model(const Model& _copy);
  Model& operator=(const Model& _assign);
  virtual ~Model();

  std::vector<Face>& GetFaces();

  GLsizei GetVertexCount() const;
  GLuint GetVAO();
};

#include <stdexcept>


inline Model::Model()
  : m_vboId(0)
  , m_vaoId(0)
  , m_dirty(false)
{ }

inline Model::Model(const std::string& _path)
  : m_vboId(0)
  , m_vaoId(0)
  , m_dirty(false)
{
  std::vector<glm::vec3> L_positions;
  std::vector<glm::vec2> L_tcs;
  std::vector<glm::vec3> L_normals;
  std::string L_currentline;

  std::ifstream file(_path.c_str());

  if(!file.is_open())
  {
    throw std::runtime_error("Failed to open model [" + _path + "]");
  }

  while(!file.eof())
  {
    std::getline(file, L_currentline);
    if(L_currentline.length() < 1) continue;

    std::vector<std::string> L_tokens;
    SplitStringWhitespace(L_currentline, L_tokens);
    if(L_tokens.size() < 1) continue;

    if(L_tokens.at(0) == "v" && L_tokens.size() >= 4)
    {
      glm::vec3 p(atof(L_tokens.at(1).c_str()),
        atof(L_tokens.at(2).c_str()),
        atof(L_tokens.at(3).c_str()));

      L_positions.push_back(p);
    }
    else if(L_tokens.at(0) == "vt" && L_tokens.size() >= 3)
    {
      glm::vec2 tc(atof(L_tokens.at(1).c_str()),
        1.0f - atof(L_tokens.at(2).c_str()));

      L_tcs.push_back(tc);
    }
    else if(L_tokens.at(0) == "vn" && L_tokens.size() >= 4)
    {
      glm::vec3 n(atof(L_tokens.at(1).c_str()),
        atof(L_tokens.at(2).c_str()),
        atof(L_tokens.at(3).c_str()));

      L_normals.push_back(n);
    }
    else if(L_tokens.at(0) == "f" && L_tokens.size() >= 4)
    {
      Face f;
      std::vector<std::string> sub;
      SplitString(L_tokens.at(1), '/', sub);
      if(sub.size() >= 1) f.a.position = L_positions.at(atoi(sub.at(0).c_str()) - 1);
      if(sub.size() >= 2) f.a.texcoord = L_tcs.at(atoi(sub.at(1).c_str()) - 1);
      if(sub.size() >= 3) f.a.normal = L_normals.at(atoi(sub.at(2).c_str()) - 1);

      for(size_t ti = 2; ti + 1 < L_tokens.size(); ti++)
      {
        SplitString(L_tokens.at(ti), '/', sub);
        if(sub.size() >= 1) f.b.position = L_positions.at(atoi(sub.at(0).c_str()) - 1);
        if(sub.size() >= 2) f.b.texcoord = L_tcs.at(atoi(sub.at(1).c_str()) - 1);
        if(sub.size() >= 3) f.b.normal = L_normals.at(atoi(sub.at(2).c_str()) - 1);

        SplitString(L_tokens.at(ti + 1), '/', sub);
        if(sub.size() >= 1) f.c.position = L_positions.at(atoi(sub.at(0).c_str()) - 1);
        if(sub.size() >= 2) f.c.texcoord = L_tcs.at(atoi(sub.at(1).c_str()) - 1);
        if(sub.size() >= 3) f.c.normal = L_normals.at(atoi(sub.at(2).c_str()) - 1);

        m_faces.push_back(f);
        m_dirty = true;
      }
    }
  }
}

inline Model::~Model()
{
  if(m_vaoId)
  {
    glDeleteVertexArrays(1, &m_vaoId);
  }

  if(m_vboId)
  {
    glDeleteBuffers(1, &m_vboId);
  }
}

inline Model::Model(const Model& _copy)
  : m_vaoId(0)
  , m_vboId(0)
  , m_faces(_copy.m_faces)
  , m_dirty(true)
{ }

inline Model& Model::operator=(const Model& _assign)
{
  m_faces = _assign.m_faces;
  m_dirty = true;

  return *this;
}

inline void Model::SplitStringWhitespace(const std::string& _input,
  std::vector<std::string>& _output)
{
  std::string curr;

  _output.clear();

  for(size_t i = 0; i < _input.length(); i++)
  {
    if(_input.at(i) == ' ' ||
      _input.at(i) == '\r' ||
      _input.at(i) == '\n' ||
      _input.at(i) == '\t')
    {
      if(curr.length() > 0)
      {
        _output.push_back(curr);
        curr = "";
      }
    }
    else
    {
      curr += _input.at(i);
    }
  }

  if(curr.length() > 0)
  {
    _output.push_back(curr);
  }
}

inline void Model::SplitString(const std::string& _input, char _splitter,
  std::vector<std::string>& _output)
{
  std::string curr;

  _output.clear();

  for(size_t i = 0; i < _input.length(); i++)
  {
    if(_input.at(i) == _splitter)
    {
      _output.push_back(curr);
      curr = "";
    }
    else
    {
      curr += _input.at(i);
    }
  }

  if(curr.length() > 0)
  {
    _output.push_back(curr);
  }
}

inline std::vector<Face>& Model::GetFaces()
{
  return m_faces;
}

inline GLuint Model::GetVAO()
{
  if(!m_faces.size())
  {
    throw std::runtime_error("Model is empty");
  }

  if(!m_vboId)
  {
    glGenBuffers(1, &m_vboId);

    if(!m_vboId)
    {
      throw std::runtime_error("Failed to generate vertex buffer");
    }
  }

  if(!m_vaoId)
  {
    glGenVertexArrays(1, &m_vaoId);

    if(!m_vaoId)
    {
      throw std::runtime_error("Failed to generate vertex array");
    }
  }

  if(m_dirty)
  {
    std::vector<GLfloat> data;

    for(size_t fi = 0; fi < m_faces.size(); ++fi)
    {
      data.push_back(m_faces[fi].a.position.x);
      data.push_back(m_faces[fi].a.position.y);
      data.push_back(m_faces[fi].a.position.z);
      data.push_back(m_faces[fi].a.texcoord.x);
      data.push_back(m_faces[fi].a.texcoord.y);
      data.push_back(m_faces[fi].a.normal.x);
      data.push_back(m_faces[fi].a.normal.y);
      data.push_back(m_faces[fi].a.normal.z);

      data.push_back(m_faces[fi].b.position.x);
      data.push_back(m_faces[fi].b.position.y);
      data.push_back(m_faces[fi].b.position.z);
      data.push_back(m_faces[fi].b.texcoord.x);
      data.push_back(m_faces[fi].b.texcoord.y);
      data.push_back(m_faces[fi].b.normal.x);
      data.push_back(m_faces[fi].b.normal.y);
      data.push_back(m_faces[fi].b.normal.z);

      data.push_back(m_faces[fi].c.position.x);
      data.push_back(m_faces[fi].c.position.y);
      data.push_back(m_faces[fi].c.position.z);
      data.push_back(m_faces[fi].c.texcoord.x);
      data.push_back(m_faces[fi].c.texcoord.y);
      data.push_back(m_faces[fi].c.normal.x);
      data.push_back(m_faces[fi].c.normal.y);
      data.push_back(m_faces[fi].c.normal.z);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data.at(0)), &data.at(0), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(m_vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
      8 * sizeof(data.at(0)), (void*)0);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
      8 * sizeof(data.at(0)), (void*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
      8 * sizeof(data.at(0)), (void*)(5 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_dirty = false;
    printf("Model cleaned\n");
  }

  return m_vaoId;
}

inline GLsizei Model::GetVertexCount() const
{
  return (GLsizei)m_faces.size() * 3;
}

inline Vertex::Vertex()
  : position(0, 0, 0)
  , texcoord(0, 0)
  , normal(0, 0, 0)
{ }

#endif