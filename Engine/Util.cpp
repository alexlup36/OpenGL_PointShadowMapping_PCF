#include "Util.h"



void printVec2(glm::vec2& v)
{
	std::cout<<"X = "<<v.x<<"; "<<"Y = "<<v.y<<"; "<<std::endl;
}

void printVec3(glm::vec3& v)
{
	std::cout<<"X = "<<v.x<<"; "<<"Y = "<<v.y<<"; "<<"Z = "<<v.z<<";"<<std::endl;
}

void printMat4(glm::mat4& m)
{
	std::cout<<m[0][0]<<" "<<m[0][1]<<" "<<m[0][2]<<" "<<m[0][3]<<std::endl;
	std::cout<<m[1][0]<<" "<<m[1][1]<<" "<<m[1][2]<<" "<<m[1][3]<<std::endl;
	std::cout<<m[2][0]<<" "<<m[2][1]<<" "<<m[2][2]<<" "<<m[2][3]<<std::endl;
	std::cout<<m[3][0]<<" "<<m[3][1]<<" "<<m[3][2]<<" "<<m[3][3]<<std::endl;
}

void printVector2(std::vector<glm::vec2>& vec)
{
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		printVec2(vec[i]);
	}
}

void printVector3(std::vector<glm::vec3>& vec)
{
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		printVec3(vec[i]);
	}
}