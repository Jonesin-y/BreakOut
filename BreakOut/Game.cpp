#include"pch.h"
#include "Game.h"
#include "glm/glm.hpp"
#include "AssetManager.h"
#include "GameLevel.h"
#include "Sprite2DRenderer.h"
#include "ParticleGenerator.h"
#include <filesystem>
#include <GLFW/glfw3.h>
const glm::vec2 PLAYER_SIZE = glm::vec2(200.0f, 40.0f);
const float PLAYER_SPEED = 400.0f;
const float BALL_RADIUS = 25.0f;
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
	ma_sound_uninit(&m_bgm);
	ma_sound_uninit(&m_brick);
	ma_sound_uninit(&m_solid_brick);
	ma_sound_uninit(&m_prop);
	ma_sound_uninit(&m_pad);

	ma_engine_uninit(&m_AudioEngine);
}
void Game::Init()
{
	const glm::vec2 PLAYER_DEFAULT_POS = glm::vec2(m_Width / 2 - PLAYER_SIZE.x/2, 0.0f);
	
	if (ma_engine_init(NULL, &m_AudioEngine) != MA_SUCCESS)
		std::cout << "miniaudio初始化失败!" << std::endl;
	ma_result bgm_result = ma_sound_init_from_file(&m_AudioEngine, "Assets/Audio/bgm.mp3", MA_SOUND_FLAG_STREAM, NULL, NULL, &m_bgm);
	ma_result brick_result = ma_sound_init_from_file(&m_AudioEngine, "Assets/Audio/brick.mp3", 0, NULL, NULL, &m_brick);
	ma_result solid_brick_result = ma_sound_init_from_file(&m_AudioEngine, "Assets/Audio/solid_brick.wav", 0, NULL, NULL, &m_solid_brick);
	ma_result pad_result = ma_sound_init_from_file(&m_AudioEngine, "Assets/Audio/pad.wav", 0, NULL, NULL, &m_pad);
	ma_result prop_result = ma_sound_init_from_file(&m_AudioEngine, "Assets/Audio/prop.wav", 0, NULL, NULL, &m_prop);

	if (bgm_result != MA_SUCCESS)
		std::cout << "bgm加载失败!" << std::endl;
	if (brick_result != MA_SUCCESS)
		std::cout << "brick加载失败!" << std::endl;
	if (solid_brick_result != MA_SUCCESS)
		std::cout << "solid_brick加载失败!" << std::endl;
	if (pad_result != MA_SUCCESS)
		std::cout << "pad加载失败!" << std::endl;
	if (prop_result != MA_SUCCESS)
		std::cout << "prop加载失败!" << std::endl;
	ma_sound_set_looping(&m_bgm, MA_TRUE);
	ma_sound_set_volume(&m_bgm, 0.5f);
	ma_sound_set_volume(&m_brick, 1.0f);
	ma_sound_set_volume(&m_solid_brick, 1.0f);
	ma_sound_set_volume(&m_prop, 1.0f);
	ma_sound_set_volume(&m_pad, 1.0f);
	ma_sound_start(&m_bgm);



	std::cout << "当前工作目录" << std::filesystem::current_path() << std::endl;
	AssetManager::LoadShader("Assets/Shader/test.vs", "Assets/Shader/test.fs","Sprite2DShader");
	AssetManager::LoadShader("Assets/Shader/particle.vs", "Assets/Shader/particle.fs", "ParticleShader");
	AssetManager::LoadShader("Assets/Shader/scene_effect.vs", "Assets/Shader/scene_effect.fs", "EffectShader");
	AssetManager::LoadTexture2D("Assets/Texture2D/Jonesin.jpg", "Jonesin.jpg");
	AssetManager::LoadTexture2D("Assets/Texture2D/Background.jpg", "Background.jpg");
	AssetManager::LoadTexture2D("Assets/Texture2D/Normal_Brick.png", "Normal_Brick.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/Solid_Brick.png", "Solid_Brick.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/paddle.png", "paddle.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/awesomeface.png", "awesomeface.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/particle.png", "particle.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/prop_chaos.png", "prop_chaos.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/prop_confuse.png", "prop_confuse.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/prop_pad_increase.png", "prop_pad_increase.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/prop_passthrough.png", "prop_passthrough.png");
	AssetManager::LoadTexture2D("Assets/Texture2D/prop_speed.png", "prop_speed.png");



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
	Ref(Shader) Particle2DShader = AssetManager::GetShader("ParticleShader");
	Ref(Shader) EffectShader = AssetManager::GetShader("EffectShader");
	Sprite2DShader->SetUniform1i("u_Sprite2DImage", 0);
	Sprite2DShader->SetUniformMat4f("u_projection", 1, GL_FALSE, &projection[0][0]);
	Particle2DShader->SetUniform1i("u_ParticleImage", 1);
	Particle2DShader->SetUniformMat4f("u_projection", 1, GL_FALSE, &projection[0][0]);
	Particle2DShader->SetUniformMat4f("u_projection", 1, GL_FALSE, &projection[0][0]);
	m_Sprite2DRenderer= std::make_shared<Sprite2DRenderer>(Sprite2DShader);
	m_ParticleGenerator = std::make_shared<ParticleGenerator>(AssetManager::GetTexture2D("particle.png"), Particle2DShader, 500);
	
	m_Effect = std::make_shared<PostProcessor>(AssetManager::GetShader("EffectShader"),m_Width,m_Height);

}

