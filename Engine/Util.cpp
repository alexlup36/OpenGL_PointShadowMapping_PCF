#include "Util.h"



void printVec2(glm::vec2& v)
{
	std::cout<<"X = "<<v.x<<"; "<<"Y = "<<v.y<<"; "<<std::endl;
}

void printVec3(glm::vec3& v)
{
	std::cout<<"X = "<<v.x<<"; "<<"Y = "<<v.y<<"; "<<"Z = "<<v.z<<";"<<std::endl;
}

void printMat4(glm::mat4& m)
{
	std::cout<<m[0][0]<<" "<<m[0][1]<<" "<<m[0][2]<<" "<<m[0][3]<<std::endl;
	std::cout<<m[1][0]<<" "<<m[1][1]<<" "<<m[1][2]<<" "<<m[1][3]<<std::endl;
	std::cout<<m[2][0]<<" "<<m[2][1]<<" "<<m[2][2]<<" "<<m[2][3]<<std::endl;
	std::cout<<m[3][0]<<" "<<m[3][1]<<" "<<m[3][2]<<" "<<m[3][3]<<std::endl;
}

void printVector2(std::vector<glm::vec2>& vec)
{
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		printVec2(vec[i]);
	}
}

void printVector3(std::vector<glm::vec3>& vec)
{
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		printVec3(vec[i]);
	}
}

void checkGLError(const char* file, int line)
{
	GLenum error(glGetError());

	while (error != GL_NO_ERROR)
	{
		std::string errorMessage;

		switch (error)
		{
		case GL_INVALID_OPERATION:
		{
			errorMessage = "INVALID_OPERATION";
			break;
		}
		case GL_INVALID_ENUM:
		{
			errorMessage = "INVALID_ENUM";
			break;
		}
		case GL_INVALID_VALUE:
		{
			errorMessage = "INVALID_VALUE";
			break;
		}
		case GL_INVALID_FRAMEBUFFER_OPERATION:
		{
			errorMessage = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		case GL_OUT_OF_MEMORY:
		{
			errorMessage = "OUT_OF_MEMORY";
			break;
		}
		case GL_STACK_UNDERFLOW:
		{
			errorMessage = "STACK_UNDERFLOW";
			break;
		}
		case GL_STACK_OVERFLOW:
		{
			errorMessage = "STACK_OVERFLOW";
			break;
		}
		}

		printf("GL_%s - File %s : line %d\n", errorMessage.c_str(), file, line);

		error = glGetError();
	}
}