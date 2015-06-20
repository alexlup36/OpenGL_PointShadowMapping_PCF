#include "Vertex.h"

//*******************
//*****Public********
//*******************

// Constructors
Vertex::Vertex(void)
	: _vertexPosition(glm::vec3(0.0f)), _vertexNormal(glm::vec3(0.0f)), _vertexTexCoord(glm::vec2(0.0f)) {}

Vertex::Vertex(glm::vec3& position, glm::vec3& normal)
	: _vertexPosition(position), _vertexNormal(normal), _vertexTexCoord(glm::vec2(0.0f)) {}
	
Vertex::Vertex(glm::vec3& position, glm::vec3& normal, glm::vec2& texCoord)
	: _vertexPosition(position), _vertexNormal(normal), _vertexTexCoord(texCoord) {}

// Destructor
Vertex::~Vertex(void)
{
}

// Properties
// Getters
glm::vec3 Vertex::GetPosition()
{
	return this->_vertexPosition;
}
	
glm::vec2 Vertex::GetTexCoord()
{
	return this->_vertexTexCoord;
}
	
glm::vec3 Vertex::GetNormal()
{
	return this->_vertexNormal;
}

// Setters
void Vertex::SetPosition(glm::vec3& newPosition)
{
	this->_vertexPosition = newPosition;
}
	
void Vertex::SetTexCoord(glm::vec2& newTexCoord)
{
	this->_vertexTexCoord = newTexCoord;
}
	
void Vertex::SetNormal(glm::vec3& newNormal)
{
	this->_vertexNormal = newNormal;
}

// Test method
void Vertex::Test()
{
	// 0 vertex
	Vertex nullVertex;
	//nullVertex.PrintVertex();

	// custom vertex
	Vertex v1 = Vertex(
		glm::vec3(1.0f, 2.0f, 3.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec2(0.0f, 2.0f));
	//v1.PrintVertex();

	// copy vertex
	Vertex* v2 = new Vertex(v1);
	//v2->PrintVertex();

	v2->SetPosition(glm::vec3(6.0f, 6.0f, 6.0f));
	v2->SetTexCoord(glm::vec2(1.0f, 1.0f));

	//if (nullVertex.GetNormal().x == nullVertex.GetNormal().y == nullVertex.GetNormal().z == 0.0f) std::cout<<"T1 OK"<<std::endl;
	//if (v1.GetPosition().y == 2.0f && v1.GetTexCoord().y == 2.0f && v1.GetNormal().y == 1.0f) std::cout<<"T2 OK"<<std::endl;
	//if (v2->GetPosition().x == 6.0f && v2->GetPosition().y == 6.0f && v2->GetPosition().z == 6.0f && v2->GetTexCoord().x == 1.0f) std::cout<<"T3 OK"<<std::endl;
}

//*******************
//*****Private*******
//*******************

void Vertex::PrintVertex()
{
	std::cout<<"Vertex position: ";
	printVec3(this->_vertexPosition);

	std::cout<<"Vertex normal: ";
	printVec3(this->_vertexNormal);

	std::cout<<"Vertex texture coordinate: ";
	printVec2(this->_vertexTexCoord);
}