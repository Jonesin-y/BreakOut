#pragma once
#include"Core.h"
#include"Shader.h"
#include"Texture2D.h"
#include<map>
class Shader;
class Texture2D;
class AssetManager
{
public:
	static void LoadShader(const std::string& vs_src, const std::string& fs_src, const std::string& gs_src, const std::string& name);
	static void LoadShader(const std::string& vs_src, const std::string& fs_src, const std::string& name);
	static void LoadTexture2D(const std::string& filePath, const std::string& name);
	const static Ref(Shader)& GetShader(const std::string& name);
	const static Ref(Texture2D)& GetTexture2D(const std::string& name);
private:
	AssetManager(); 
	static std::map<std::string, Ref(Shader)> m_Shaders;
	static std::map<std::string, Ref(Texture2D)>m_Texture2Ds;
	
};

