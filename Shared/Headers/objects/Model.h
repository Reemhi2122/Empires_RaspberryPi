#pragma once
#include <vector>

class Model
{
public:
	Model();
	~Model();

	std::vector<float>* GetVertexData();
	std::vector<float>* GetIndicies();

private:
	std::vector<float> m_VertexData;
	std::vector<float> m_Indicies;
};