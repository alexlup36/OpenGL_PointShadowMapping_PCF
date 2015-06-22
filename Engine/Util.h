#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#define PI 3.14159265

// Initialize screen dimension
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

void printVec2(glm::vec2& v);
void printVec3(glm::vec3& v);

void printMat4(glm::mat4& m);

void printVector2(std::vector<glm::vec2>& vec);
void printVector3(std::vector<glm::vec3>& vec);