#include "pch.h"
#include "Props.h"
#include "Texture2D.h"

Prop::Prop(const Ref(Texture2D)& texture, glm::vec2 position, glm::vec3 color, float duration, PropType type)
	:GameObject(texture, position, color), Actived(false),Destroyed(false), Duration(duration), Type(type)
{
	Velocity = glm::vec2(0.0f, -300.0f);
	Size = glm::vec2(100.0f, 20.0f);
}
