#include "Headers/modules/DebugModule.h"
#include <cstdio>

DebugModule::DebugModule(std::vector<GLfloat>* a_Vertices)
{
	m_Vertices = a_Vertices;
	Init();
}

DebugModule::~DebugModule()
{}

bool DebugModule::Init()
{
	glGenVertexArrays(1, &m_Vao_);
	glGenBuffers(1, &m_Vbo_);

	glBindVertexArray(m_Vao_);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo_);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBufferData(GL_ARRAY_BUFFER, m_Vertices->size() * sizeof(float), &m_Vertices->at(0), GL_STATIC_DRAW);
	return true;
}
