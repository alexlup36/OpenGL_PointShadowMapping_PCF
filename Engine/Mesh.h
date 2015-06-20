#pragma once
#include "Util.h"
#include "Vertex.h"
#include <vector>
#include <map>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

struct PackedVertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;

	bool operator<(const PackedVertex that) const
	{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	};
};

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	// Load the mesh contents into the vertex buffer
	void LoadIntoVB();

	void Display();

	// Add data to lists
	void AddVertexPosition(glm::vec3& position);
	void AddVertexTexCoord(glm::vec2& position);
	void AddVertexNormal(glm::vec3& position);

	void AddIndex(unsigned short newIndex);

	// Get vert position list size
	unsigned int VertexPositionListSize();

	std::vector<glm::vec3> _vertexPositionList;
	std::vector<glm::vec2> _vertexTexCoordList;
	std::vector<glm::vec3> _vertexNormalList;

	std::vector<unsigned short> _indexList;
private:
	// Vertex buffer id
	GLuint _vertexBuffer;
	// Texture coordinate buffer id
	GLuint _uvBuffer;
	// Normal buffer id
	GLuint _normalBuffer;
	// Index buffer id
	GLuint _elementBuffer;

	// Drawing mode
	GLuint _mode;
	GLuint _usage;
};