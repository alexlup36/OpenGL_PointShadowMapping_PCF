#include "Shader.h"


Shader::Shader(const char* vertex_file_path, const char* fragment_file_path)
{
	strcpy_s(_vertexShaderPath, vertex_file_path);
	strcpy_s(_fragmentShaderPath, fragment_file_path);

	// Create the shaders
	_VertexShaderID			= glCreateShader(GL_VERTEX_SHADER);
	_FragmentShaderID		= glCreateShader(GL_FRAGMENT_SHADER);
}


Shader::~Shader(void)
{
	glDetachShader(_ShaderProgramID, _VertexShaderID);
	glDetachShader(_ShaderProgramID, _FragmentShaderID);

	glDeleteProgram(_ShaderProgramID);

	glDeleteShader(_VertexShaderID);
	glDeleteShader(_FragmentShaderID);
}

void Shader::Enable()
{
	glUseProgram(_ShaderProgramID);
}

void Shader::Disable()
{
	glUseProgram(0);
}

// Set the program ID
void Shader::SetProgramID(GLuint newProgramID)
{
	this->_ShaderProgramID = newProgramID;
}

// Get the program ID
GLuint Shader::ProgramID()
{
	return _ShaderProgramID;
}

int Shader::ReadShaderFromFile(std::string& shaderCode, const char* shaderPath)
{
	std::ifstream VertexShaderStream(shaderPath, std::ios::in);

	if (VertexShaderStream.is_open())
	{
		std::string Line = "";

		while (getline(VertexShaderStream, Line))
		{
			shaderCode += "\n" + Line;
		}

		VertexShaderStream.close();
	} 
	else
	{
		std::cout<<"Failed to open %s."<<shaderPath<<std::endl;
		return 0;
	}

	return 1;
}

void Shader::CompileShader(GLuint shaderID, std::string& shaderCode, const char* shaderPath)
{
	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Shader
	std::cout<<"Compiling shader :"<<shaderPath<<"."<<std::endl;
	char const* SourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &SourcePointer , NULL);
	glCompileShader(shaderID);

	// Check Shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		//std::cout<<ShaderErrorMessage[0]<<std::endl;
		printf("%s\n", &ShaderErrorMessage[0]);
	}
}

void Shader::CreateProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
{
	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Link the program
	std::cout<<"Linking program"<<std::endl;
	_ShaderProgramID = glCreateProgram();
	glAttachShader(_ShaderProgramID, vertexShaderID);
	glAttachShader(_ShaderProgramID, fragmentShaderID);
	glLinkProgram(_ShaderProgramID);

	// Check the program
	glGetProgramiv(_ShaderProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(_ShaderProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(_ShaderProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
}

GLuint Shader::LoadShaders()
{
	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	if (!ReadShaderFromFile(VertexShaderCode, _vertexShaderPath))
	{
		std::cout<<"Failed to read vertex shader."<<std::endl;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	if (!ReadShaderFromFile(FragmentShaderCode, _fragmentShaderPath))
	{
		std::cout<<"Failed to read fragment shader."<<std::endl;
	}

	// Compile Vertex Shader
	CompileShader(_VertexShaderID, VertexShaderCode, _vertexShaderPath);

	// Compile Fragment Shader
	CompileShader(_FragmentShaderID, FragmentShaderCode, _fragmentShaderPath);

	// Link the program
	CreateProgram(_VertexShaderID, _FragmentShaderID);

	return _ShaderProgramID;
}

GLuint Shader::GetUniformLocation(const char* uniformVarName)
{
	GLint location = glGetUniformLocation(_ShaderProgramID, uniformVarName);

    if (location == 0xFFFFFFFF)
    {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", uniformVarName);
    }

    return location;
}
