#pragma once

#include <iostream>
#include <thread>
#include <chrono>

#include "../util/gui.h"
#include "../util/camera.h"
#include "../world/world.h"
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

const uint32_t SCREEN_WIDTH = 800;
const uint32_t SCREEN_HEIGHT = 600;

class Window
{
public:
  Window();
  ~Window();
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  void processInput(GLFWwindow* window, World& world);
  GLFWwindow* handle;
  Gui imgui;
  static Camera camera;
  static bool hideCursor;
  static float deltaTime;
  static float lastFrame;
  static float lastX;
  static float lastY;
  const GLFWvidmode* mode;
  
private:
  GLFWmonitor* monitor;
  static bool isCursorHidden;
  static bool isFullscreen;
  
  static bool firstMouse;
  bool key_pressed[GLFW_KEY_LAST];

};

