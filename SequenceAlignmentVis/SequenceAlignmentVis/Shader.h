#ifndef SHADER_INCLUDED_H
#define SHADER_INCLUDED_H

#include <string>
#include "glm/glm.hpp"

class Shader
{
public:
	Shader(const std::string& fileName);
	void Bind();
	void setLightDirection(glm::vec3 direction) { lightDir = direction; }
	void Update(glm::mat4 MVP, glm::mat4 Normal);
	void setColor(glm::vec3 color,glm::vec3 id);

	virtual ~Shader();
protected:
private:
	static const unsigned int NUM_SHADERS = 2;
	static const unsigned int NUM_UNIFORMS = 6;
	void operator=(const Shader& shader) {}
	Shader(const Shader& shader) {}
	

	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(unsigned int shader, unsigned int flag, bool isProgram, const std::string& errorMessage);
	unsigned int CreateShader(const std::string& text, unsigned int type);

	glm::vec3 lightDir;
	unsigned int m_program;
	unsigned int m_shaders[NUM_SHADERS];
	unsigned int m_uniforms[NUM_UNIFORMS];
};

#endif
