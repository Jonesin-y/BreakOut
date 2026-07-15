#pragma once
#include<Glad/glad.h>
#include<string>

class Shader
{
public:
	Shader(const std::string& vs, const std::string& fs,const std::string&gs);
	Shader(const std::string& vs, const std::string& fs);
	std::string LoadFromFile(const std::string& fileName);
	GLuint CompileShader(unsigned int type,const std::string& shader_src);
	GLuint CreateShader(const std::string& vs_src, const std::string& fs_src,const std::string& gs_src);
	GLuint CreateShader(const std::string& vs_src, const std::string& fs_src);
	void SetUniform1i(const std::string& name, int value);

	void SetUniform1iv(const std::string& name, unsigned int count, GLint* value);

	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, float v0,float v1);
	void SetUniform3f(const std::string& name, float v0,float v1,float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void SetUniform1fv(const std::string& name, unsigned int count, GLfloat* value);
	void SetUniform2fv(const std::string& name, unsigned int count, GLfloat* value);
	void SetUniform3fv(const std::string& name, unsigned int count, GLfloat* value);

	void SetUniformMat3f(const std::string& name, unsigned int count, unsigned char transpose, const float* value);
	void SetUniformMat4f(const std::string& name, unsigned int count, unsigned char transpose, const float* value);

	void Bind();
	void UnBind();
private:
	unsigned int m_RendererID;
	std::string vs_src;
	std::string fs_src;
};
