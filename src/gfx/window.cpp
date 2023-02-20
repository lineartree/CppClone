#include "window.h"
#include "imgui/imgui.h"
#include <GLFW/glfw3.h>

bool Window::isCursorHidden = true;
bool Window::hideCursor = true;
bool Window::isFullscreen = false;
bool Window::firstMouse = true;
float Window::lastX = static_cast<float>(SCREEN_WIDTH) / 2;
float Window::lastY = static_cast<float>(SCREEN_WIDTH) / 2;
float Window::deltaTime = 0.0f;
float Window::lastFrame = 0.0f;
Camera Window::camera(glm::vec3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_LENGTH));

Window::Window()
{ 
  
  // init glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  handle = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minecraft Clone", NULL, NULL);
  if (!handle)
  {
    std::cout << "Failed to create window.\n";
    return;
  }
  
  glfwMakeContextCurrent(handle);
  glfwSetFramebufferSizeCallback(handle, framebuffer_size_callback);
  glfwSetCursorPosCallback(handle, mouse_callback);
  glfwSetScrollCallback(handle, scroll_callback);

  monitor = glfwGetPrimaryMonitor();
  mode = glfwGetVideoMode(monitor);

  GLFWimage icon;
  std::string icon_path = "res/images/icon.png";
  icon.pixels = stbi_load(icon_path.c_str(), &icon.width, &icon.height, 0, 4);
  if (!icon.pixels)
  {
    std::cout << "Error loading " << icon_path << "\n";
    return;
  }
  glfwSetWindowIcon(handle, 1, &icon);
  stbi_image_free(icon.pixels);

  if(!gladLoadGL())
  {
    std::cout << "Failed to init glad." << std::endl;
    return;
  }

  // configure global opengl state
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glFrontFace(GL_CCW);

  // tell GLFW to capture our mouse
  glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  isCursorHidden = true;

  // disable vsync
  // glfwSwapInterval(0);

  imgui.init(handle);
  
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
  glViewport(0, 0, width, height);
}

void Window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
  auto& io = ImGui::GetIO();
  if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    return;
  if (!isCursorHidden)
    return;
  
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  auto& io = ImGui::GetIO();
  if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    return;
  if (!isCursorHidden)
    return;
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void Window::processInput(GLFWwindow* window, World& world)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    isCursorHidden = false;
    hideCursor = false;
  }
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    auto& io = ImGui::GetIO();
    if (io.WantCaptureMouse || io.WantCaptureKeyboard)
      return;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    isCursorHidden = true;
    hideCursor = true;
    firstMouse = true;
  }
  
  // normal speed
  if (isCursorHidden)
  {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      camera.ProcessKeyboard(DOWN, deltaTime);

    // speed up
    if (glfwGetKey(window, GLFW_KEY_W) && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, deltaTime * 2);
    if (glfwGetKey(window, GLFW_KEY_S) && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD, deltaTime * 2);
    if (glfwGetKey(window, GLFW_KEY_A) && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, deltaTime * 2);
    if (glfwGetKey(window, GLFW_KEY_D) && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT, deltaTime * 2);
    if (glfwGetKey(window, GLFW_KEY_SPACE) && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
      camera.ProcessKeyboard(UP, deltaTime * 2);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
      camera.ProcessKeyboard(DOWN, deltaTime * 2);
  }
  
  
  // fullscreen
  if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS && !key_pressed[GLFW_KEY_F11])
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    key_pressed[GLFW_KEY_F11] = true;
  }
  else if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS && key_pressed[GLFW_KEY_F11])
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    glfwSetWindowMonitor(window, nullptr, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    glfwSetWindowPos(window, (mode->width - SCREEN_WIDTH) / 2, (mode->height - SCREEN_HEIGHT) / 2);
    key_pressed[GLFW_KEY_F11] = false;
  }
  
  // place blocks
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_T) == GLFW_REPEAT)
  {
    glm::vec3 position = Window::camera.Position;
    position.z -= 1;
    world.set_block(position, 7);
  }

  // world choice
  if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    world.world_choice(0);
  }
  if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    world.world_choice(1);
  }
  
  // vsync
  if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && !key_pressed[GLFW_KEY_8])
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    glfwSwapInterval(0);
    key_pressed[GLFW_KEY_8] = true;
  }
  else if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && key_pressed[GLFW_KEY_8])
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    glfwSwapInterval(1);
    key_pressed[GLFW_KEY_8] = false;
  }
}

Window::~Window()
{
  glfwTerminate();
}
