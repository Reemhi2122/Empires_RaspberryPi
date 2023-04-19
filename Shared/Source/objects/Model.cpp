#include "Headers/objects/Model.h"

Model::Model() {}

Model::~Model() {}

std::vector<float>* Model::GetVertexData()
{
    return &m_VertexData;
}

std::vector<float>* Model::GetIndicies() 
{
    return &m_Indicies;
}