#pragma once
#include "GameObject.h"
#include "glm/glm.hpp"
#include <memory>
#include "Core.h"
class Texture2D;
class BallObject : public GameObject
{
public:
	float Radius;
	bool lanuch;
	glm::vec2 lanuchDir;
	BallObject(const Ref(Texture2D)& sprite,glm::vec2 pos,float speed, float radius);
	void Move(float deltaTime,int Width,int Height);//Width 和 Height用于计算球移动的方形空间范围
	void ReSet(glm::vec2 position,float velocity);
	

};

