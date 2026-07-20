#pragma once
#include "stb_truetype.h"
#include "glm/glm.hpp"
#include <memory>
#include<string>

#include "Core.h"
class VertexArray;
class Texture2D;
class Shader;
class TextRenderer
{
public:
	TextRenderer(const Ref(Shader)& shader);
	void Draw(const std::string& text,float screenX,float screenY,float scale,glm::vec3 color);
private:
	stbtt_bakedchar m_cdata[96];
	Ref(Texture2D) m_Texture2D;
	Ref(Shader)m_TextShader;
};

