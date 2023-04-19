#include "modules/CubeModule.h"

#include <cstdio>
#include "OGLGraphics.h"

CubeModule::CubeModule()
{
	m_VertexArrSize_ = 64;
	m_IndexArrSize_ = 36;
	Init();
}

CubeModule::~CubeModule()
{
}

bool CubeModule::Init()
{
	if (m_VertexArrSize_ == 0 || m_IndexArrSize_ == 0)
	{
		#ifdef DEBUG
		printf("Error: vertex_arr_size_ and index_arr_size cannot be empty!");
		#endif

		return false;
	}

	m_VertexArr_ = new float[m_VertexArrSize_] {
		// vertices					// colors				// texture coordinates
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

			-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, .0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
	};

	m_Indices_ = new unsigned int[m_IndexArrSize_] {

			// triangle one
			0, 1, 2,

			// triangle two
			0, 2, 3,

			// triangle three
			4, 0, 3,

			// triangle four
			4, 3, 7,

			// triangle five
			5, 1, 2,

			// triangle six
			5, 2, 6,

			// triangle seven
			4, 5, 6,

			// triangle eight
			4, 6, 7,

			// triangle nine
			0, 1, 5,

			// triangle ten
			0, 5, 4,

			// triangle eleven
			5, 2, 6,

			// triangle twelve
			5, 6, 7
	};

	#ifdef DEBUG
	if (m_VertexArr_ == nullptr || m_Indices_ == nullptr)
	{
		printf("Error: Could not allocate vertex_arr_ or indices_!");
		return false;
	}
	printf("Allocation of vertex_arr_ and indices_ succeeded.");
	#endif
	
	// generating buffers & vertex arrays
	glGenVertexArrays(1, &m_Vao_);
	glGenBuffers(1, &m_Vbo_);
	glGenBuffers(1, &m_Ebo_);

	glBindVertexArray(m_Vao_);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo_);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBufferData(GL_ARRAY_BUFFER, m_VertexArrSize_ * sizeof(float), m_VertexArr_, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexArrSize_ * sizeof(unsigned int), m_Indices_, GL_STATIC_DRAW);

	return true;
}
