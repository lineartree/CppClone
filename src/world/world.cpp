#include "world.h"


World::World()
  :texture_manager(16, 16, 256)
{
  block_types.emplace_back();
  block_types.emplace_back(BlockType(texture_manager, {{"all", "cobblestone"}}, model.cube)); block_types.emplace_back(BlockType(texture_manager, {{"top","grass"}, {"bottom", "dirt"}, {"sides", "grass_side"}}, model.cube));
  block_types.emplace_back(BlockType(texture_manager, {{"all", "grass"}}, model.cube));
  block_types.emplace_back(BlockType(texture_manager, {{"all", "dirt"}}, model.cube));
  block_types.emplace_back(BlockType(texture_manager, {{"all", "stone"}}, model.cube));
  block_types.emplace_back(BlockType(texture_manager, {{"all", "sand"}}, model.cube));
  block_types.emplace_back(BlockType(texture_manager, {{"all", "planks"}}, model.cube));
  block_types.emplace_back(BlockType(texture_manager, {{"top", "log_top"}, {"bottom", "log_top"}, {"sides", "log_side"}}, model.cube));
  block_types.emplace_back(BlockType(texture_manager, {{"all", "daisy"}}, model.plant));
  block_types.emplace_back(BlockType(texture_manager, {{"all", "rose"}}, model.plant));
  block_types.emplace_back(BlockType(texture_manager, {{"top", "cactus_top"}, {"bottom", "cactus_bottom"}, {"sides", "cactus_side"}}, model.cactus));
  block_types.emplace_back(BlockType(texture_manager, {{"all", "dead_bush"}}, model.plant));
  texture_manager.generate_mipmaps();
  
  chunks = {};
}

// get the block index in a chunk of blocks(CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_LENGTH)
int World::get_block_index(int x, int y, int z)
{
  return x + y * CHUNK_WIDTH + z * CHUNK_WIDTH * CHUNK_HEIGHT;
}

// get the chunk position
inline glm::vec3 World::get_chunk_position(glm::vec3 position)
{
  return glm::vec3(std::floor(position.x / CHUNK_WIDTH),
                   std::floor(position.y / CHUNK_HEIGHT),
                   std::floor(position.z / CHUNK_LENGTH));
}

// get the block's relative position in the chunk
inline glm::vec3 World::get_local_position(glm::vec3 position)
{
  return glm::vec3((int)position.x % CHUNK_WIDTH,
                   (int)position.y % CHUNK_HEIGHT,
                   (int)position.z % CHUNK_LENGTH);
}

// get the block number, means different blocks types like grass, stone or sand
inline int World::get_block_number(glm::vec3 position)
{
  glm::vec3 chunk_position = get_chunk_position(position);

  // if the block is adjacent with another block, return block type air(0)
  if (chunks.count(chunk_position) == 0)
    return 0;
  
  glm::vec3 local = get_local_position(position);

  int block_number = chunks[chunk_position]->blocks[get_block_index(local.x, local.y, local.z)];
  return block_number;
}

// check the block is opaque or not
bool World::is_opaque_block(glm::vec3 position)
{
  BlockType block_type = block_types[get_block_number(position)];
  // if the block type is air, return not opaque
  if (get_block_number(position) == 0)
    return false;
  return !block_type.transparent;
}

void World::set_block(glm::vec3 position, int number)
{
  glm::vec3 chunk_position = get_chunk_position(position);
  if (chunks.count(chunk_position) == 0)
  {
    if (number == 0)
      return;

    chunks[chunk_position] = new Chunk(*this, chunk_position);
  }

  if (get_block_number(position) == number)
    return;
  
  glm::vec3 local_pos = get_local_position(position);
  int lx = (int)local_pos.x;
  int ly = (int)local_pos.y;
  int lz = (int)local_pos.z;

  chunks[chunk_position]->blocks[get_block_index(lx, ly, lz)] = number;
  chunks[chunk_position]->update_mesh();

  float cx = chunk_position.x;
  float cy = chunk_position.y;
  float cz = chunk_position.z;

  auto try_update_chunk_mesh = [&](glm::vec3 chunk_position) {
    if (chunks.count(chunk_position) != 0)
      chunks[chunk_position]->update_mesh();
  };

  if (lx == CHUNK_WIDTH - 1) try_update_chunk_mesh(glm::vec3(cx + 1, cy, cz));
  if (lx == 0) try_update_chunk_mesh(glm::vec3(cx - 1, cy, cz));
  if (ly == CHUNK_HEIGHT - 1) try_update_chunk_mesh(glm::vec3(cx, cy + 1, cz));
  if (ly == 0) try_update_chunk_mesh(glm::vec3(cx, cy - 1, cz));
  if (lz == CHUNK_LENGTH - 1) try_update_chunk_mesh(glm::vec3(cx, cy, cz + 1));
  if (lz == 0) try_update_chunk_mesh(glm::vec3(cx, cy, cz - 1));
  
}

void World::draw()
{
  for (auto& chunk_item : chunks)
  {
    auto chunk_position = chunk_item.first;
    chunks[chunk_position]->draw();
  }
}

void World::world_choice(int choice)
{
  // clear all blocks
  for (auto& chunk_item : chunks)
  {
    auto chunk_position = chunk_item.first;
    chunks[chunk_position]->clearBuffers();
    
    delete chunk_item.second;
  }
  chunks.clear();
  
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> distribution(0, 20);

  for (int x = 0; x < 8; x++)
    for (int z = 0; z < 8; z++)
    {
      glm::vec3 chunk_position(x - 4, -1, z - 4);
      Chunk current_chunk(*this, chunk_position);

      for (uint32_t i = 0; i < CHUNK_WIDTH; i++)
        for (uint32_t j = 0; j < CHUNK_HEIGHT; j++)
          for (uint32_t k = 0; k < CHUNK_LENGTH; k++)
          {
            
            auto num = distribution(generator);
            // flowers - grass - cobblestone
            if (j < 14) // interior
              current_chunk.blocks[get_block_index(i, j, k)] = (num < 12) ? 1 : 0;
            else if (j == 14) // surface
              current_chunk.blocks[get_block_index(i, j, k)] = (num < 10) ? 3 : 0;
            // check the ground is sand and then add models
            else if (current_chunk.blocks[get_block_index(i, j - 1, k)] == 3)
              current_chunk.blocks[get_block_index(i, j, k)]  = (num < 1) ? 9 : (num < 3) ? 10 : 0;
            
            if (choice == 0)
              continue;
            
            // cactus - sand - stone
            if (j < 14) // interior
              current_chunk.blocks[get_block_index(i, j, k)] = (num < 12) ? 5 : 0;
            else if (j == 14) // surface
              current_chunk.blocks[get_block_index(i, j, k)] = (num < 10) ? 6 : 0;
            // check the ground is sand and then add models
            else if (current_chunk.blocks[get_block_index(i, j - 1, k)] == 6)
              current_chunk.blocks[get_block_index(i, j, k)]  = (num < 1) ? 11 : (num < 3) ? 12 : 0;
          }
      chunks[chunk_position] =  new Chunk(current_chunk);
    }
  
  
  
  for (auto& chunk_item : chunks)
  {
    auto chunk_position = chunk_item.first;
    chunks[chunk_position]->update_mesh();
  }
  
}

World::~World()
{
  for (auto& chunk_item : chunks)
  {
    auto chunk_position = chunk_item.first;
    chunks[chunk_position]->clearBuffers();
    delete chunk_item.second;
  }
}
