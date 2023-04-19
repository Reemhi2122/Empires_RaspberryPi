#pragma once
#include <glm/vec3.hpp>

class Shader;

class Material
{
public:
	/// <summary>
	/// Generates a Material that contains texture data, a pointer to the currently used Shader
	/// and a color (in case no texture is to be used). The LoadGenTexture function loads
	/// the texture in a file, sends it to the GPU and produces an ID through which the texture
	/// can be accessed.
	/// </summary>
	/// <param name="a_Shader"></param>
	Material(Shader& a_Shader);

	static void LoadGenTexture(unsigned int& a_TextureId, const char* a_FilePath = "../../../Resources/Textures/Harvey2.jpg");

	unsigned int GetTextureID();

	const Shader& GetCurrentShader();
	void ChangeShader(Shader& a_Shader);

	const glm::vec3& GetColor();
	void SetColor(const glm::vec3& a_Color);


private:
	unsigned int m_Texture_;
	Shader* m_CurrentShader_;
	glm::vec3 m_Color_{1.0f, 1.0f, 0.0f};
};
