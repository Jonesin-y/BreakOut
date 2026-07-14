#include "pch.h"
#include "ParticleGenerator.h"
#include "Shader.h"
#include "Texture2D.h"
#include "VertexArray.h"
#include "Buffer.h"
Particle::Particle(float life, glm::vec2 position, glm::vec2 velocity, glm::vec4 color)
	:Life(life),Position(position),Velocity(velocity),Color(color)
{
}

ParticleGenerator::ParticleGenerator(const Ref(Texture2D)& texture, const Ref(Shader)& shader, unsigned int count)
	:m_Texture2D(texture), m_Shader(shader), m_Count(count), m_lastUnusedParticle(0)
{
	Init();
}

void ParticleGenerator::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_Shader->Bind();
	m_Texture2D->Bind(1);
	m_VAO->Bind();
	for (int i = 0;i < m_Count;++i)
	{
		if (m_Particles[i]->Life > 0)
		{
			//printf("%d\n", i);
			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(m_Particles[i]->Position, 0.0f));
			m_Shader->SetUniformMat4f("u_model", 1, GL_FALSE, &model[0][0]);
			m_Shader->SetUniform4f("u_color", m_Particles[i]->Color.x, m_Particles[i]->Color.y, m_Particles[i]->Color.z, m_Particles[i]->Color.w);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
	}
	m_VAO->UnBind();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

}

void ParticleGenerator::UpdateParticles(float deltaTime,glm::vec2 pos,unsigned int newParticles,float offset)
{
	for (int i = 0;i < newParticles;++i)
	{
		unsigned int lastOne = LastUnusedParticle();
		RespawnParticle(lastOne, pos, offset);
	}
	for (int i = 0;i < m_Count;++i)
	{
		if (m_Particles[i]->Life > 0.0f)
		{
			m_Particles[i]->Position += deltaTime * m_Particles[i]->Velocity;
			m_Particles[i]->Color.w -= deltaTime;
			m_Particles[i]->Life -= 2*deltaTime;
		}
	}
}

bool ParticleGenerator::IsEmpty()
{
	for (int i = 0;i < m_Count;++i)
	{
		if (m_Particles[i]->Life > 0)
			return false;
	}
	return true;
}

void ParticleGenerator::Init()
{
	m_VAO = std::make_shared<VertexArray>();
	float vertices[] =
	{
		0.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,0.0f,
		1.0f,1.0f,1.0f,1.0f
	};
	unsigned int indices[] =
	{
		0,1,2,
		1,2,3
	};
	Ref(VertexBuffer) VBO = std::make_shared<VertexBuffer>(vertices, (2 + 2) * 4 * sizeof(float));
	Ref(IndexBuffer)  IBO = std::make_shared<IndexBuffer>(indices, 2 * 3);
	BufferLayout layout = {
		{Float2,"Position"},
		{Float2,"TexCoord"}
	};
	VBO->SetBufferLayout(layout);
	m_VAO->AddVertexBuffer(VBO);
	m_VAO->SetIndexBuffer(IBO);
	for (int i = 0;i < m_Count;++i)
		m_Particles.push_back(std::make_shared<Particle>());
}

unsigned int ParticleGenerator::LastUnusedParticle()
{
	for (int i = m_lastUnusedParticle;i < m_Count;++i)
	{
		if (m_Particles[i]->Life <= 0.0f)
		{
			m_lastUnusedParticle = i;
			return i;
		}
	}
	for (int i = 0;i < m_lastUnusedParticle;++i)
	{
		if (m_Particles[i]->Life <= 0.0f)
		{
			m_lastUnusedParticle = i;
			return i;
		}
	}


	m_lastUnusedParticle = 0;
	return 0;
}

void ParticleGenerator::RespawnParticle(unsigned int ParticleIndex,glm::vec2 pos, float offset)
{
	float rColor = 0.5 + (rand() % 100) / 100.0f;
	float velocityX = ((rand() % 100) - 50) / 25.0f;
	float velocityY = ((rand() % 100) - 50) / 25.0f;
	float spawnX = (((rand() % 100) - 50) / 50.0f)*offset;
	float spawnY = (((rand() % 100) - 50) / 50.0f) * offset;
	m_Particles[ParticleIndex]->Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	m_Particles[ParticleIndex]->Life = 0.5f;
	m_Particles[ParticleIndex]->Velocity = glm::vec2(velocityX,velocityY);
	m_Particles[ParticleIndex]->Position = pos + glm::vec2(spawnX,spawnY);
}
