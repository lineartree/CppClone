#include "gui.h"
#include "imgui/imgui.h"

Gui::Gui()
{
}

void Gui::init(GLFWwindow* window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // setup platform/renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  // setup imgui style
  ImGui::StyleColorsDark();
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("res/fonts/Noto Mono Nerd Font Complete Mono.ttf", 14);
}

void Gui::render(Camera camera, float lastX, float lastY)
{
  
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  // render ImGui
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
  ImGui::Begin("Debug");
  ImGui::Text("View");

  std::vector<glm::vec3*> vec;
  vec.push_back(&camera.Position);
  float* vecs[3] = {&vec[0]->x, &vec[0]->y, &vec[0]->z};
  // ImGui::DragFloat3("cameraPos", &vecs);
  ImGui::DragFloat3("cameraPos", *vecs, -10.0f, 10.0f);
  ImGui::DragFloat("xpos", &lastX);
  ImGui::DragFloat("ypos", &lastY);

  ImGui::Text("Projection");
  ImGui::SliderFloat("FOV", &camera.Zoom, 0.0f, 120.0f);
  ImGui::Text("Help");
  if (ImGui::CollapsingHeader("Keybinds"))
  {
    ImGui::Text("w forward");
    ImGui::Text("s backward");
    ImGui::Text("a left");
    ImGui::Text("d right");
    ImGui::Text("CTRL sprint");
    ImGui::Text("F11 fullscreen");
    ImGui::Text("ESC show/disable cursor");
    ImGui::Text("8 enable/disable vsync");
    ImGui::Text("9 world choice 1");
    ImGui::Text("10 world choice 2");
  };

  ImGui::End();
  ImGui::PopStyleVar();

  // render ImGui into screen
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  ImGui::EndFrame();
}

Gui::~Gui()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
