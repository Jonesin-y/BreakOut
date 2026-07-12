#pragma once
#include<string>
class Texture2D
{
public:
	Texture2D(const std::string fileName);
	~Texture2D();
	void Bind(unsigned int slot);
	void UnBind();
	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
private:
	int m_Width;
	int m_Height;
	int m_BBP;
	GLuint m_RendererID;
	unsigned char* m_TexData;
};

