#include "hello_triangle.h"
#include "file_utility.h"
#include "texture_loader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace gpr5300
{
void HelloTriangle::Begin()
{
  TextureManager texture_manager;
  texture_ = texture_manager.CreateTexture("data/textures/container.jpg");

  LoadShaders();
  CreateBuffers();
}

void HelloTriangle::ProcessInput()
{
  SDL_PumpEvents();
  const Uint8* state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_W])
    cameraPos_ += cameraSpeed_ * cameraFront_ * dt_;
  if (state[SDL_SCANCODE_S])
    cameraPos_ -= cameraSpeed_ * cameraFront_ * dt_;
  if (state[SDL_SCANCODE_A])
    cameraPos_ -= glm::normalize(glm::cross(cameraFront_, cameraUp_)) * cameraSpeed_ * dt_;
  if (state[SDL_SCANCODE_D])
    cameraPos_ += glm::normalize(glm::cross(cameraFront_, cameraUp_)) * cameraSpeed_ * dt_;

}

glm::mat4 HelloTriangle::UpdateCamera(glm::mat4 view) {

  ProcessInput();

  if (firstMouse_)
  {
    lMouseX_ = mouseX_;
    lMouseY_ = mouseY_;
    firstMouse_ = false;
  }
  lMouseX_ = mouseX_;
  lMouseY_ = mouseY_;
  // Get the current mouse position and button state
  int mouseState = SDL_GetMouseState(&mouseX_, &mouseY_);

  // Check if the left mouse button is pressed
  if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {

    // Calculate the offset only when the mouse is clicked
    float xoffset = mouseX_ - lMouseX_;
    float yoffset = lMouseY_ - mouseY_; // reversed since y-coordinates range from bottom to top

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw_   += xoffset;
    pitch_ -= yoffset;

    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;



    // Update the camera direction
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    direction.y = sin(glm::radians(pitch_));
    direction.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

    std::cout << "Yaw: " << yaw_ << ", Pitch: " << pitch_ << std::endl;
    std::cout << "Direction: " << direction.x << ", " << direction.y << ", " << direction.z << "\n\n\n";

    if (glm::length(direction) > 0.0f) {
      cameraFront_ = glm::normalize(direction);
    }
  }

  return glm::lookAt(cameraPos_, cameraPos_ + cameraFront_, cameraUp_);
}

void HelloTriangle::LoadShaders()
{
  // Load and compile vertex shader
  const auto vertexContent = LoadFile("data/shaders/hello_triangle/triangle.vert");
  const auto* ptr = vertexContent.data();
  vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader_, 1, &ptr, nullptr);
  glCompileShader(vertexShader_);

  GLint success;
  glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    std::cerr << "Error while compiling vertex shader\n";
  }

  // Load and compile fragment shader
  const auto fragmentContent = LoadFile("data/shaders/hello_triangle/triangle.frag");
  ptr = fragmentContent.data();
  fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader_, 1, &ptr, nullptr);
  glCompileShader(fragmentShader_);

  glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    std::cerr << "Error while compiling fragment shader\n";
  }

  // Link shaders into a program
  program_ = glCreateProgram();
  glAttachShader(program_, vertexShader_);
  glAttachShader(program_, fragmentShader_);
  glLinkProgram(program_);

  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success)
  {
    std::cerr << "Error while linking shader program\n";
  }
}

void HelloTriangle::CreateBuffers()
{
  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };


  unsigned int indices[] = {
      0, 1, 2, 0, 2, 3
  };

  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);



}

void HelloTriangle::End()
{
  glDeleteProgram(program_);
  glDeleteShader(vertexShader_);
  glDeleteShader(fragmentShader_);
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &ebo_);
}

void HelloTriangle::Update(float dt)
{

  elapsedTime_ += dt;
  dt_ = dt;



  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 model = glm::rotate(glm::mat4(1.0f), elapsedTime_, glm::vec3(0.5f, 1.0f, 0.0f));
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);


  view = UpdateCamera(view);

  glUseProgram(program_);
  glUniformMatrix4fv(glGetUniformLocation(program_, "model"), 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(glGetUniformLocation(program_, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(program_, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  glBindTexture(GL_TEXTURE_2D, texture_);
  glBindVertexArray(vao_);

  glBindVertexArray(vao_);
  for(unsigned int i = 0; i < 10; i++)
  {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = elapsedTime_ * i;
    model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));
    unsigned int modelLoc = glGetUniformLocation(program_, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

}

}