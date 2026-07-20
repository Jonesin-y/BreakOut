#include "pch.h"
#include "TextRenderer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "stb_truetype.h"
TextRenderer::TextRenderer(const Ref(Shader)& shader)
	:m_TextShader(shader)
{



	FILE* file = fopen("Assets/Font/OCRAEXT.TTF", "rb");
	if (!file)
	{
		std::cout << "字体文件加载失败: Assets/Font/OCRAEXT.TTF" << std::endl;
		return;
	}
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char* ttf_buffer = new unsigned char[size];
	fread(ttf_buffer, 1, size, file);
	fclose(file);
	unsigned char* temp_bitmap = new unsigned char[1024*1024];
	stbtt_BakeFontBitmap(ttf_buffer,0,64.0f,temp_bitmap,1024,1024,32,96,m_cdata);
	delete[]ttf_buffer;

	m_Texture2D = std::make_shared<Texture2D>(1024,1024,temp_bitmap);
	delete[]temp_bitmap;
}

void TextRenderer::Draw(const std::string& text, float screenX, float screenY, float scale, glm::vec3 color)
{
	m_TextShader->Bind();
	m_Texture2D->Bind(0);
	
	m_TextShader->SetUniform3fv("u_TextColor", 1, &color[0]);

	float currentX = 0.0f;
	float currentY = 0.0f;
	for (char c : text)
	{
		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(m_cdata,1024,1024,c-32,&currentX,&currentY,&q,1);
		float true_y0 = - q.y0;
		float true_y1 = - q.y1;
		//翻转纹理坐标的y轴，令其纹理坐标原点和OpenGL纹理坐标原点对齐
		float final_x0 = screenX + q.x0 * scale;
		float final_x1 = screenX + q.x1 * scale;
		float final_y0 = screenY + true_y0 * scale;
		float final_y1 = screenY + true_y1 * scale;
		
		float vertices[] =
		{
			final_x0,final_y0,q.s0,q.t0,
			final_x0,final_y1,q.s0,q.t1,
			final_x1,final_y0,q.s1,q.t0,
			final_x1,final_y1,q.s1,q.t1
		};
		unsigned int indices[] =
		{
			0,1,2,
			1,2,3
		};
		VertexArray VAO;
		VAO.Bind();
		Ref(VertexBuffer) VBO = std::make_shared<VertexBuffer>(vertices, (2 + 2) * 4 * sizeof(float));
		Ref(IndexBuffer)  IBO = std::make_shared<IndexBuffer>(indices, 6);
		BufferLayout layout =
		{
			{Float2,"Position"},
			{Float2,"TexCoord"}
		};
		VBO->SetBufferLayout(layout);
		VAO.AddVertexBuffer(VBO);
		VAO.SetIndexBuffer(IBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		VAO.UnBind();
	}


}

