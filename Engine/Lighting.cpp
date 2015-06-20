#include "Lighting.h"


Lighting::Lighting(GLuint shaderProgram)
{
	// Initialize the locations
	_MVPLocation			= glGetUniformLocation(shaderProgram, "MVP");
	_WorldLocation			= glGetUniformLocation(shaderProgram, "World");
	_SamplerLocation		= glGetUniformLocation(shaderProgram, "Sampler");

	_EyeWorldPosLocation		= glGetUniformLocation(shaderProgram, "eyePosition");
	_SpecularIntensityLocation	= glGetUniformLocation(shaderProgram, "specularIntensity");
	_SpecularPowerLocation		= glGetUniformLocation(shaderProgram, "specularPower");

	// Get the location for the directional light
	dirLightLocation.Color				= glGetUniformLocation(shaderProgram, "directionalLight.Color");
	dirLightLocation.AmbientIntensity	= glGetUniformLocation(shaderProgram, "directionalLight.AmbientIntensity");
	dirLightLocation.Direction			= glGetUniformLocation(shaderProgram, "directionalLight.Direction");
	dirLightLocation.DiffuseIntensity	= glGetUniformLocation(shaderProgram, "directionalLight.DiffuseIntensity");

	_modeLocation = glGetUniformLocation(shaderProgram, "mode");
	
	// Get the locations for the point lights
	_NumPointLightsLocation = glGetUniformLocation(shaderProgram, "NumPointLights");

	for (unsigned int i = 0; i < 20; i++)
	{
		// Convert the index to string
		std::string index = static_cast<std::ostringstream*>( &(std::ostringstream() << i) )->str();

		// Build the corresponding string and initialize the location
		std::string temp = std::string("PointLights[") + index + "].Position";
		//std::cout<<temp<<std::endl;
		pointLightsLocation[i].Position				= glGetUniformLocation(shaderProgram, temp.c_str());

		temp = std::string("PointLights[") + index + "].Color";
		//std::cout<<temp<<std::endl;
		pointLightsLocation[i].Color				= glGetUniformLocation(shaderProgram, temp.c_str());

		temp = std::string("PointLights[") + index + "].AmbientIntensity";
		//std::cout<<temp<<std::endl;
		pointLightsLocation[i].AmbientIntensity		= glGetUniformLocation(shaderProgram, temp.c_str());

		temp = std::string("PointLights[") + index + "].DiffuseIntensity";
		//std::cout<<temp<<std::endl;
		pointLightsLocation[i].DiffuseIntensity		= glGetUniformLocation(shaderProgram, temp.c_str());


		temp = std::string("PointLights[") + index + "].Constant";
		//std::cout<<temp<<std::endl;
		pointLightsLocation[i].Atten.Constant	= glGetUniformLocation(shaderProgram, temp.c_str());

		temp = std::string("PointLights[") + index + "].Linear";
		//std::cout<<temp<<std::endl;
		pointLightsLocation[i].Atten.Exp		= glGetUniformLocation(shaderProgram, temp.c_str());

	    temp = std::string("PointLights[") + index + "].Exp";
		//std::cout<<temp<<std::endl;
		pointLightsLocation[i].Atten.Linear		= glGetUniformLocation(shaderProgram, temp.c_str());

		// Check if the location is initialized successefully
		if (pointLightsLocation[i].Color == GL_INVALID_VALUE ||
            pointLightsLocation[i].AmbientIntensity == GL_INVALID_VALUE ||
            pointLightsLocation[i].Position == GL_INVALID_VALUE ||
            pointLightsLocation[i].DiffuseIntensity == GL_INVALID_VALUE ||
            pointLightsLocation[i].Atten.Constant == GL_INVALID_VALUE ||
            pointLightsLocation[i].Atten.Linear == GL_INVALID_VALUE ||
            pointLightsLocation[i].Atten.Exp == GL_INVALID_VALUE) {
            std::cout<<"Failed to init locations.";
        }
	}
}


Lighting::~Lighting(void)
{
}

void Lighting::UpdateLight(float dt)
{
	//glUniform3f(pointLightsLocation[0].Position, Lights[i].Position.x, Lights[i].Position.y, Lights[i].Position.z);
	//glUniform3f(pointLightsLocation[1].Position, Lights[i].Position.x, Lights[i].Position.y, Lights[i].Position.z);
	//glUniform3f(pointLightsLocation[2].Position, Lights[i].Position.x, Lights[i].Position.y, Lights[i].Position.z);
}

void Lighting::SetMVP(const glm::mat4& MVP)
{
	glUniformMatrix4fv(_MVPLocation, 1, GL_FALSE, &MVP[0][0]);    
}

void Lighting::SetWorld(const glm::mat4& world)
{
	glUniformMatrix4fv(_WorldLocation, 1, GL_FALSE, &world[0][0]);
}
    
void Lighting::SetTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(_SamplerLocation, TextureUnit);
}
    
void Lighting::SetDirectionalLight(const DirectionalLight& Light)
{
	// Set the light properties
	glUniform3f(dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	glUniform3f(dirLightLocation.Direction, Light.Direction.x, Light.Direction.y, Light.Direction.z);
	glUniform1f(dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void Lighting::SetPointLights(std::vector<PointLight>& Lights)
{
	// Set the number of point lights
	glUniform1i(_NumPointLightsLocation, Lights.size());

	// Set the properties for each light
	for (unsigned int i = 0; i < Lights.size(); i++)
	{
		// Set the light properties
		glUniform3f(pointLightsLocation[i].Color, Lights[i].Color.x, Lights[i].Color.y, Lights[i].Color.z);
		glUniform3f(pointLightsLocation[i].Position, Lights[i].Position.x, Lights[i].Position.y, Lights[i].Position.z);
		glUniform1f(pointLightsLocation[i].DiffuseIntensity, Lights[i].DiffuseIntensity);

		// Set the light attenuation properties
		glUniform1f(pointLightsLocation[i].Atten.Constant, Lights[i].Attenuation.Constant);
		glUniform1f(pointLightsLocation[i].Atten.Linear, Lights[i].Attenuation.Linear);
		glUniform1f(pointLightsLocation[i].Atten.Exp, Lights[i].Attenuation.Exp);
	}
}

void Lighting::SetLightDirection(const glm::vec3& LightDirection)
{
	glUniform3f(dirLightLocation.Direction, LightDirection.x, LightDirection.y, LightDirection.z);
}

void Lighting::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
{
	glUniform3f(_EyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}
    
void Lighting::SetSpecularIntensity(float Intensity)
{
	glUniform1f(_SpecularIntensityLocation, Intensity);
}
    
void Lighting::SetSpecularPower(float Power)
{
	glUniform1f(_SpecularPowerLocation, Power);
}

void Lighting::SetMode(int mode)
{
	glUniform1i(_modeLocation, mode);
}
