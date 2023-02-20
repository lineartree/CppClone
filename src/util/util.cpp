#include "../gfx/window.h"
#include "util.h"

void Fps::show_fps(GLFWwindow* window)
{
  auto currentTime = std::chrono::steady_clock::now();
  frameCount++;
  if (currentTime - lastTime >= std::chrono::seconds(1))
  {
    std::string title = "Minecraft Clone [" + std::to_string(frameCount) + " fps]"; 
    glfwSetWindowTitle(window, title.c_str());
    frameCount = 0;
    lastTime += std::chrono::seconds(1);
  }
}
