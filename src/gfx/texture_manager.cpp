#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "texture_manager.h"

TextureManager::TextureManager(int width, int height, int numTextures)
  : m_width(width), m_height(height), m_numTextures(numTextures)
{
  // Generate and bind the texture
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D_ARRAY, id);

  // Set texture parameters
  /* glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT); */
  /* glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT); */
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, numTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void TextureManager::add_texture(std::string texture)
{
  if (std::find(textures.begin(), textures.end(), texture) == textures.end())
  {
    textures.emplace_back(texture);
    
    int width, height, channels;
    std::string path = "res/images/" + texture + ".png";
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data)
      std::cerr << "Error loading texture " << path << std::endl;

    int index = std::distance(textures.begin(), std::find(textures.begin(), textures.end(), texture));
    glBindTexture(1, id);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, m_width, m_height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
  }
}

void TextureManager::generate_mipmaps()
{
  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void TextureManager::bind() const
{
  glBindTexture(GL_TEXTURE_2D_ARRAY, id);
}

void TextureManager::unbind() const
{
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

TextureManager::~TextureManager()
{
  glDeleteTextures(1, &id);
}
