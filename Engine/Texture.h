#pragma once
#include "Util.h"
#include <string>

class Texture
{
public:
	Texture(const char* texturePath);
	~Texture(void);

	void SetTexture();

private:
	// Texture path
	std::string _filePath;

	// Texture ID
	GLuint _textureID;
	// Texture type
	GLuint _textureType;
	// Texture mag-filter
	GLuint _textureMagFilter;
	// Texture min-filter
	GLuint _textureMinFilter;
	// Texture wrap
	GLuint _textureWrap;

	// Texture manager
	// *******************

	// Load DDS texture
	int loadDDS(const char* texturePath);
};

