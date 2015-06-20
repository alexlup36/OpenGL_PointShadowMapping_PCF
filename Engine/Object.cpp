#include "Object.h"

// Constructors
Object::Object(const std::string& name)
	: name(name), 
	_color(1.0f, 1.0f, 0.0f), 
	_translation(0.0f, 0.0f, 0.0f),
	_scale(1.0f, 1.0f, 1.0f),
	_rot(0.0f, 1.0f, 0.0f),
	_childrenList(NULL),
	_angle(0.0f), 
	_parentObject(NULL)
{
	_draw = true;
	_animate = true;
}

Object::Object(Object& object, const std::string& name)
	: name(name), 
	_color(object.Scale()), 
	_translation(object.Translation()),
	_scale(object.Scale()),
	_rot(object.Rotation()),
	_childrenList(NULL),
	_angle(0.0f),
	_parentObject(NULL)
{
	// Copy the meshes from the old object to the new one
	for (unsigned int i = 0; i < object._meshList.size(); i++)
	{
		Mesh* tempMesh = new Mesh(*object._meshList[i]);
		_meshList.push_back(tempMesh);
	}

	_draw = true;
	_animate = true;
}


Object::~Object(void)
{
	// Delete the elements from the mesh list
	while (!_meshList.empty())
	{
		delete _meshList.back();
		_meshList.pop_back();
	}

	// Delete the elements from the child list
	while (!_childrenList.empty())
	{
		delete _childrenList.back();
		_childrenList.pop_back();
	}
}

bool Object::LoadObj(const char* path)
{
	std::cout<<"Loading obj: "<<path<<std::endl;

	std::vector<glm::vec3> outVertices;
	std::vector<glm::vec3> outNormals;
	std::vector<glm::vec2> outUVs;

	// Load the values into the buffers
	if (!ParseObj(path, outVertices, outNormals, outUVs))
	{
		std::cout<<"Failed to parse the .obj model "<<path<<std::endl;
		return false;
	}
	/*// Debug
	// Vertices
	std::cout<<"Vertices...................\n\n"<<std::endl;
	printVector3(outVertices);
	// Texture coordinates
	std::cout<<"UVs........................\n\n"<<std::endl;
	printVector2(outUVs);
	// Normals
	std::cout<<"Normals.....................\n\n"<<std::endl;
	printVector3(outNormals);*/

	// Index the contents and build the vertex and index list
	indexVBO(outVertices, outUVs, outNormals);

	std::cout<<"Load obj: "<<path<<" successfully"<<std::endl;

	return true;
}

void Object::LoadIntoVB()
{
	std::cout<<"Load data into the buffers: "<<std::endl;

	_meshListIterator = _meshList.begin();

	while (_meshListIterator != _meshList.end())
	{
		(*_meshListIterator)->LoadIntoVB();

		_meshListIterator++;
	}
}

// Get the model matrix
glm::mat4 Object::GetModel()
{
	// Update the model matrix
	//_modelMatrix = glm::mat4(1.0f);

	// Scale the object
	_modelMatrix = glm::scale(_modelMatrix, _scale);

	// Rotate the object 
	_modelMatrix = glm::rotate(_modelMatrix, _angle, _rot);

	// Translate the object
	_modelMatrix = glm::translate(_modelMatrix, _translation);

	// debug
	//printMat4(_modelMatrix);
	//std::cout<<std::endl;

	// Return the model matrix
	return _modelMatrix;
}


// Properties
// Setters
void Object::SetColor(GLfloat x, GLfloat y, GLfloat z)
{
	_color = glm::vec3(x, y, z);
}

void Object::SetTranslate(GLfloat x, GLfloat y, GLfloat z)
{
	_translation += glm::vec3(x, y, z);
}

void Object::SetRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle)
{
	
	_rot.x = x;
	_rot.y = y;
	_rot.z = z;
	std::cout<<"aici"<<std::endl;
	_angle += angle;

}

void Object::SetScale(GLfloat x, GLfloat y, GLfloat z)
{
	_scale = glm::vec3(x, y, z);
}

void Object::Translate(GLfloat x, GLfloat y, GLfloat z)
{
	_modelMatrix = glm::translate(_modelMatrix, glm::vec3(x, y, z));
}

void Object::Rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle)
{
	_modelMatrix = glm::rotate(_modelMatrix, angle, glm::vec3(x, y, z));
}

void Object::Scale(GLfloat x, GLfloat y, GLfloat z)
{
	_modelMatrix = glm::scale(_modelMatrix, glm::vec3(x, y, z));
}

// Getters
glm::vec3 Object::Color()
{
	return _color;
}
	
glm::vec3 Object::Translation()
{
	return _translation;
}
	
glm::vec3 Object::Rotation()
{
	return _rot;
}
	
glm::vec3 Object::Scale()
{
	return _scale;
}

