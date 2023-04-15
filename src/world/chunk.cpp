#include "chunk.h"
#include "world.h"

Chunk::Chunk(World& world, glm::vec3 chunk_position)
  : world(world)
{
  // this->world = new World();
  position[0] = chunk_position.x * CHUNK_WIDTH;
  position[1] = chunk_position.y * CHUNK_HEIGHT;
  position[2] = chunk_position.z * CHUNK_LENGTH;

  for (uint32_t i = 0; i < TOTAL_BLOCKS; i++)
    blocks[i] = 0;


  has_mesh = false;
  mesh_index_counter = 0;

  // buffers
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  glGenBuffers(1, &vertex_vbo);
  glGenBuffers(1, &tex_coords_vbo);
  glGenBuffers(1, &shading_values_vbo);
  glGenBuffers(1, &ebo);

}

inline void Chunk::add_face(int face)
{
  std::vector<float> vertex_positions(block_type.vertex_positions.value.begin() + face * 12, block_type.vertex_positions.value.begin() + (face + 1) * 12);


  for (int i = 0; i < 4; i++)
  {
    vertex_positions[i * 3 + 0] += x;
    vertex_positions[i * 3 + 1] += y;
    vertex_positions[i * 3 + 2] += z;
  }

  this->mesh_vertex_positions.insert(this->mesh_vertex_positions.end(), vertex_positions.begin(), vertex_positions.end());

  std::vector<int> indices = {0, 1, 2, 0, 2, 3};
  for (int i = 0; i < 6; i++)
    indices[i] += this->mesh_index_counter;

  this->mesh_indices.insert(this->mesh_indices.end(), indices.begin(), indices.end());
  this->mesh_index_counter += 4;

  this->mesh_tex_coords.insert(this->mesh_tex_coords.end(), block_type.tex_coords.value.begin() + face * 12, block_type.tex_coords.value.begin() + (face + 1) * 12);
  this->mesh_shading_values.insert(this->mesh_shading_values.end(), block_type.shading_values.value.begin() + face * 4, block_type.shading_values.value.begin() + (face + 1) * 4);
};

void Chunk::update_mesh()
{
  has_mesh = true;
  mesh_index_counter = 0;
  
  for (uint32_t local_x = 0; local_x < CHUNK_WIDTH; local_x++)
    for (uint32_t local_y = 0; local_y < CHUNK_HEIGHT; local_y++)
      for (uint32_t local_z = 0; local_z < CHUNK_LENGTH; local_z++)
      {
        uint32_t block_number = blocks[world.get_block_index(local_x, local_y, local_z)];

        if (block_number)
        {
          block_type = world.block_types[block_number];

          x = position[0] + local_x;
          y = position[1] + local_y;
          z = position[2] + local_z;
          
          if (block_type.isCube)
          {
            // check the adjacent block is not opaque, means that the adjacent block is air, then we add face
            if (!world.is_opaque_block(glm::vec3(x + 1, y, z))) add_face(0);
            if (!world.is_opaque_block(glm::vec3(x - 1, y, z))) add_face(1);
            if (!world.is_opaque_block(glm::vec3(x, y + 1, z))) add_face(2);
            if (!world.is_opaque_block(glm::vec3(x, y - 1, z))) add_face(3);
            if (!world.is_opaque_block(glm::vec3(x, y, z + 1))) add_face(4);
            if (!world.is_opaque_block(glm::vec3(x, y, z - 1))) add_face(5);
          }
          else
          {
            for (int i = 0; i < block_type.vertex_positions.rows; i++)
              add_face(i);
          }
        }
      }

  // pass mesh data to GPU
  // vertex positions
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_vertex_positions.size(), &mesh_vertex_positions[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // tex coords
  glBindBuffer(GL_ARRAY_BUFFER, tex_coords_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_tex_coords.size(), &mesh_tex_coords[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  // shading values
  glBindBuffer(GL_ARRAY_BUFFER, shading_values_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_shading_values.size(), &mesh_shading_values[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // ebo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh_indices.size(), &mesh_indices[0], GL_STATIC_DRAW);

}

void Chunk::draw()
{
  if (!mesh_index_counter)
    return;

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
}


void Chunk::clearBuffers()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vertex_vbo);
  glDeleteBuffers(1, &tex_coords_vbo);
  glDeleteBuffers(1, &shading_values_vbo);
  glDeleteBuffers(1, &ebo);
}
