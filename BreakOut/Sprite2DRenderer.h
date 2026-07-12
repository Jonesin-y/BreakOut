#pragma once
#include"Core.h"
#include<memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
class Shader;
class Texture2D;
class VertexArray;
class Sprite2DRenderer
{
public:
	Sprite2DRenderer(const Ref(Shader)& SpriteShader);
	~Sprite2DRenderer();
	void Init();
	void DrawSprite2D(const Ref(Texture2D)& texture,glm::vec2 position,glm::vec2 size,float rotation,glm::vec4 color);
private:
	Ref(Shader) m_Shader;
	Ref(VertexArray) m_VAO;
};

