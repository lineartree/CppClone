#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

class TextureManager
{
public:
  unsigned int id;
  std::string image;

  TextureManager(int width, int height, int maxTextures);
  TextureManager(const char* path);
  ~TextureManager();
  void bind() const;
  void unbind() const;
  void add_texture(std::string texture);
  void generate_mipmaps();
  std::vector<std::string> textures;

private:
  int m_width;
  int m_height;
  int m_numTextures;
  std::string path;
};
