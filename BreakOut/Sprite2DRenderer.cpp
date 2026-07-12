#include "pch.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include "Shader.h"
#include "Sprite2DRenderer.h"
#include <Glad/glad.h>
Sprite2DRenderer::Sprite2DRenderer(const Ref(Shader)& SpriteShader)
{
	m_VAO = std::make_shared<VertexArray>();
	m_Shader = SpriteShader;
	Init();
}

Sprite2DRenderer::~Sprite2DRenderer()
{
}

void Sprite2DRenderer::Init()
{
	float vertices[] =
	{
		//位置     纹理坐标
		0.0f,0.0f, 0.0f,0.0f,
		1.0f,0.0f, 1.0f,0.0f,
		0.0f,1.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,1.0f
	};
	int indices[] =
	{
		0,1,2,
		1,2,3
	};
	Ref(VertexBuffer) VBO = std::make_shared<VertexBuffer>(vertices,(2 + 2)*4*sizeof(float));
	Ref(IndexBuffer) IBO  = std::make_shared<IndexBuffer>(indices, 6);
	BufferLayout layout =
	{
		{Float2,"Position"},
		{Float2,"TexCoord"}
	};
	VBO->SetBufferLayout(layout);
	m_VAO->AddVertexBuffer(VBO);
	m_VAO->SetIndexBuffer(IBO);
}

void Sprite2DRenderer::DrawSprite2D(const Ref(Texture2D)& texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color)
{
	m_Shader->Bind();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5*size.x,-0.5*size.y,0.0f));
	model = glm::scale(model, glm::vec3(size.x, size.y, 0.0f));
	m_Shader->SetUniformMat4f("u_model", 1, GL_FALSE, &model[0][0]);
	m_Shader->SetUniform4f("u_spriteColor", color.x, color.y, color.z, color.w);
	texture->Bind(0);
	m_VAO->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	m_VAO->UnBind();

}

