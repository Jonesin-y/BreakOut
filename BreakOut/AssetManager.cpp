#include"pch.h"
#include "AssetManager.h"
#include"Shader.h"
#include"Texture2D.h"
std::map<std::string, Ref(Shader)>AssetManager::m_Shaders;
std::map<std::string, Ref(Texture2D)>AssetManager::m_Texture2Ds;
void AssetManager::LoadShader(const std::string& vs_src, const std::string& fs_src, const std::string& gs_src, const std::string& name)
{
	m_Shaders[name] = std::make_shared<Shader>(vs_src, fs_src,gs_src);
}

void AssetManager::LoadShader(const std::string& vs_src, const std::string& fs_src, const std::string& name)
{
	m_Shaders[name] = std::make_shared<Shader>(vs_src, fs_src);
}

void AssetManager::LoadTexture2D(const std::string& filePath, const std::string& name)
{
	m_Texture2Ds[name] = std::make_shared<Texture2D>(filePath);
}

const Ref(Shader)& AssetManager::GetShader(const std::string& name)
{
	auto it = m_Shaders.find(name);
	if (it != m_Shaders.end())
	{
		return it->second;
	}

	printf("Shader: %s 不存在喵!\n", name.c_str());
	static const Ref(Shader) s_NullShader;
	return s_NullShader;
}

const Ref(Texture2D)& AssetManager::GetTexture2D(const std::string& name)
{
	auto it = m_Texture2Ds.find(name);
	if (it != m_Texture2Ds.end())
	{
		return it->second;
	}

	printf("Texture2D: %s 不存在喵!\n", name.c_str());
	static const Ref(Texture2D) s_NullTexture2D;
	return s_NullTexture2D;
}
