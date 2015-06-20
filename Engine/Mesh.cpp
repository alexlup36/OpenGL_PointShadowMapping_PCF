#include "Mesh.h"


Mesh::Mesh(void)
	: _vertexBuffer(0), 
	_uvBuffer(0),
	_normalBuffer(0),
	_elementBuffer(0),
	_usage(GL_STATIC_DRAW), 
	_mode(GL_TRIANGLES) { }


Mesh::~Mesh(void)
{
	// Delete the vertex position buffer
	if (_vertexBuffer)
	{
		glDeleteBuffers(1, &_vertexBuffer);
	}
	// Delete the vertex texture coordinate buffer
	if (_uvBuffer)
	{
		glDeleteBuffers(1, &_uvBuffer);
	}
	// Delete the vertex normal buffer
	if (_normalBuffer)
	{
		glDeleteBuffers(1, &_normalBuffer);
	}

	// Delete the index buffer
	if (_elementBuffer)
	{
		glDeleteBuffers(1, &_elementBuffer);
	}
}

void Mesh::LoadIntoVB()
{
	glGetError();

	// Create vertex buffer object
	/*GLuint VertexArrayId;
	glGenBuffers(1, &VertexArrayId);
	glBindVertexArray(VertexArrayId);*/

	int i = glGetError();
	if (i != 0)
	{
		std::cout<<"Error vertex buffer object: "<<i<<std::endl;
	}

	// Load the positions into the buffer
	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, _vertexPositionList.size() * sizeof(glm::vec3), &_vertexPositionList[0], _usage);

	// Load the texture coordinates into the buffer
	glGenBuffers(1, &_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, _vertexTexCoordList.size() * sizeof(glm::vec2), &_vertexTexCoordList[0], _usage);

	// Load the normals into the buffer
	glGenBuffers(1, &_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, _vertexNormalList.size() * sizeof(glm::vec3), &_vertexNormalList[0], _usage);

	// Generate a buffer for the indices as well
	glGenBuffers(1, &_elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexList.size() * sizeof(unsigned short), &_indexList[0] , _usage);
}

void Mesh::Display()
{
	// first attribute buffer - vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// second attribute buffer - texture coordinates
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// third attribute buffer - normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
	
	// draw the triangles
	glDrawElements(_mode,
		_indexList.size(),
		GL_UNSIGNED_SHORT,
		(void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Mesh::AddVertexPosition(glm::vec3& position)
{
	_vertexPositionList.push_back(position);
}
	
void Mesh::AddVertexTexCoord(glm::vec2& texCoord)
{
	_vertexTexCoordList.push_back(texCoord);
}
	
void Mesh::AddVertexNormal(glm::vec3& normal)
{
	_vertexNormalList.push_back(normal);
}

void Mesh::AddIndex(unsigned short newIndex)
{
	_indexList.push_back(newIndex);
}

unsigned int Mesh::VertexPositionListSize()
{
	return _vertexPositionList.size();
}