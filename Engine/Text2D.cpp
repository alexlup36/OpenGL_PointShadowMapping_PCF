#include "Text2D.h"
#include "SOIL2.h"

GLuint Text2D::Text2DTextureID = 0;
GLuint Text2D::Text2DVertexBufferID = 0;
GLuint Text2D::Text2DUVBufferID = 0;
GLuint Text2D::Text2DShaderID = 0;
GLuint Text2D::Text2DUniformID = 0;
Shader* Text2D::textShader = NULL;


GLuint Text2D::LoadTGAFile(const char* imagepath)
{
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	int uiWidth, uiHeight, uiComponents;

	// Get the texture data from the file

	// Using soil
	unsigned char* sImageData = SOIL_load_image(imagepath, &uiWidth, &uiHeight, &uiComponents, 4);

	if (sImageData == NULL)
	{
		std::cout << "Texture loading failed " << imagepath << std::endl;
	}
	else
	{
		std::cout << "Texture data loaded successfully " << imagepath << std::endl;
	}

	// Nice tri-linear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	
	// Set texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, uiWidth, uiHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sImageData);

	// Enable mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Return the ID of the texture we just created
	return textureID;
}


void Text2D::InitText2D(const char* texturePath)
{
	textShader = new Shader("..\\Shaders\\TextVertexShader.txt", "..\\Shaders\\TextFragmentShader.txt");
	textShader->LoadShaders();

	textShader->Enable();

	// Initialize texture
	Text2DTextureID = LoadTGAFile(texturePath);

	// Initialize VBO
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	Text2DShaderID = textShader->ProgramID();

	// Initialize uniforms' IDs
	Text2DUniformID = glGetUniformLocation(Text2DShaderID, "myTextureSampler");

	textShader->Disable();
}


void Text2D::PrintText2D(const char * text, int x, int y, int size)
{
	unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;

	for (unsigned int i = 0 ; i < length ; i++)
	{	
		glm::vec2 vertex_up_left    = glm::vec2(x + i * size, y + size);
		glm::vec2 vertex_up_right   = glm::vec2(x + i * size + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + i * size + size, y);
		glm::vec2 vertex_down_left  = glm::vec2(x + i * size, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , 1.0f - uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, 1.0f - uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, 1.0f - (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , 1.0f - (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}

	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	textShader->Enable();

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(Text2DUniformID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	textShader->Disable();
}