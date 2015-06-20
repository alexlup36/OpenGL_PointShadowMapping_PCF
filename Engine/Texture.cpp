#include "Texture.h"


Texture::Texture(const char* texturePath)
	: _filePath(texturePath) 
{
	// Load the texture
	loadDDS(texturePath);

	if (!_textureID)
	{
		fprintf(stderr, "Failed to load the texture %s", texturePath);
	}

	// Init texture properties
	_textureType		= GL_TEXTURE_2D;
	_textureMagFilter	= GL_LINEAR;
	_textureMinFilter	= GL_LINEAR_MIPMAP_LINEAR;
	_textureWrap		= GL_CLAMP_TO_BORDER;	

	// Add texture to texture manager
	// ******************
}


Texture::~Texture(void)
{
	glDeleteTextures(1, &_textureID);
}

void Texture::SetTexture()
{
	glTexParameteri(_textureType, GL_TEXTURE_MAG_FILTER, _textureMagFilter);
	glTexParameteri(_textureType, GL_TEXTURE_MIN_FILTER, _textureMinFilter);
	//glTexParameteri(_textureType, GL_TEXTURE_WRAP_S, _textureWrap);
}

int Texture::loadDDS(const char* texturePath)
{
	unsigned char header[124];

	FILE *fp; 
 
	/* try to open the file */ 
	fopen_s(&fp, texturePath, "rb"); 
	if (fp == NULL) 
	{
		return 0; 
	}
   
	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) 
	{ 
		fclose(fp); 
		return 0; 
	}
	
	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);

 
	unsigned char* buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	/* close the file pointer */ 
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch(fourCC) 
	{ 
	case FOURCC_DXT1: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
		break; 
	case FOURCC_DXT3: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
		break; 
	case FOURCC_DXT5: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		break; 
	default: 
		free(buffer); 
		return 0; 
	}

	// Create one OpenGL texture
	glGenTextures(1, &_textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(_textureType, _textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 
	} 

	free(buffer); 
	return 1;
}