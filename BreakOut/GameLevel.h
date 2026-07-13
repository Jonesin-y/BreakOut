#pragma once
#include<string>
#include<memory>
#include<vector>
#include"Core.h"
class Sprite2DRenderer;
class GameObject;
class GameLevel
{
public:
	GameLevel() {}
	GameLevel(const std::string& levelPath, unsigned int Width, unsigned int Height);
	void Load(const std::string& levelPath,unsigned int Width,unsigned int Height);
	void Draw(const Ref(Sprite2DRenderer)& renderer);
	bool IsCompleted();
	const std::vector<Ref(GameObject)>& GetBricks()const { return m_Bricks; }
private:
	void Init(std::vector<std::vector<unsigned int>>& tileData,unsigned int Width,unsigned int Height);
	std::vector<Ref(GameObject)> m_Bricks;
};