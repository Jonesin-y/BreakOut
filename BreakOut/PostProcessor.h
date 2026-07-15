#pragma once
#include<memory>
#include"Core.h"
#include<Glad/glad.h>
class Shader;
class FrameBuffer;
class RenderBuffer;
class VertexArray;
class Texture2D;
class PostProcessor
{
public:
	float ShakeTime;
	float ConfuseTime;
	float ChaosTime;
	GLboolean Shake;
	GLboolean Chaos;
	GLboolean Confuse;
	PostProcessor(const Ref(Shader)& shader, int width, int height);
	~PostProcessor();
	void BeginRender();
	void EndRender();
	void Render(float time);


private:
	int m_Width, m_Height;
	Ref(Texture2D)m_Texture2D;
	Ref(Shader) m_Shader;
	Ref(FrameBuffer) m_MSFBO,m_FBO;
	Ref(VertexArray) m_VAO;
};

