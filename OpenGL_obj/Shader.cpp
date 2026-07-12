#include"pch.h"
#include<Glad/glad.h>
#include "Shader.h"

Shader::Shader(const std::string& vs, const std::string& fs,const std::string& gs)
{
	std::string vs_src = LoadFromFile(vs);
	std::string fs_src =LoadFromFile(fs);
	std::string gs_src =LoadFromFile(gs);
	m_RendererID = CreateShader(vs_src,fs_src,gs_src);
}

Shader::Shader(const std::string& vs, const std::string& fs)
{
	std::string vs_src = LoadFromFile(vs);
	std::string fs_src = LoadFromFile(fs);
	m_RendererID = CreateShader(vs_src, fs_src);
}

std::string Shader::LoadFromFile(const std::string& fileName)
{
	std::ifstream fstream(fileName);
	if (!fstream.is_open())
	{
		printf("文件打开失败喵！");
		return "";
	}
	std::stringstream sstream;
	std::string line;
	while (getline(fstream, line))
	{
		sstream << line << "\n";
	}
	return sstream.str();
}

GLuint Shader::CompileShader(unsigned int type,const std::string& shader_src)
{
	unsigned int id = glCreateShader(type);
	const char* src_c = shader_src.c_str();
	glShaderSource(id, 1, &src_c, NULL);
	glCompileShader(id);
	//Shader COMPILE Debug Message
	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length);
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Fail to Compile" << ((type == GL_VERTEX_SHADER) ? "VertexShader" : "FragmentShader")<< std::endl;
		std::cout << "Error: " << message << std::endl;
	}

	return id;
}

GLuint Shader::CreateShader(const std::string& vs_src, const std::string& fs_src,const std::string& gs_src)
{
	unsigned int id = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vs_src);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fs_src);
	GLuint gs = CompileShader(GL_GEOMETRY_SHADER, gs_src);
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glAttachShader(id, gs);
	glLinkProgram(id);
	GLint result;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length);
		glGetProgramInfoLog(id, length, &length, message);
		std::cout << "Fail to Link" << std::endl;
		std::cout << "Error: " << message << std::endl;
		return -1;
	}
	//Shader Program LINKING Debug Message

	glValidateProgram(id);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);
	return id;
}

GLuint Shader::CreateShader(const std::string& vs_src, const std::string& fs_src)
{
	unsigned int id = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vs_src);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fs_src);
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);
	GLint result;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length);
		glGetProgramInfoLog(id, length, &length, message);
		std::cout << "Fail to Link" << std::endl;
		std::cout << "Error: " << message << std::endl;
		return -1;
	}
	//Shader Program LINKING Debug Message

	glValidateProgram(id);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return id;
}

void Shader::Bind()
{
	glUseProgram(m_RendererID);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glProgramUniform1i(m_RendererID, location, value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glProgramUniform1f(m_RendererID, location, value);
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glProgramUniform2f(m_RendererID, location, v0,v1);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glProgramUniform3f(m_RendererID, location, v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glProgramUniform4f(m_RendererID, location, v0, v1, v2, v3);
}

void Shader::SetUniformMat3f(const std::string& name, unsigned int count, unsigned char transpose, const float* value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glProgramUniformMatrix3fv(m_RendererID, location, count, transpose, value);
}

void Shader::SetUniformMat4f(const std::string& name, unsigned int count, unsigned char transpose, const float* value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glProgramUniformMatrix4fv(m_RendererID, location, count, transpose, value);
}