void Object::Display()
{
	// Activate the shader
	//this->_shader->Enable();
	//glUseProgram(_shader->ProgramID());

	// Iterate through the mesh list and display each mesh
	if (IsDrawing())
	{
		_meshListIterator = _meshList.begin();
		while (_meshListIterator != _meshList.end())
		{
			(*_meshListIterator)->Display();
			_meshListIterator++;
		}
	}

	// Iterate through the children list and display each child
	_childrenListIterator = _childrenList.begin();
	while (_childrenListIterator != _childrenList.end())
	{
		(*_childrenListIterator)->Display();
		_childrenListIterator++;
	}
}

void Object::AddChild(Object* child)
{
	_childrenList.push_back(child);
}

void Object::SetParent(Object* parent)
{
	_parentObject = parent;
}

std::vector<Mesh*> Object::GetMeshList()
{
	return _meshList;
}

bool Object::ParseObj(const char* path, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec3> tempNormals;
	std::vector<glm::vec2> tempUVs;

	FILE* file;
	fopen_s(&file, path, "r");
	if (!file)
	{
		std::cout<<"Unable to open file: "<<path<<std::endl;
		return false;
	}

	while (1)
	{
		char line[256];

		int res = fscanf_s(file, "%s", line);
		if (res == EOF)
		{
			break;
		}
		else
		{
			// Parse the file
			if (strcmp(line, "v") == 0)
			{
				// Vertex
				glm::vec3 tempVertex;
				fscanf_s(file, "%f%f%f\n", &tempVertex.x, &tempVertex.y, &tempVertex.z);
				tempVertices.push_back(tempVertex);
			}
			else if (strcmp(line, "vt") == 0)
			{
				// UVs
				glm::vec2 tempUV;
				//tempUV.y = -tempUV.y;
				fscanf_s(file, "%f%f\n", &tempUV.x, &tempUV.y);

				tempUVs.push_back(tempUV);
			}
			else if (strcmp(line, "vn") == 0)
			{
				// Normals
				glm::vec3 tempNormal;
				fscanf_s(file, "%f%f%f\n", &tempNormal.x, &tempNormal.y, &tempNormal.z);
				tempNormals.push_back(tempNormal);
			}
			else if (strcmp(line, "f") == 0)
			{
				// Faces
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int resCount = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				if (resCount != 9)
				{
					std::cout<<"Parse errror\n";
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);

				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);

				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		vertices.push_back(tempVertices.at(vertexIndex - 1));
		normals.push_back(tempNormals.at(normalIndex - 1));
		uvs.push_back(tempUVs.at(uvIndex - 1));
	}

	return true;
}

bool Object::getSimilarVertexIndex_fast(PackedVertex& packed, std::map<PackedVertex, unsigned short>& VertexToOutIndex, unsigned short& result)
{
	std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);

	if ( it == VertexToOutIndex.end() )
	{
		return false;
	}
	else
	{
		result = it->second;
		return true;
	}
}

void Object::indexVBO(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_uvs, std::vector<glm::vec3>& in_normals)
{
	Mesh* current = new Mesh();

	std::map<PackedVertex,unsigned short> VertexToOutIndex;

	// For each input vertex
	for ( unsigned int i=0; i<in_vertices.size(); i++ )
	{
		PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};
		
		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

		if ( found )
		{   // A similar vertex is already in the VBO, use it instead !
			current->AddIndex(index);
		}
		else
		{   // If not, it needs to be added in the output data.
			current->AddVertexPosition(in_vertices[i]);
			current->AddVertexNormal(in_normals[i]);
			current->AddVertexTexCoord(in_uvs[i]);

			unsigned short newindex = (unsigned short)current->VertexPositionListSize() - 1;
			current->AddIndex(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}

	/*// Debug
	// Vertices
	std::cout<<"Vertices...................\n\n"<<std::endl;
	printVector3(current->_vertexPositionList);
	// Texture coordinates
	std::cout<<"UVs........................\n\n"<<std::endl;
	printVector2(current->_vertexTexCoordList);
	// Normals
	std::cout<<"Normals.....................\n\n"<<std::endl;
	printVector3(current->_vertexNormalList);
	// Indices
	std::cout<<"Indices.....................\n\n"<<std::endl;
	//printVector3(current->_indexList);*/

	// If there is a parent object defined, the new created mesh is added to the parent's list of meshes
	if (_parentObject == NULL)
	{
		_meshList.push_back(current);
	}
	else
	{
		//_parentObject->GetMeshList().push_back(current);
	}
}

// Set the drawing state
void Object::SetDraw(bool drawState)
{
	_draw = drawState;
}

// Set the animation state
void Object::SetAnimate(bool animateState)
{
	_animate = animateState;
}

// Get the drawing state
bool Object::IsDrawing()
{
	return _draw;
}
	
// Get the animation state
bool Object::IsAnimating()
{
	return _animate;
}

// Reset the transformation
void Object::ResetModel()
{
	_rot = glm::vec3(0.0f, 0.0f, 0.0f);

	_translation = glm::vec3(0.0f, 0.0f, 0.0f);

	_scale = glm::vec3(0.0f, 0.0f, 0.0f);
}

