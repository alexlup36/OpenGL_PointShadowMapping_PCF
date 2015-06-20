#pragma once
#include "Util.h"
//#include "Vertex.h"
#include "Shader.h"
#include "Mesh.h"
#include <vector>

class Object
{
public:
	Object(const std::string& name);
	Object(Object& object, const std::string& name);
	~Object(void);

	// ---------------------------------------------------------------------------

	inline void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	inline void operator delete(void* p)
	{
		_mm_free(p);
	}

	// ---------------------------------------------------------------------------

	// Load vertices
	bool LoadObj(const char* path);

	void LoadIntoVB();

	// Get the model matrix
	glm::mat4 GetModel();

	// Properties
	// Setters
	void SetColor(GLfloat x, GLfloat y, GLfloat z);
	void SetTranslate(GLfloat x, GLfloat y, GLfloat z);
	void SetRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle);
	void SetScale(GLfloat x, GLfloat y, GLfloat z);

	// Getters
	glm::vec3 Color();
	glm::vec3 Translation();
	glm::vec3 Rotation();
	glm::vec3 Scale();

	void Translate(GLfloat x, GLfloat y, GLfloat z);
	void Rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle);
	void Scale(GLfloat x, GLfloat y, GLfloat z);

	// Reset the transformation
	void ResetModel();

	// Display method
	void Display();

	// Add a child object
	void AddChild(Object* child);
	// Set the parent object
	void SetParent(Object* parent);

	// Get the mesh list
	std::vector<Mesh*> GetMeshList();

	// Name of the object
	std::string name;

	// Set the drawing state
	void SetDraw(bool drawState);
	// Set the animation state
	void SetAnimate(bool animateState);

	// Get the drawing state
	bool IsDrawing();
	// Get the animation state
	bool IsAnimating();

private:
	bool _draw;
	bool _animate;

	// Base color
	glm::vec3 _color;
	
	// Translation, ratation and scale vectors
	glm::vec3 _translation;
	glm::vec3 _rot;
	glm::vec3 _scale;

	// Rotation angle
	GLfloat _angle;

	// Model matrix associated with above vectors
	glm::mat4x4 _modelMatrix;

	// List of meshes which compose the object
	std::vector<Mesh*> _meshList;

	// List of children
	std::vector<Object*> _childrenList;
	// Parent reference
	Object* _parentObject;

	// Iterators
	std::vector<Mesh*>::const_iterator _meshListIterator;
	std::vector<Object*>::const_iterator _childrenListIterator;

	// Load obj file
	bool ParseObj(const char* path, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs);

	bool getSimilarVertexIndex_fast(PackedVertex& packed, std::map<PackedVertex, unsigned short>& VertexToOutIndex, unsigned short& result);

	void indexVBO(std::vector<glm::vec3> & in_vertices, std::vector<glm::vec2> & in_uvs, std::vector<glm::vec3> & in_normals);
};

