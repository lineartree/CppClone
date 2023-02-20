#pragma once

#include "texture_manager.h"
#include "../models/model.h"
#include <cstring>
#include <memory>
#include <map>

class BlockType {
public:
  struct Vert {
    std::vector<float> value;
    int rows;
    int cols;
  };

  struct Tex {
    std::vector<float> value;
    int rows;
    int cols;
  };

  struct Shading {
    std::vector<float> value;
    int rows;
    int cols;
  };
  
  Vert vertex_positions;
  Tex tex_coords;
  Shading shading_values;
  bool isCube;
  bool transparent;
  BlockType() {};
  
  template <typename T>
  BlockType(TextureManager& texture_manager, std::map<std::string, std::string> block_face_textures, T& model)
    : isCube(model.isCube), transparent(model.transparent)
  {
    // static_assert(!std::is_pointer<T>::value, "T cannot be a pointer type");
    vertex_positions.rows = sizeof(model.vertices) / sizeof(model.vertices[0]);
    vertex_positions.cols = sizeof(model.vertices[0]) / sizeof(model.vertices[0][0]);
    for (int i = 0; i < vertex_positions.rows; i++)
      for (int j = 0; j < vertex_positions.cols; j++)
        vertex_positions.value.emplace_back(model.vertices[i][j]);
    
    tex_coords.rows = sizeof(model.tex_coords) / sizeof(model.tex_coords[0]);
    tex_coords.cols = sizeof(model.tex_coords[0]) / sizeof(model.tex_coords[0][0]);
    for (int i = 0; i < tex_coords.rows; i++)
      for (int j = 0; j < tex_coords.cols; j++)
        tex_coords.value.emplace_back(model.tex_coords[i][j]);
    
    shading_values.rows = sizeof(model.shading_values) / sizeof(model.shading_values[0]);
    shading_values.cols = sizeof(model.shading_values[0]) / sizeof(model.shading_values[0][0]);
    for (int i = 0; i < shading_values.rows; i++)
      for (int j = 0; j < shading_values.cols; j++)
        shading_values.value.emplace_back(model.shading_values[i][j]);
   
    
    auto set_block_face = [&](int face, int texture) {
      if (face > tex_coords.rows - 1)
        return;
      for (int vertex = 0; vertex < 4; vertex++)
        tex_coords.value[face * 12 + vertex * 3 + 2] = texture;
    };
    

    for (const auto& [face, texture] : block_face_textures)
    {
      texture_manager.add_texture(texture);

      std::vector<std::string> textures = texture_manager.textures;
      int texture_index = std::distance(textures.begin(), std::find(textures.begin(), textures.end(), texture));

      if (face == "all")
      { // set the texture for all faces if "all" is specified
        set_block_face(0, texture_index);
        set_block_face(1, texture_index);
        set_block_face(2, texture_index);
        set_block_face(3, texture_index);
        set_block_face(4, texture_index);
        set_block_face(5, texture_index);
      } else if (face == "sides")
      { // set the texture for only the sides if "sides" is specified
        set_block_face(0, texture_index);
        set_block_face(1, texture_index);
        set_block_face(4, texture_index);
        set_block_face(5, texture_index);
      } else
      { // set the texture for only one of the sides if one of the sides is specified
        const std::vector<std::string> sides = {"right", "left", "top", "bottom", "front", "back"};
        set_block_face(std::distance(sides.begin(), std::find(sides.begin(), sides.end(), face)), texture_index);
      }
    }
  }
private:
};
