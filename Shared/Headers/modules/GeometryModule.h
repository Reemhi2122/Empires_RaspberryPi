#pragma once
#include <glm/vec3.hpp>

class GeometryModule
{
public:
	/// <summary>
	/// Add Pointers any Module to a class to give it access to vertex data
	/// Use the Init to send all the data to the GPU
	/// 
	/// </summary>
	GeometryModule();
	~GeometryModule();

	virtual bool Init() = 0;

	glm::vec3 GetVertex(unsigned int a_FirstIndex) const;
	unsigned int GetEboIndex(unsigned int a_Location) const;

	unsigned int GetVao() const;
	unsigned int GetEbo() const;

	unsigned int GetIndexArrSize() const;
	unsigned int GetVertexArraySize() const;
	unsigned int GetTextureID() const;

	bool SetVertex(glm::vec3& a_Vector, unsigned int a_FirstIndex) const;

public:
	float* m_VertexArr_ = nullptr;
	unsigned int* m_Indices_ = nullptr;

	unsigned int m_VertexArrSize_ = 0;
	unsigned int m_IndexArrSize_ = 0;
	unsigned int m_TextureID = 0;
	
	unsigned int m_Vbo_ = 0;
	unsigned int m_Vao_ = 0;
	unsigned int m_Ebo_ = 0;
};