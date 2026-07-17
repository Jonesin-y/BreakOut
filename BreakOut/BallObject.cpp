#include "pch.h"
#include "BallObject.h"

BallObject::BallObject(const Ref(Texture2D)& sprite, glm::vec2 pos, float speed, float radius)
	:GameObject(sprite, pos, speed, glm::vec2(radius * 2, radius * 2), glm::vec3(1.0f, 1.0f, 1.0f)), lanuchDir(glm::vec2(0.0f)), Radius(radius), lanuch(false),PassThrough(false)
{
}

void BallObject::Move(float deltaTime,int Width,int Height)
{
	//std::cout <<Velocity.x << "," <<Velocity.y << std::endl;
	Position += Velocity * deltaTime;
	if(lanuch)
	{
		//水平方向上的判定
		if (Position.x <= 0.0f)
		{
			Velocity.x = -Velocity.x;
			Position.x = 0.0f;
		}
		if (Position.x + Radius * 2 >= Width)
		{
			Velocity.x = -Velocity.x;
			Position.x = Width - Radius * 2;
		}
		//竖直方向上的判定
		if (Position.y <= 0.0f)
		{
			lanuch = false;
		}
		if (Position.y + Radius * 2 >= Height)
		{
			Velocity.y = -Velocity.y;
			Position.y = Height - Radius*2;
		}
	}
	else
	{
		return;
	}

}

void BallObject::ReSet(glm::vec2 position, float velocity)
{
}

