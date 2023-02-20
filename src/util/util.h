#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <chrono>

class Window;

struct vec3_comparator {
  bool operator()(const glm::vec3& a, const glm::vec3& b) const {
    return a.x < b.x || (a.x == b.x && a.y < b.y) || (a.x == b.x && a.y == b.y && a.z < b.z);
  }
};

struct Fps
{
  std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
  int frameCount = 0;
  void show_fps(GLFWwindow* window);
};

