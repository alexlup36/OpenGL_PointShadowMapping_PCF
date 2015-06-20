#pragma once

#include "Util.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

class Shader
{
public:
	Shader(const char* vertex_file_path, const char* fragment_file_path);
	~Shader(void);

	// Enable the use of the shader
	void Enable();
	void Disable();

	// Load vertex and fragment shader from file
	GLuint LoadShaders();

	// Get the uniform variable location
	GLuint GetUniformLocation(const char* uniformVarName);

	// Set the program ID
	void SetProgramID(GLuint newProgramID);
	// Return the program ID
	GLuint ProgramID();

private:
	// File paths for both the vertex shader and fragment shader
	char _vertexShaderPath[50];
	char _fragmentShaderPath[50];

	// Vertex shader and fragment shader
	GLuint _VertexShaderID;
	GLuint _FragmentShaderID;

	// Shader program
	GLuint _ShaderProgramID;

	// Read shader code from file to string
	int ReadShaderFromFile(std::string& shaderCode, const char* shaderPath);
	// Compile and check the shader
	void CompileShader(GLuint shaderID, std::string& shaderCode, const char* shaderPath);
	// Create the program
	void CreateProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
};

