#pragma once
#include"Sprite2DRenderer.h"
#include"ParticleGenerator.h"
#include"GameObject.h"
#include"BallObject.h"
#include"Props.h"
#include"PostProcessor.h"
#include<vector>
#include<memory>
#include<tuple>
#include"Core.h"
#include<miniaudio/miniaudio.h>
enum CollisionDir
{
	NONE = -1,UP,DOWN,LEFT,RIGHT
};
enum GameState
{
	GAME_ACTIVE,GAME_MENU,GAME_WIN
};
class GameLevel;
class Game
{
public:
	
	Game(float Width, float Height);
	~Game();
	GameState State;
	bool Keys[1024] = { false };
	void Init();
	void Update(float deltaTime);
	void ProcessInput(float deltaTime);
	void Render();
	
private:
	float m_Width, m_Height;
	unsigned int m_Level;
	Ref(PostProcessor)m_Effect;
	Ref(ParticleGenerator) m_ParticleGenerator;
	Ref(Sprite2DRenderer) m_Sprite2DRenderer;
	Ref(GameObject) m_Player;
	Ref(BallObject) m_Ball;
	std::vector<Ref(GameLevel)>m_Levels;
	std::vector<Ref(Prop)>m_Props;

	std::tuple<CollisionDir, glm::vec2> IsCollision(const Ref(BallObject)& ball, const Ref(GameObject)& AABB);
	bool IsCollision(const Ref(GameObject)& AABB1, const Ref(GameObject)& AABB2);
	void DoCollision();
	void UpdateProps(float deltaTime);
	void SpawnProp(glm::vec2 Position);
	void ActiveProp(const Ref(Prop)& prop);
	bool IsOtherPropActive(PropType type);
	bool ShouldSpawn(unsigned int chance);
	ma_engine m_AudioEngine;
	ma_sound m_bgm, m_brick, m_prop, m_pad, m_solid_brick;

};

