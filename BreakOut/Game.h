#pragma once
#include"Sprite2DRenderer.h"
#include"GameObject.h"
#include"BallObject.h"
#include<vector>
#include<memory>
#include<tuple>
#include"Core.h"
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
	std::tuple<CollisionDir, glm::vec2> IsCollision(const Ref(BallObject)& ball, const Ref(GameObject)& AABB);
	std::tuple<CollisionDir, glm::vec2> IsCollision(const Ref(GameObject)& AABB1, const Ref(GameObject)& AABB2);
	void DoCollision();
private:
	float m_Width, m_Height;
	unsigned int m_Level;
	
	Ref(Sprite2DRenderer) m_Sprite2DRenderer;
	Ref(GameObject) m_Player;
	Ref(BallObject) m_Ball;
	std::vector<Ref(GameLevel)>m_Levels;
};

