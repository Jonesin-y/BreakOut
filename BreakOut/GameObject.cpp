#include "pch.h"
#include "Texture2D.h"
#include "Sprite2DRenderer.h"
#include "GameObject.h"

GameObject::GameObject():
	Sprite(),Position(glm::vec2(0.0f)),Size(glm::vec2(0.0f)),Rotation(0.0f),Color(glm::vec3(0.0f)),IsSolid(false),IsDestroyed(false)
{

}

GameObject::GameObject(const Ref(Texture2D)& sprite, glm::vec2 position,float speed,glm::vec2 size, float rotation, glm::vec3 color)
	:Sprite(sprite),Position(position),Speed(speed), Size(size), Rotation(rotation), Color(color), IsSolid(false), IsDestroyed(false)
{
}

GameObject::GameObject(const Ref(Texture2D)& sprite, glm::vec2 position, glm::vec2 velocity, float speed, glm::vec2 size, glm::vec3 color)
	:Sprite(sprite), Position(position), Velocity(velocity),Speed(speed), Size(size), Rotation(0.0f), Color(color), IsSolid(false), IsDestroyed(false)
{
}

GameObject::GameObject(const Ref(Texture2D)& sprite, glm::vec2 position, float speed, glm::vec2 size, glm::vec3 color)
	:Sprite(sprite), Position(position), Speed(speed), Size(size), Rotation(0.0f), Color(color), IsSolid(false), IsDestroyed(false)
{
}

GameObject::GameObject(const Ref(Texture2D)& sprite, glm::vec2 position, glm::vec2 size, glm::vec3 color)
	:Sprite(sprite),Position(position), Speed(0.0f),Size(size),Rotation(0.0f),Color(color),IsSolid(false),IsDestroyed(false)
{
}

GameObject::GameObject(const Ref(Texture2D)& sprite, glm::vec2 position, glm::vec3 color)
	:Sprite(sprite),Position(position),Color(color)
{
}

void GameObject::DrawObject(const Ref(Sprite2DRenderer)& renderer)
{
	if (Sprite)
		renderer->DrawSprite2D(Sprite, Position, Size, Rotation, glm::vec4(Color, 1.0f));
	else
		std::cout << "纹理不存在喵！" << std::endl;
}
