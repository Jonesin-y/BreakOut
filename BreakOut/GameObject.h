#pragma once
#include"glm/glm.hpp"
#include<memory>
#include "Core.h"
class Sprite2DRenderer;
class Texture2D;
class GameObject
{
public:
	Ref(Texture2D)Sprite;
	glm::vec2 Position;
	glm::vec2 Size;
	glm::vec2 Velocity;
	glm::vec3 Color;
	float Speed;
	float Rotation;
	bool IsSolid;
	bool IsDestroyed;
	GameObject();
	GameObject(const Ref(Texture2D)& sprite, glm::vec2 position, float speed, glm::vec2 size, float rotation, glm::vec3 color);
	GameObject(const Ref(Texture2D)& sprite, glm::vec2 position, glm::vec2 velocity,float speed, glm::vec2 size, glm::vec3 color);
	GameObject(const Ref(Texture2D)& sprite, glm::vec2 position, float speed,glm::vec2 size, glm::vec3 color);
	GameObject(const Ref(Texture2D)& sprite, glm::vec2 position, glm::vec2 size,glm::vec3 color);
	GameObject(const Ref(Texture2D)& sprite, glm::vec2 position, glm::vec3 color);
	void DrawObject(const Ref(Sprite2DRenderer)& renderer);
};

