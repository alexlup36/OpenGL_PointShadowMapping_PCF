#pragma once
#include "Util.h"
#include "Shader.h"
#include <sstream>

struct BaseLight
{
	glm::vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;

	BaseLight()
    {
		Color = glm::vec3(0.0f, 0.0f, 0.0f);
        AmbientIntensity = 0.0f;
        DiffuseIntensity = 0.0f;
    }
};

struct PointLight : public BaseLight
{
    glm::vec3 Position;
	glm::mat4* TransformationMatrix;

    struct
    {
        float Constant;
        float Linear;
        float Exp;
    } Attenuation;

	PointLight()
    {
        Position = glm::vec3(0.0f, 0.0f, 0.0f);
        Attenuation.Constant = 1.0f;
        Attenuation.Linear = 0.0f;
        Attenuation.Exp = 0.0f;
    }

	PointLight(
		const glm::vec3 position, 
		const glm::vec3 color, 
		const float cnst, 
		const float lin, 
		const float exp, 
		const float ambInt, 
		const float difInt)
	{
		Color = color;
		Position = position;

		AmbientIntensity = ambInt;
		DiffuseIntensity = difInt;

		Attenuation.Constant = cnst;
		Attenuation.Linear = lin;
		Attenuation.Exp = exp;
	}
};

struct DirectionalLight : public BaseLight
{
	glm::vec3 Direction;

	DirectionalLight()
    {
        Direction = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};


class Lighting
{
public:
	Lighting(GLuint shaderProgram);
	~Lighting(void);

	void UpdateLight(float dt);

	void SetMVP(const glm::mat4& MVP);
	void SetWorld(const glm::mat4& world);
    void SetTextureUnit(unsigned int TextureUnit);
    void SetDirectionalLight(const DirectionalLight& Light);
	void SetPointLights(std::vector<PointLight>& Lights);
	void SetLightDirection(const glm::vec3& LightDirection);
	void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
    void SetSpecularIntensity(float Intensity);
    void SetSpecularPower(float Power);

private:
    GLuint _MVPLocation;
	GLuint _WorldLocation;
    GLuint _SamplerLocation;
	GLuint _EyeWorldPosLocation;
    GLuint _SpecularIntensityLocation;
    GLuint _SpecularPowerLocation;
	GLuint _NumPointLightsLocation;
	GLuint _modeLocation;

	struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Direction;
    } dirLightLocation;

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Position;
        struct
        {
            GLuint Constant;
            GLuint Linear;
            GLuint Exp;
        } Atten;
    } pointLightsLocation[50];
	// Max number of point lights
};

