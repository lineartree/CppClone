#pragma once

#include <cstdint>
#include <tuple>
#include <omp.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../gfx/blocktype.h"
#include "../util/util.h"

class World;

const uint32_t CHUNK_WIDTH = 16;
const uint32_t CHUNK_HEIGHT = 16;
const uint32_t CHUNK_LENGTH = 16;
const uint32_t TOTAL_BLOCKS = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_LENGTH;

class Chunk
{
public:
  Chunk(World& world, glm::vec3 chunk_position);
  void update_mesh();
  void draw();
  void clearBuffers();
  uint32_t blocks[TOTAL_BLOCKS];
  BlockType block_type;
  int position[3];
  
  
private:
  World& world;
  std::vector<float> mesh_vertex_positions;
  std::vector<float> mesh_tex_coords;
  std::vector<float> mesh_shading_values;
  std::vector<uint32_t> mesh_indices;
  void add_face(int face);
  
  int x, y, z;

  bool has_mesh;
  int mesh_index_counter;

  GLuint vao;
  GLuint vertex_vbo;
  GLuint tex_coords_vbo;
  GLuint shading_values_vbo;
  GLuint ebo;
};

