#include "hello_triangle.h"
#include "engine.h"

int main(int argc, char** argv)
{
  gpr5300::HelloTriangle scene;
  gpr5300::Engine engine(&scene);
  engine.Run();

  return EXIT_SUCCESS;
}
