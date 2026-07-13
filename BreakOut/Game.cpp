#include"pch.h"
#include "Game.h"
#include "glm/glm.hpp"
#include "AssetManager.h"
#include "GameLevel.h"
#include "Sprite2DRenderer.h"
#include <filesystem>
#include <GLFW/glfw3.h>

const glm::vec2 PLAYER_SIZE = glm::vec2(100.0f, 20.0f);
const float PLAYER_SPEED = 400.0f;
const float BALL_RADIUS = 12.5f;
float ball_speed = 300.0f;

glm::vec2 collision_direction[] =
{
	glm::vec2(0.0f,1.0f),//上
	glm::vec2(0.0f,-1.0f),//下
	glm::vec2(-1.0f,0.0f),//左
	glm::vec2(1.0f,0.0f)//右
};

Game::Game(float Width, float Height)
	:m_Width(Width),m_Height(Height),State(GAME_ACTIVE)
{

}
Game::~Game()
{
}
void Game::Init()
{
	const glm::vec2 PLAYER_DEFAULT_POS = glm::vec2(m_Width / 2 - PLAYER_SIZE.x/2, 0.0f);
	
	

	std::cout << "当前工作目录" << std::filesystem::current_path() << std::endl;
	AssetManager::LoadShader("Assets/Shader/test.vs", "Assets/Shader/test.fs","Sprite2DShader");
	AssetManager::LoadTexture2D("Assets/Texture2D/Jonesin.jpg", "Jonesin.jpg");
	AssetManager::LoadTexture2D("Assets/Texture2D/Background.jpg", "Background.jpg");
	AssetManager::LoadTexture2D("Assets/Texture2D/Normal_Brick.png", "Normal_Brick.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/Solid_Brick.png", "Solid_Brick.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/paddle.png", "paddle.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/awesomeface.png", "awesomeface.png");


	m_Player = std::make_shared<GameObject>(
		AssetManager::GetTexture2D("paddle.png"),
		PLAYER_DEFAULT_POS,
		PLAYER_SPEED,
		PLAYER_SIZE,
		0.0f,
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	m_Ball = std::make_shared<BallObject>
		(
			AssetManager::GetTexture2D("awesomeface.png"),
			glm::vec2(m_Player->Position.x + m_Player->Size.x * 0.5f - BALL_RADIUS * 0.5f, m_Player->Position.y + m_Player->Size.y),
			ball_speed,
			BALL_RADIUS
			);

	Ref(GameLevel) level_0 = std::make_shared<GameLevel>();level_0->Load("Assets/Levels/Level0.txt", m_Width, m_Height/2);
	m_Levels.push_back(level_0);
	Ref(GameLevel) level_1 = std::make_shared<GameLevel>();level_1->Load("Assets/Levels/Level1.txt", m_Width, m_Height/2);
	m_Levels.push_back(level_1);
	Ref(GameLevel) level_2 = std::make_shared<GameLevel>();level_2->Load("Assets/Levels/Level2.txt", m_Width, m_Height/2);
	m_Levels.push_back(level_2);
	Ref(GameLevel) level_3 = std::make_shared<GameLevel>();level_3->Load("Assets/Levels/Level3.txt", m_Width, m_Height/2);
	m_Levels.push_back(level_3);
	m_Level = 0;
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(0.0f, m_Width, 0.0f, m_Height, -1.0f, 1.0f);
	Ref(Shader) Sprite2DShader = AssetManager::GetShader("Sprite2DShader");
	Sprite2DShader->SetUniform1i("u_image", 0);
	Sprite2DShader->SetUniformMat4f("u_projection", 1, GL_FALSE, &projection[0][0]);
	m_Sprite2DRenderer= std::make_shared<Sprite2DRenderer>(Sprite2DShader);



}

void Game::Update(float deltaTime)
{

	if (!m_Ball->lanuch)
		m_Ball->Position = glm::vec2(m_Player->Position.x + m_Player->Size.x * 0.5f - BALL_RADIUS * 0.5f, m_Player->Position.y + m_Player->Size.y);
	else
	{
		m_Ball->Move(deltaTime, m_Width, m_Height);
		DoCollision();
	}
}

void Game::ProcessInput(float deltaTime)
{
	//std::cout << "ProcessInput!" << std::endl;
	if (State == GAME_ACTIVE)
	{
		if (Keys[GLFW_KEY_J] == true)
			m_Ball->lanuch = true;
		if (Keys[GLFW_KEY_A] == true && m_Player->Position.x >= 0.0f)
		{
			//std::cout << "A was Pressed!" << std::endl;
			if (!m_Ball->lanuch)
				m_Ball->Velocity = glm::vec2(-1.0f * ball_speed,1.0 * ball_speed);
			std::cout <<m_Ball->Velocity.x << "," << m_Ball->Velocity.y << std::endl;
			m_Player->Position.x -= deltaTime * m_Player->Speed;
		}
		else if (Keys[GLFW_KEY_D] == true && m_Player->Position.x + m_Player->Size.x <= m_Width)
		{
			if (!m_Ball->lanuch)
				m_Ball->Velocity = glm::vec2(1.0f * ball_speed, 1.0 * ball_speed);
			std::cout << m_Ball->Velocity.x << "," << m_Ball->Velocity.y << std::endl;
			m_Player->Position.x += deltaTime * m_Player->Speed;
		}
		else
		{
			if (!m_Ball->lanuch)
				m_Ball->Velocity = glm::vec2(0.0f, 1.0 * ball_speed);
		}
	}
}

void Game::Render()
{
	if (State == GAME_ACTIVE)
	{
		m_Sprite2DRenderer->DrawSprite2D(AssetManager::GetTexture2D("Background.jpg"),
			glm::vec2(0.0f), glm::vec2(m_Width, m_Height),
			0.0f, glm::vec4(1.0f));
		if (!m_Levels[m_Level]->IsCompleted())
			m_Levels[m_Level]->Draw(m_Sprite2DRenderer);
		else
			std::cout << "关卡已完成！" << std::endl;
		m_Player->DrawObject(m_Sprite2DRenderer);
		m_Ball->DrawObject(m_Sprite2DRenderer);
	}
}

std::tuple<CollisionDir,glm::vec2> Game::IsCollision(const Ref(BallObject)& ball, const Ref(GameObject)& brick)
{
	glm::vec2 closestPoint;
	glm::vec2 ballCenter = glm::vec2(ball->Position.x+ball->Radius, ball->Position.y + ball->Radius);
	closestPoint.x = std::clamp(ballCenter.x, brick->Position.x, brick->Position.x + brick->Size.x);
	closestPoint.y = std::clamp(ballCenter.y, brick->Position.y, brick->Position.y + brick->Size.y);
	glm::vec2 diff = ballCenter-closestPoint;
	float distance = glm::distance(ballCenter, closestPoint);
	float max = 0.0f;
	int bestDir = -1;
	if (distance <= ball->Radius)
	{
		for (int i = 0;i < 4;++i)
		{
			if (glm::dot(glm::normalize(diff), collision_direction[i]) > max)
			{
				max = glm::dot(diff, collision_direction[i]);
				bestDir = i;
			}
		}
	}
	return std::tuple<CollisionDir, glm::vec2>((CollisionDir)bestDir, closestPoint);
}

void Game::DoCollision()
{
	if (State == GAME_ACTIVE)
	{
		for (auto& brick : m_Levels[m_Level]->GetBricks())
		{
			if (!brick->IsDestroyed)
			{
				CollisionDir result = std::get<0>(IsCollision(m_Ball,brick));
				glm::vec2 closestPoint = std::get<1>(IsCollision(m_Ball, brick));
				if (result!= NONE)
				{
					if(!brick->IsSolid)
						brick->IsDestroyed = true;
					switch (result)
					{
					case UP: 
					{
						m_Ball->Position = glm::vec2(closestPoint.x - m_Ball->Radius, closestPoint.y);
						m_Ball->Velocity.y = -m_Ball->Velocity.y;
						break;
					}
					case DOWN: 
					{
						m_Ball->Position = glm::vec2(closestPoint.x - m_Ball->Radius, closestPoint.y - m_Ball->Radius * 2);
						m_Ball->Velocity.y = -m_Ball->Velocity.y;
						break;
					}
					case LEFT: 
					{
						m_Ball->Position = glm::vec2(closestPoint.x - m_Ball->Radius * 2, closestPoint.y - m_Ball->Radius);
						m_Ball->Velocity.x = -m_Ball->Velocity.x;
						break;
					}
					case RIGHT: 
					{
						m_Ball->Position = glm::vec2(closestPoint.x,closestPoint.y - m_Ball->Radius);
						m_Ball->Velocity.x = -m_Ball->Velocity.x;
						break;
					}
					}
				}
			}
			
		}
		CollisionDir result = std::get<0>(IsCollision(m_Ball, m_Player));
		glm::vec2 closestPoint = std::get<1>(IsCollision(m_Ball, m_Player));
		if (result != NONE)
		{
			switch (result)
			{
			case UP:
			{
				m_Ball->Position = glm::vec2(closestPoint.x - m_Ball->Radius, closestPoint.y);
				m_Ball->Velocity.y = -m_Ball->Velocity.y;
				break;
			}
			case DOWN:
			{
				m_Ball->Position = glm::vec2(closestPoint.x - m_Ball->Radius, closestPoint.y - m_Ball->Radius * 2);
				m_Ball->Velocity.y = -m_Ball->Velocity.y;
				break;
			}
			case LEFT:
			{
				m_Ball->Position = glm::vec2(closestPoint.x - m_Ball->Radius * 2, closestPoint.y - m_Ball->Radius);
				m_Ball->Velocity.x = -m_Ball->Velocity.x;
				break;
			}
			case RIGHT:
			{
				m_Ball->Position = glm::vec2(closestPoint.x, closestPoint.y - m_Ball->Radius);
				m_Ball->Velocity.x = -m_Ball->Velocity.x;
				break;
			}
			}
		}
	}

}
