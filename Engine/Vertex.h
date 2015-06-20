#pragma once

#include "Util.h"

class Vertex
{
public:
	// Constructors
	Vertex(void);
	Vertex(glm::vec3& position, glm::vec3& normal);
	Vertex(glm::vec3& position, glm::vec3& normal, glm::vec2& texCoord);
	
	// Virtual destructor
	virtual ~Vertex(void);

	// Properties
	glm::vec3 GetPosition();
	glm::vec2 GetTexCoord();
	glm::vec3 GetNormal();

	void SetPosition(glm::vec3& newPosition);
	void SetTexCoord(glm::vec2& newTexCoord);
	void SetNormal(glm::vec3& newNormal);

	// Test method
	void Test();

private:
	glm::vec3 _vertexPosition;
	glm::vec3 _vertexNormal;
	glm::vec2 _vertexTexCoord;

	void PrintVertex();
};

