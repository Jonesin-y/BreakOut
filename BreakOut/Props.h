#pragma once
#include"GameObject.h"
enum PropType
{
	SPEEDUP,
	PASS_THROUGH,
	PAD_SIZE_INCREASE,
	CONFUSE,
	CHAOS
};
class Prop : public GameObject
{
public:
	bool Destroyed;
	bool Actived;
	float Duration;
	PropType Type;
	Prop(const Ref(Texture2D)& texture, glm::vec2 position,glm::vec3 color,float duration, PropType type);
};

