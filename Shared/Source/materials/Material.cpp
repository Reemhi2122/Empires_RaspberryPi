#include "materials/Material.h"
#include "stb_image.h"

#include "OGLGraphics.h"



Material::Material(Shader& a_Shader) : m_CurrentShader_(&a_Shader)
{
	//LoadGenTexture(m_Texture_);
}

// the main functionality of this function was heavily inspired by this tutorial: https://learnopengl.com/Getting-started/Textures
void Material::LoadGenTexture(unsigned& a_TextureId, const char* a_FilePath)
{
	int width = 0;
	int height = 0;
	int nrChannels = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(a_FilePath, &width, &height, &nrChannels, 0);

	glGenTextures(1, &a_TextureId);

	glBindTexture(GL_TEXTURE_2D, a_TextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// checking whether or not texture was loaded
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to Load texture");
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, a_TextureId);
}

void Material::ChangeShader(Shader& a_Shader)
{
	m_CurrentShader_ = &a_Shader;
}

unsigned Material::GetTextureID()
{
	return m_Texture_;
}

const Shader& Material::GetCurrentShader()
{
	return *m_CurrentShader_;
}

const glm::vec3& Material::GetColor()
{
	return m_Color_;
}

void Material::SetColor(const glm::vec3& a_Color)
{
	m_Color_ = a_Color;
}
