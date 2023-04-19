#include "Headers/modules/GeometryModule.h"
#include <cstdio>

GeometryModule::GeometryModule()
{
}

GeometryModule::~GeometryModule()
{
	delete[] m_VertexArr_;
	delete[] m_Indices_;

	#ifdef DEBUG
	printf("Geometry Object successfully deleted");
	#endif
}

glm::vec3 GeometryModule::GetVertex(const unsigned int a_FirstIndex) const
{
	glm::vec3 temp;
	temp.x = m_VertexArr_[a_FirstIndex];
	temp.y = m_VertexArr_[a_FirstIndex + 1];
	temp.z = m_VertexArr_[a_FirstIndex + 2];

	return temp;
}

unsigned int GeometryModule::GetEboIndex(const unsigned int a_Location) const
{
	return m_Indices_[a_Location];
}

unsigned GeometryModule::GetVao() const
{
	return m_Vao_;
}

unsigned int GeometryModule::GetEbo() const
{
	return m_Ebo_;
}

unsigned GeometryModule::GetIndexArrSize() const
{
	assert(m_IndexArrSize_ != 0);
	return m_IndexArrSize_;
}

unsigned GeometryModule::GetVertexArraySize() const
{
	assert(m_VertexArrSize_ != 0);
	return m_VertexArrSize_;
}

unsigned int GeometryModule::GetTextureID() const
{
	return m_TextureID;
}

bool GeometryModule::SetVertex(glm::vec3& a_vector, const unsigned int first_index) const
{
	const int arraySize = m_IndexArrSize_;
	
	if (first_index < arraySize)
	{
		m_VertexArr_[first_index] = a_vector.x;
		m_VertexArr_[first_index + 1] = a_vector.y;
		m_VertexArr_[first_index + 2] = a_vector.z;

		return true;
	}
	
	printf("Error: index out of bounds. Put in a index between 0 and %i", arraySize);
	return false;
}
