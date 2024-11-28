#ifndef HELLO_TRIANGLE_H
#define HELLO_TRIANGLE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "scene.h"

namespace gpr5300
{
class HelloTriangle : public Scene
{
 public:
  void Begin() override;
  void End() override;
  void Update(float dt) override;

  HelloTriangle(){

  }

 private:
  void LoadShaders();
  void CreateBuffers();
  void ConfigureShaders();
  glm::mat4 UpdateCamera(glm::mat4 view);
  void ProcessInput();

  glm::vec3 cameraPos_ = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraUp_    = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 cameraFront_ = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraSpeed_ = glm::vec3(5, 5, 5);

  glm::vec3 direction_;
  float yaw_ = -90.0f;
  float pitch_ = 0;
  int mouseX_ = 0, mouseY_ = 0;
  float lMouseX_ = 0;
  int lMouseY_ = 0;

  GLuint vertexShader_ = 0;

  GLuint fragmentShader_ = 0;
  GLuint program_ = 0;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
  GLuint ebo_ = 0;
  unsigned int texture_ = 0;
  float dt_ = 0;

  float elapsedTime_ = 0.0f;

  bool firstMouse_ = true;
};
}

#endif // HELLO_TRIANGLE_H