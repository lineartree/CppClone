#pragma once

#include <tuple>
#include <map>
#include <cmath>
#include <random>
#include <chrono>
#include <memory>

#include "chunk.h"
#include "../gfx/texture_manager.h"


class World
{
public:
  World();
  ~World();
  int get_block_index(int x, int y, int z);
  glm::vec3 get_chunk_position(glm::vec3 position);
  glm::vec3 get_local_position(glm::vec3 position);
  int get_block_number(glm::vec3 position);
  bool is_opaque_block(glm::vec3 position);
  void set_block(glm::vec3 position, int number);
  void draw();
  void world_choice(int choice);
  std::vector<BlockType> block_types;
  TextureManager texture_manager;
  Model model;
private:
  std::map<glm::vec3, Chunk*, vec3_comparator> chunks;
};

