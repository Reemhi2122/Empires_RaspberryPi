#pragma once
#include "Headers/OGLGraphics.h"

class Shader
{
public:
	/// <summary>
	/// A simple class for managing Shaders. If provided with a path to a shader file,
	/// it loads && stores it. The shader can the simply be activated by calling Use().
	/// If no path is provided, it will fall back on a default shader.
	/// </summary>
	/// <param name="a_FragmentShaderLocation"></param>
	/// <param name="a_VertexShaderLocation"></param>
	Shader(const char* a_FragmentShaderLocation = "../../../HelloCubes/Resources/Shaders/defaultshader.frag", 
		const char* a_VertexShaderLocation = "../../../HelloCubes/Resources/Shaders/defaultshader.vert");
	
	
	void Use() const;

	unsigned int GetProgramID() const;

private:
	static unsigned int CreateShader(const char* file_name, GLenum type);
	static unsigned int LinkShader(unsigned int fragment_shader, unsigned int vertex_shader);

	unsigned int m_ProgramId_;
	unsigned int m_VertShaderId_;
	unsigned int m_FragShaderId_;
};
