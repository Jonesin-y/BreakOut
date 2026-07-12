#include"pch.h"
#include "Game.h"
#include "glm/glm.hpp"
#include "AssetManager.h"
#include "Sprite2DRenderer.h"
Game::Game(float Width, float Height)
	:m_Width(Width),m_Height(Height)
{

}
Game::~Game()
{
}
void Game::Init()
{
	AssetManager::LoadShader("Assets/Shader/test.vs", "Assets/Shader/test.fs","Sprite2DShader");
	AssetManager::LoadTexture2D("Assets/Texture2D/Jonesin.jpg", "Jonesin.jpg");
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(0.0f, m_Width, 0.0f, m_Height, -1.0f, 1.0f);
	Ref(Shader) Sprite2DShader = AssetManager::GetShader("Sprite2DShader");
	Sprite2DShader->SetUniform1i("u_image", 0);
	Sprite2DShader->SetUniformMat4f("u_projection", 1, GL_FALSE, &projection[0][0]);
	m_Sprite2DRenderer= std::make_shared<Sprite2DRenderer>(Sprite2DShader);



}

void Game::Update(float deltaTime)
{

}

void Game::ProcessInput(float deltaTime)
{
}

void Game::Render()
{
	int width = AssetManager::GetTexture2D("Jonesin.jpg")->GetWidth();
	int height = AssetManager::GetTexture2D("Jonesin.jpg")->GetHeight();
	m_Sprite2DRenderer->DrawSprite2D(
		AssetManager::GetTexture2D("Jonesin.jpg"), 
		glm::vec2(200.0f, 200.0f), 
		glm::vec2((float)width, (float)height), 45.0f,
		glm::vec4(1.0f,1.0f,1.0f, 1.0f));
}
