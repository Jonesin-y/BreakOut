#include "pch.h"
#include<Glad/glad.h>
#include "stb_image.h"
#include "Texture2D.h"
Texture2D::Texture2D(const std::string fileName)
{
	stbi_set_flip_vertically_on_load(1);
	stbi_info(fileName.c_str(), &m_Width, &m_Height, &m_BBP);
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	//printf("创建纹理:%s,ID=%u\n", fileName.c_str(), m_RendererID);
	m_TexData = stbi_load(fileName.c_str(), &m_Width, &m_Height, &m_BBP, 4);
	GLsizei levels = (GLsizei)std::floor(std::log2(std::max(m_Width, m_Height))) + 1;
	GLenum InternalFormat, dataFormat;
	if (m_TexData)
	{
		printf("纹理加载成功: %s (%dx%d)\n", fileName.c_str(), m_Width, m_Height);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureStorage2D(m_RendererID, levels, GL_RGBA8, m_Width, m_Height);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, m_TexData);
		glGenerateTextureMipmap(m_RendererID);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			printf("OpenGL错误(纹理上传):0x%X,纹理:%s\n", err, fileName.c_str());

	}
	else
		printf("加载2D纹理失败喵！ 文件名:", fileName);
	if (m_TexData)
		stbi_image_free(m_TexData);

}

Texture2D::~Texture2D()
{
	UnBind();
	glDeleteTextures(1,&m_RendererID);
}

void Texture2D::Bind(unsigned int slot)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		printf("glBindTextureUnit错误: 0x%X,textureID = %u,slot = %u\n", err, m_RendererID, slot);
	glBindTextureUnit(slot, m_RendererID);
}

void Texture2D::UnBind()
{
}
