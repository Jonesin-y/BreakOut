#pragma once
#include"Sprite2DRenderer.h"
enum GameState
{
	GAME_ACTIVE,GAME_MENU,GAME_WIN
};
class Game
{
public:
	Game(float Width, float Height);
	~Game();
	GameState State;
	bool Keys[1024];
	void Init();
	void Update(float deltaTime);
	void ProcessInput(float deltaTime);
	void Render();
private:
	float m_Width, m_Height;
	Ref(Sprite2DRenderer) m_Sprite2DRenderer;

};

