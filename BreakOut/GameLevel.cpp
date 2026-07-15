#include "pch.h"
#include "GameLevel.h"
#include "Sprite2DRenderer.h"
#include "AssetManager.h"
#include "GameObject.h"

GameLevel::GameLevel(const std::string& levelPath, unsigned int Width, unsigned int Height)
{
	Load(levelPath, Width, Height);
}

void GameLevel::Load(const std::string& levelPath, unsigned int Width, unsigned int Height)
{
	std::ifstream fstream(levelPath);
	unsigned int tileCode;
	std::vector<std::vector<unsigned int>>tileData;
	if (!fstream.is_open())
	{
		std::cout << "文件打开失败！:" << levelPath << std::endl;
		return;
	}
	std::string line;
	while (getline(fstream, line))
	{
		std::istringstream sstream(line);
		std::vector<unsigned int>row;
		while (sstream >> tileCode)
		{
			row.push_back(tileCode);
		}
		tileData.push_back(row);
	}
	if(tileData.size()>0)
		Init(tileData, Width, Height);
}

void GameLevel::Draw(const Ref(Sprite2DRenderer)& renderer)
{
	for (auto& it : m_Bricks)
	{
		if(!it->IsDestroyed)
		it->DrawObject(renderer);
	}
}

bool GameLevel::IsCompleted()
{
	for (auto& it : m_Bricks)
	{
		if (!it->IsSolid && !it->IsDestroyed)
			return false;
	}
	return true;
}

bool GameLevel::Init(std::vector<std::vector<unsigned int>>& tileData, unsigned int lvlWidth, unsigned int lvlHeight)
{
	unsigned int x_Count = tileData[0].size();
	unsigned int y_Count = tileData.size();
	float brickWidth =  static_cast<float>(lvlWidth) / x_Count;
	float brickHeight = static_cast<float>(lvlHeight) / y_Count;
	for (int y = 0;y < y_Count;++y)
	{
		for (int x = 0;x < x_Count;++x)
		{
			if (tileData[y][x] == 1)
			{
				glm::vec2 pos(brickWidth*x,lvlHeight + brickHeight*y);
				glm::vec2 size(brickWidth, brickHeight);
				glm::vec3 color(0.8f, 0.8f, 0.7f);
				Ref(GameObject) brick = std::make_shared<GameObject>(AssetManager::GetTexture2D("Solid_Brick.png"), pos, size, color);
				brick->IsSolid = true;
				m_Bricks.push_back(brick);
			}
			else if (tileData[y][x] > 1)
			{
				glm::vec2 pos(brickWidth * x, lvlHeight+brickHeight * y);
				glm::vec2 size(brickWidth, brickHeight);
				glm::vec3 color(1.0f, 1.0f, 1.0f);
				switch (tileData[y][x])
				{
				{case 2:color = glm::vec3(0.2f, 0.6f, 1.0f);break;}
				{case 3:color = glm::vec3(0.0f, 0.7f, 0.0f);break;}
				{case 4:color = glm::vec3(0.8f, 0.8f, 0.4f);break;}
				{case 5:color = glm::vec3(1.0f, 0.5f, 0.0f);break;}
				}
				Ref(GameObject) brick = std::make_shared<GameObject>(AssetManager::GetTexture2D("Normal_Brick.png"), pos, size,color);
				m_Bricks.push_back(brick);
			}
		}
	}
	return true;
}

