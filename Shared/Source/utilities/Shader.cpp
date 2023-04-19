#include "Headers/utilities/Shader.h"
#include "Headers/OGLGraphics.h"
#include <cstdio>
#include <fstream>
#include <vector>

Shader::Shader(const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation)
{
	m_FragShaderId_ = CreateShader(a_FragmentShaderLocation, GL_FRAGMENT_SHADER);
	m_VertShaderId_ = CreateShader(a_VertexShaderLocation, GL_VERTEX_SHADER);

	m_ProgramId_ = LinkShader(m_FragShaderId_, m_VertShaderId_);
}

// both the Create- and LinkShader functions are the Courtesy of Dragos Popescu (200370)
// who helped me greatly with my first steps with OpenGLES and
// with finding my bearings around it
unsigned int Shader::CreateShader(const char* file_name, GLenum type)
{
	std::ifstream file(file_name, std::ios::ate);

	if (!file.is_open())
	{
		fprintf(stdin, "Couldn't load shader: %s\n", file_name);
		return 0;
	}

	const size_t size = static_cast<size_t>(file.tellg());
	std::vector<char> shaderSource(size);
	file.seekg(0);
	file.read(shaderSource.data(), size);
	shaderSource.push_back(0);

	const auto shader = glCreateShader(type);
	auto* data = shaderSource.data();

	glShaderSource(shader, 1, &data, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[200];
		glGetShaderInfoLog(shader,
		                   200, nullptr, log);
		printf("Shader not compiled: %s\n", log);

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

unsigned int Shader::LinkShader(unsigned int fragment_shader, unsigned int vertex_shader)
{
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);

	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		char log[200];
		glGetProgramInfoLog(program, 200, nullptr, log);
		printf(log);

		return 0;
	}

	return program;
}

void Shader::Use() const
{
	glUseProgram(m_ProgramId_);
}

unsigned int Shader::GetProgramID() const
{
	return m_ProgramId_;
}
