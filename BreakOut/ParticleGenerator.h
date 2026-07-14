#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Core.h"
class VertexArray;
class Shader;
class Texture2D;
struct Particle
{
	float Life;
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec4 Color;
	Particle(float life = 1.0f, glm::vec2 position = glm::vec2(0.0f), glm::vec2 velocity =glm::vec2(0.0f), glm::vec4 color=glm::vec4(1.0f));

};
class ParticleGenerator
{
public:
	ParticleGenerator(const Ref(Texture2D)&texture,const Ref(Shader)& shader, unsigned int count);
	void Draw();
	void UpdateParticles(float deltaTime,glm::vec2 pos, unsigned int newParticles, float offset);
	bool IsEmpty();
private:
	std::vector<Ref(Particle)> m_Particles;
	Ref(Texture2D) m_Texture2D;
	Ref(Shader) m_Shader;
	Ref(VertexArray) m_VAO;
	unsigned int m_lastUnusedParticle;
	unsigned int m_Count;
	unsigned int m_GenerateParticleCount;
	unsigned int m_AllParticleCount;
	void Init();
	unsigned int LastUnusedParticle();
	void RespawnParticle(unsigned int ParticleIndex, glm::vec2 pos, float offset);


};

