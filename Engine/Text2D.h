#pragma once
#include <vector>
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Util.h"
#include "Shader.h"
#include "Texture.h"

class Text2D
{
public:
	// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-11-2d-text/
	static GLuint LoadTGAFile(const char * imagepath);
	static void InitText2D(const char * texturePath);
	static void PrintText2D(const char * text, int x, int y, int size);

private:
	static GLuint Text2DTextureID;
	static GLuint Text2DVertexBufferID;
	static GLuint Text2DUVBufferID;
	static GLuint Text2DShaderID;
	static GLuint Text2DUniformID;
	static Shader* textShader;
};

