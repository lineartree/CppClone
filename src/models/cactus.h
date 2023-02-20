#pragma once

struct Cactus
{
  float vertices[6][12] = {
    { 0.4375,  0.5000,  0.5000,  0.4375, -0.5000,  0.5000,  0.4375, -0.5000, -0.5000,  0.4375,  0.5000, -0.5000}, // right
    {-0.4375,  0.5000, -0.5000, -0.4375, -0.5000, -0.5000, -0.4375, -0.5000,  0.5000, -0.4375,  0.5000,  0.5000}, // left
    { 0.5000,  0.5000,  0.5000,  0.5000,  0.5000, -0.5000, -0.5000,  0.5000, -0.5000, -0.5000,  0.5000,  0.5000}, // top
    {-0.5000, -0.5000,  0.5000, -0.5000, -0.5000, -0.5000,  0.5000, -0.5000, -0.5000,  0.5000, -0.5000,  0.5000}, // bottom
    {-0.5000,  0.5000,  0.4375, -0.5000, -0.5000,  0.4375,  0.5000, -0.5000,  0.4375,  0.5000,  0.5000,  0.4375}, // front
    { 0.5000,  0.5000, -0.4375,  0.5000, -0.5000, -0.4375, -0.5000, -0.5000, -0.4375, -0.5000,  0.5000, -0.4375}, // back
  };

  float tex_coords[6][12] = {
    {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0},
    {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0},
    {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0},
    {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0},
    {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0},
    {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0},
  };

  float shading_values[6][4] = {
    {0.6, 0.6, 0.6, 0.6},
    {0.6, 0.6, 0.6, 0.6},
    {1.0, 1.0, 1.0, 1.0},
    {0.4, 0.4, 0.4, 0.4},
    {0.8, 0.8, 0.8, 0.8},
    {0.8, 0.8, 0.8, 0.8},
  };

  bool isCube = false;
  bool transparent = true;
};

