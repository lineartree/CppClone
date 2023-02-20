#include "gfx/shader.h"
#include "gfx/window.h"
#include "imgui/imgui.h"
#include "util/camera.h"
#include "world/world.h"


int main()
{
  Window window;
  World world;
  world.world_choice(0);
  Shader ourShader("res/shaders/minecraft.vert", "res/shaders/minecraft.frag");
  Fps fps;

  ourShader.use();

  // loop
  while (!glfwWindowShouldClose(window.handle))
  {
    fps.show_fps(window.handle);

    // per-frame time logic
    float currentFrame = static_cast<float>(glfwGetTime());
    Window::deltaTime = currentFrame - Window::lastFrame;
    Window::lastFrame = currentFrame;

    // input
    window.processInput(window.handle, world);


    // render
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // active shader
    ourShader.use();

    // pass projection matrix to shader
    float aspect_ratio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    glm::mat4 projection = glm::perspective( glm::radians(Window::camera.Zoom), aspect_ratio, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = Window::camera.GetViewMatrix();
    ourShader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    ourShader.setMat4("model", model);
    
    // bind textures
    glActiveTexture(GL_TEXTURE0);
    world.texture_manager.bind();
    ourShader.setInt("texture_sampler", 0);
    
    
    world.draw();
    glFinish();

    
    // render imgui
    window.imgui.render(Window::camera, Window::lastX, Window::lastY);

    // glfw
    glfwSwapBuffers(window.handle);
    glfwPollEvents();
  }

  glDeleteProgram(ourShader.ID);
  glfwTerminate();
  return 0;
}