void Game::Update(float deltaTime)
{
	if (State == GAME_ACTIVE)
	{
		if (m_Effect->Shake)
			m_Effect->ShakeTime -= deltaTime;
		if (m_Effect->ShakeTime <= 0)
			m_Effect->Shake = false;
		if (!m_Ball->lanuch)
			m_Ball->Position = glm::vec2(m_Player->Position.x + m_Player->Size.x * 0.5f - BALL_RADIUS * 0.5f, m_Player->Position.y + m_Player->Size.y);
		else
		{
			m_Ball->Move(deltaTime, m_Width, m_Height);
		}
		DoCollision();
		UpdateProps(deltaTime);
		if (m_Ball->Velocity.x >= 0.05f || m_Ball->Velocity.x <= -0.05f)
			m_ParticleGenerator->UpdateParticles(deltaTime, glm::vec2(m_Ball->Position.x + m_Ball->Radius, m_Ball->Position.y + m_Ball->Radius), 1, m_Ball->Radius / 2);
		else if (!m_ParticleGenerator->IsEmpty())
			m_ParticleGenerator->UpdateParticles(deltaTime, glm::vec2(m_Ball->Position.x + m_Ball->Radius, m_Ball->Position.y + m_Ball->Radius), 0, m_Ball->Radius / 2);

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
			//std::cout <<m_Ball->Velocity.x << "," << m_Ball->Velocity.y << std::endl;
			m_Player->Position.x -= deltaTime * m_Player->Speed;
		}
		else if (Keys[GLFW_KEY_D] == true && m_Player->Position.x + m_Player->Size.x <= m_Width)
		{
			if (!m_Ball->lanuch)
				m_Ball->Velocity = glm::vec2(1.0f * ball_speed, 1.0 * ball_speed);
			//std::cout << m_Ball->Velocity.x << "," << m_Ball->Velocity.y << std::endl;
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
		m_Effect->BeginRender();
		m_Sprite2DRenderer->DrawSprite2D(AssetManager::GetTexture2D("Background.jpg"),
			glm::vec2(0.0f), glm::vec2(m_Width, m_Height),
			0.0f, glm::vec4(1.0f));
		m_Player->DrawObject(m_Sprite2DRenderer);
		m_Ball->DrawObject(m_Sprite2DRenderer);
		for (auto& prop : m_Props)
		{
			if (!prop->Destroyed)
				prop->DrawObject(m_Sprite2DRenderer);
		}
		m_ParticleGenerator->Draw();
		if (!m_Levels[m_Level]->IsCompleted())
			m_Levels[m_Level]->Draw(m_Sprite2DRenderer);
		else
			std::cout << "关卡已完成！" << std::endl;
		m_ParticleGenerator->Draw();
		m_Effect->EndRender();
		m_Effect->Render(glfwGetTime());
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

bool Game::IsCollision(const Ref(GameObject)& AABB1, const Ref(GameObject)& AABB2)
{
	if (AABB1->Position.x + AABB1->Size.x > AABB2->Position.x && AABB1->Position.x < AABB2->Position.x + AABB2->Size.x)
	{
		if (AABB1->Position.y < AABB2->Position.y + AABB2->Size.y && AABB1->Position.y + AABB1->Size.y > AABB2->Position.y)
			return true;
	}
	return false;
}

void Game::DoCollision()
{
	if (State == GAME_ACTIVE)
	{
		for (auto& brick : m_Levels[m_Level]->GetBricks())
		{
			if (!brick->IsDestroyed)
			{
				if (!m_Ball->PassThrough)
				{
					CollisionDir result = std::get<0>(IsCollision(m_Ball, brick));
					glm::vec2 closestPoint = std::get<1>(IsCollision(m_Ball, brick));
					if (result != NONE)
					{
						if (!m_Effect->Shake)
						{
							m_Effect->ShakeTime = 0.05f;
							m_Effect->Shake = true;
						}
						if (!brick->IsSolid)
						{
							if (ma_sound_is_playing(&m_brick))
							{
								ma_sound_stop(&m_brick);
								ma_sound_seek_to_pcm_frame(&m_brick, 0);
								ma_sound_start(&m_brick);
							}
							else
								ma_sound_start(&m_brick);
							brick->IsDestroyed = true;
							SpawnProp(brick->Position);
						}
						else if (brick->IsSolid)
						{
							if (ma_sound_is_playing(&m_solid_brick))
							{
								ma_sound_stop(&m_solid_brick);
								ma_sound_seek_to_pcm_frame(&m_solid_brick, 0);
								ma_sound_start(&m_solid_brick);
							}
							else
								ma_sound_start(&m_solid_brick);
						}

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
				else if (m_Ball->PassThrough)
				{
					CollisionDir result = std::get<0>(IsCollision(m_Ball, brick));
					glm::vec2 closestPoint = std::get<1>(IsCollision(m_Ball, brick));
					if (!brick->IsSolid && result !=NONE)
					{
						if (ma_sound_is_playing(&m_brick))
						{
							ma_sound_stop(&m_brick);
							ma_sound_seek_to_pcm_frame(&m_brick, 0);

							ma_sound_start(&m_brick);
						}
						else
							ma_sound_start(&m_brick);
						brick->IsDestroyed = true;
						SpawnProp(brick->Position);
					}
					else if (brick->IsSolid)
					{
						
						if (result != NONE)
						{
							if (!m_Effect->Shake)
							{
								if (ma_sound_is_playing(&m_solid_brick))
								{
									ma_sound_stop(&m_solid_brick);
									ma_sound_seek_to_pcm_frame(&m_solid_brick, 0);

									ma_sound_start(&m_solid_brick);
								}
								else
									ma_sound_start(&m_solid_brick);

								m_Effect->ShakeTime = 0.05f;
								m_Effect->Shake = true;
							}
						}
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
			
		}
		CollisionDir ball_result = std::get<0>(IsCollision(m_Ball, m_Player));
		glm::vec2 closestPoint = std::get<1>(IsCollision(m_Ball, m_Player));
		if (ball_result != NONE)
		{
			if(m_Ball->lanuch)
				ma_sound_start(&m_pad);
				switch (ball_result)
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
		for (auto& prop : m_Props)
		{
			if (IsCollision(prop, m_Player))
			{
				if (ma_sound_is_playing(&m_prop))
				{
					ma_sound_stop(&m_prop);
					ma_sound_seek_to_pcm_frame(&m_prop, 0);

					ma_sound_start(&m_prop);
				}
				else
					ma_sound_start(&m_prop);

				ActiveProp(prop);
			}
		}

	}

}
void Game::SpawnProp(glm::vec2 Position)
{
	if (ShouldSpawn(60))
	{

		Ref(Prop) prop = std::make_shared<Prop>(
			AssetManager::GetTexture2D("prop_pad_increase.png"),
			Position,glm::vec3(1.0f,0.6f,0.4f),
			10.0f,
			PAD_SIZE_INCREASE
			);
		m_Props.push_back(prop);
		std::cout << "生成Prop!类型为:" << prop->Type << std::endl;
	}
	if (ShouldSpawn(60))
	{
		Ref(Prop) prop = std::make_shared<Prop>(
			AssetManager::GetTexture2D("prop_passthrough.png"),
			Position, glm::vec3(0.5f, 1.0f, 0.5f),
			10.0f,
			PASS_THROUGH
		);
		m_Props.push_back(prop);
		std::cout << "生成Prop!类型为:" << prop->Type << std::endl;

	}
	if (ShouldSpawn(60))
	{
		Ref(Prop) prop = std::make_shared<Prop>(
			AssetManager::GetTexture2D("prop_speed.png"),
			Position, glm::vec3(0.5f, 0.5f, 0.5f),
			15.0f,
			SPEEDUP
		);
		m_Props.push_back(prop);
		std::cout << "生成Prop!类型为:" << prop->Type << std::endl;

	}
	if (ShouldSpawn(15))
	{
		Ref(Prop) prop = std::make_shared<Prop>(
			AssetManager::GetTexture2D("prop_chaos.png"),
			Position, glm::vec3(0.9f, 0.25f, 0.25f),
			15.0f,
			CHAOS
		);
		m_Props.push_back(prop);
		std::cout << "生成Prop!类型为:" << prop->Type << std::endl;

	}
	if (ShouldSpawn(15))
	{
		Ref(Prop) prop = std::make_shared<Prop>(
			AssetManager::GetTexture2D("prop_confuse.png"),
			Position, glm::vec3(1.0f, 0.3f, 0.3f),
			15.0f,
			CONFUSE
		);
		m_Props.push_back(prop);
		std::cout << "生成Prop!类型为:" << prop->Type << std::endl;

	}
}
void Game::ActiveProp(const Ref(Prop)& prop)
{
	if (prop->Actived)
		return;
	
	prop->Destroyed = true;
	

	switch (prop->Type)
	{
	case SPEEDUP:
	{
		std::cout << "SPEEDUP已激活!" << std::endl;
		if (!IsOtherPropActive(SPEEDUP))
		{
			m_Ball->Velocity *= 1.5;
			prop->Actived = true;
		}
		break;
	}
	case PAD_SIZE_INCREASE:
	{
		std::cout << "PAD_SIZE_INCREASE已激活!" << std::endl;
		if (!IsOtherPropActive(PAD_SIZE_INCREASE))
		{
			m_Player->Size.x *= 1.5;
			prop->Actived = true;
		}
		break;
	}
	case PASS_THROUGH:
	{
		std::cout << "PASS_THROUGH已激活!" << std::endl;
		if (!IsOtherPropActive(PASS_THROUGH))
		{
			m_Ball->PassThrough = true;
			prop->Actived = true;
		}
		m_Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
		break;
	}
	case CHAOS:
	{
		std::cout << "CHAOS已激活!" << std::endl;
		if (!IsOtherPropActive(CHAOS))
		{
			m_Effect->Chaos = true;
			prop->Actived = true;
		}
		break;
	}
	case CONFUSE:
	{
		std::cout << "CONFUSE已激活!" << std::endl;
		if (!IsOtherPropActive(CONFUSE))
		{
			m_Effect->Confuse = true;
			prop->Actived = true;
		}
		break;
	}
	
	}
}
bool Game::IsOtherPropActive(PropType type)
{
	for (int i = 0; i < m_Props.size();++i)
	{
		if (m_Props[i]->Type == type && m_Props[i]->Actived)
			return true;
	}
	return false;
}//用于判断是否同时激活了多个同种Prop的效果
bool Game::ShouldSpawn(unsigned int chance)
{
	unsigned int random = rand() % chance;
	return random == 0;
}//计算Prop的产生概率
void Game::UpdateProps(float deltaTime)
{
	for (auto& prop : m_Props)
	{
		prop->Position += prop->Velocity * deltaTime;
		if (prop->Position.y <= 0)
		{
			printf("Destroyed = true!\n");
			prop->Destroyed = true;
		}
		if (prop->Duration >= 0 && prop->Actived)
			prop->Duration -= deltaTime;
		else if (prop->Duration <= 0)
		{
			prop->Actived = false;

			switch (prop->Type)
			{
			case SPEEDUP:
			{
				if (!IsOtherPropActive(SPEEDUP))
				{
					m_Ball->Velocity /= 1.5;
					break;
				}
			}
			case PAD_SIZE_INCREASE:
			{
				if (!IsOtherPropActive(PAD_SIZE_INCREASE))
				{
					m_Player->Size = PLAYER_SIZE;
					break;
				}
			}
			case PASS_THROUGH:
			{
				if (!IsOtherPropActive(PASS_THROUGH))
				{
					m_Ball->PassThrough = false;
					m_Ball->Color = glm::vec3(1.0f);
					break;
				}
			}
			case CHAOS:
			{
				if (!IsOtherPropActive(CHAOS))
				{
					m_Effect->Chaos = false;
					break;
				}
			}
			case CONFUSE:
			{
				if (!IsOtherPropActive(CONFUSE))
				{
					m_Effect->Confuse = false;
					break;
				}
			}
			}
		}
	}
	m_Props.erase(std::remove_if
	(m_Props.begin(), m_Props.end(), [](const Ref(Prop)& prop) {return prop->Destroyed && !prop->Actived;})
		, m_Props.end());
	
}


