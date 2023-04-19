#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>


//#include "quad_tree/QuadTree.h"
#include "utilities/DataStorage.h"

class DataStorage;

class Generator
{
public:
	explicit Generator(int a_Seed);
	~Generator();
	
	void GenerateGalaxy(const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation);
	std::vector<glm::vec3>* GetGeneratedPositions();

private:
	
	void GenerateNames(int a_Amount, int a_Mode);
	
	void GeneratePositions(const unsigned int a_Amount);

	static bool LoadInputList(const char* a_FilePath, std::vector<std::string>& a_Storage);
	
	void ClearData();
	static float GenRandFloat();
	static char GenRandVowel();
	static char GenRandConsonant();

	static float GenGaussian(double a_Mean, double a_StandardDeviation);

	static int GenLevel(float a_Mean, float a_StandardDeviation, float a_Min, float a_Max);

	void GenerateEmpires(int a_Amount);

	glm::vec4 GenFactionColor();

	void GenPlanetColors(glm::vec3 a_MasterColor, glm::vec3& a_LandColor, glm::vec3& a_WaterColor);
	
	glm::vec4 GenColor(glm::vec3 a_MixColor);

	//void GenQuadTree(DataStorage& a_Storage, int screen_width, int screen_height);

	void GenConnections(int a_MaxDistance);
	
	int m_Seed_;
	
	enum Modes
	{
		Syllable = 0,
		Letter = 1
	};

	std::vector<std::string> generated_names;
	std::vector<glm::vec3> generated_positions;
	std::vector<std::string> m_SyllableList_;
	std::vector<std::string> m_SymbolList_;
};
