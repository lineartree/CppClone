#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <map>

#include "camera.h"


class Gui
{
public:
  Gui();
  
  void init(GLFWwindow* window);
  void render(Camera camera, float lastX, float lastY);

  ~Gui();
};

