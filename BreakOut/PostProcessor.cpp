#include "pch.h"
#include "PostProcessor.h"
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include<Glad/glad.h>
PostProcessor::PostProcessor(const Ref(Shader)& shader, int width, int height)
	:m_Width(width),m_Height(height),Shake(GL_FALSE),Chaos(GL_FALSE),Confuse(GL_FALSE),ShakeTime(0.0f)
{
	//初始化m_FBO和m_MSFBO;
	m_Texture2D=std::make_shared<Texture2D>(width, height);
	Ref(RenderBuffer)RBO = std::make_shared<RenderBuffer>(width, height,8);
	m_MSFBO = std::make_shared<FrameBuffer>();
	m_FBO = std::make_shared<FrameBuffer>();
	m_MSFBO->Bind(GL_FRAMEBUFFER);
	m_MSFBO->AddRendererBuffer(RBO->GetBufferID());
	m_MSFBO->UnBind();
	m_FBO->Bind(GL_FRAMEBUFFER);
	m_FBO->AddTexture2D(m_Texture2D->GetID());
	m_FBO->UnBind();
	//向顶点缓冲区，索引缓冲区传入数据，m_VAO记录缓冲区布局
	m_VAO = std::make_shared<VertexArray>();
	float vertices[] =
	{
		-1.0f,-1.0f,0.0f,0.0f,
		1.0f,-1.0f,1.0f,0.0f,
		-1.0f,1.0f,0.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f
	};
	int indices[] =
	{
		0,1,2,
		1,2,3
	};
	Ref(VertexBuffer) VBO = std::make_shared<VertexBuffer>(vertices, (2 + 2) * 4 * sizeof(float));
	Ref(IndexBuffer) IBO = std::make_shared<IndexBuffer>(indices, 6);
	BufferLayout layout =
	{
		{Float2,"Position"},
		{Float2,"TexCoord"}
	};
	VBO->SetBufferLayout(layout);
	m_VAO->AddVertexBuffer(VBO);
	m_VAO->SetIndexBuffer(IBO);

	m_Shader = std::make_shared<Shader>("Assets/Shader/scene_effect.vs","Assets/Shader/scene_effect.fs");
	m_Shader->SetUniform1i("u_scene", 0);
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] =
	{
		{0.0f,0.0f},
		{offset,0.0f},
		{0.0f,offset},
		{offset,offset},
		{-offset,0.0f},
		{0.0f,-offset},
		{-offset,-offset},
		{offset,-offset},
		{-offset,offset}
	};
	m_Shader->SetUniform2fv("u_offsets", 9, (GLfloat*)offsets);
	GLint edge_kernel[9] =
	{
		-1,-1,-1,
		-1, 8,-1,
		-1,-1,-1
	};
	m_Shader->SetUniform1iv("u_edge_kernel", 9, edge_kernel);
	GLfloat blur_kernel[9] =
	{
		1.0f / 16.0f,2.0f / 16.0f,1.0f / 16.0f,
		2.0f / 16.0f,4.0f / 16.0f,2.0f / 16.0f,
		1.0f / 16.0f,2.0f / 16.0f,2.0f / 16.0f
	};
	m_Shader->SetUniform1fv("u_blur_kernel", 9, blur_kernel);
}

PostProcessor::~PostProcessor()
{
}

void PostProcessor::BeginRender()
{
	m_MSFBO->Bind(GL_FRAMEBUFFER);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
	m_FBO->Bind(GL_DRAW_FRAMEBUFFER);
	m_MSFBO->Bind(GL_READ_FRAMEBUFFER);
	glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	m_MSFBO->UnBind();
	m_FBO->UnBind();

}

void PostProcessor::Render(float time)
{
	m_VAO->Bind();
	m_Shader->Bind();
	m_FBO->UnBind();
	m_Texture2D->Bind(0);
	m_Shader->SetUniform1f("u_time", time);
	m_Shader->SetUniform1i("u_shake", Shake);
	m_Shader->SetUniform1i("u_chaos", Chaos);
	m_Shader->SetUniform1i("u_confuse", Confuse);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	m_VAO->UnBind();
}

