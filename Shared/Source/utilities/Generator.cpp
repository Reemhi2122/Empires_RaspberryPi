#include "Headers/utilities/Generator.h"
#include "Headers/utilities/Shader.h"
#include "Headers/modules/CubeModule.h"
#include "Headers/modules/ModelModule.h"
#include "Headers/modules/PyramidModule.h"
#include "Headers/materials/Material.h"
#include "Headers/objects/Planet.h"
#include "Headers/Empire.h"
#include "Headers/PhysicsObj.h"

#include <cmath>
#include <cstdlib>
#include <fstream>

#include "Headers/utilities/DataStorage.h"

Generator::Generator(int a_Seed) : m_Seed_(a_Seed)
{
}

Generator::~Generator()
{
}

void Generator::GenerateNames(const int a_Amount, const int a_Mode)
{
	std::cout << "\nStarted Generating Names..." << std::endl;

	switch (a_Mode)
	{
		case Syllable:
		{
			// loading lists
#ifdef _RASPARM
			LoadInputList("../../../Shared/Resources/Generators/syllables.txt", m_SyllableList_);
#else
			LoadInputList("../Shared/Resources/Generators/syllables.txt", m_SyllableList_);
#endif

			for (int i = 0; i < a_Amount; i++)
			{
				//std::cout << "Position: " << i << " / " << a_Amount << std::endl;
				
				// generating length of name
				const int randNameLength = (rand() % 5) + 2;
				std::string name;

				bool uppercase = true;

				for (int k = 0; k < randNameLength; k++)
				{
					// if name length is not 2 and k == name length/2, add a space,
					// else add a random syllable
					if (randNameLength != 2 && k == randNameLength / 2)
					{
						name += " ";
						uppercase = true;
					}
					else
					{
						const int randNumbr = rand() % (m_SyllableList_.size() -1);

						// concatenate syllable either with uppercase or lower case
						// depending on whether the last added symbol was a space
						if (uppercase)
						{
							std::string& temp = m_SyllableList_[randNumbr];
							temp[0] = toupper(temp[0]);
							name += temp;
							
						} else
						{
							name += m_SyllableList_[randNumbr];
						}

						uppercase = false;
					}
				}
				name[0] = toupper(name[0]);
				generated_names.push_back(name);
			}

			ClearData();
			break;
		}

		case Letter:
		{
		
			for (int i = 0; i < a_Amount; i++)
			{
				//std::cout << "Position: " << i << " / " << a_Amount << std::endl;

				// generate name length
				const int randNameLength = (rand() % 14) + 2;
				std::string name;

				//make sure first letter is capitalized
				bool uppercase = true;

				// for the name length, choose random letter
				for (int k = 0; k < randNameLength; k++)
				{
					// add blank space at the middle of the name if the name is
					// longer than 5 letters
					if (randNameLength > 5 && k == randNameLength / 2)
					{
						
						name += " ";
						uppercase = true;
						
					} else
					{
						// alternate between Vowels and Consonants
						if (k % 2 == 1)
						{
							name += GenRandVowel();
						}
						else name += GenRandConsonant();

						// make letter uppercase if needed
						if (uppercase) name[k] = toupper(name[k]);

						// reset uppercase indicator
						uppercase = false;
					}
				}

				// store name
				generated_names.push_back(name);
			}
			break;
		}
		
		default:
		{
			generated_names.push_back("Error! Enter valid mode.");
			break;
		}
	}
}

// this code was taken from https://itinerantgames.tumblr.com/post/78592276402/a-2d-procedural-galaxy-with-c
void Generator::GenerateGalaxy(const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation)
{
	//seed generation
	srand(m_Seed_);
	
	DataStorage* a_Storage;
	a_Storage = a_Storage->GetInstance();
	
	Shader* default_shader = new Shader(a_FragmentShaderLocation, a_VertexShaderLocation);
	Material* default_material = new Material(*default_shader);

#ifdef _RASPARM
	GeometryModule* default_module = new ModelModule("../../../Shared/Resources/Objects/MarsPlanet.obj");
#else
	GeometryModule* default_module = new ModelModule("../Shared/Resources/Objects/MarsPlanet.obj");
#endif

	a_Storage->PushBackShader(default_shader);
	//a_Storage.PushBackShader(backup_shader);
	
	a_Storage->PushBackMaterial(default_material);
	//a_Storage.PushBackMaterial(backup_material);
	
	a_Storage->PushBackModule(default_module);
	//a_Storage.PushBackModule(backup_module);

	GeneratePositions(a_Storage->amount_planets);
	GenerateNames(a_Storage->amount_planets, Syllable);

	std::cout << "\nStarted Generating Planets..." << std::endl;

	int amount_planets = a_Storage->amount_planets;

	for(int i = 0; i < amount_planets; i++)
	{
		//std::cout << "Planet: " << i << " / " << amount_planets << std::endl;
		Planet* temp = new Planet(default_material, default_module);
		temp->Scales = glm::vec3(0.05, 0.05, 0.05);
		
		temp->WorldPosition = generated_positions[i];
		temp->SetName(&generated_names[i]);
		temp->UpdatePosition();
		
		temp->SetTechLevel(GenLevel(5, 2, 1.0f, 15.0f));
		temp->SetResourceLevel(GenLevel(5, 2, 1.0f, 15.0f));

		temp->SetSeed(rand());
		temp->SetHelperSeed(rand());
		int noise_scale = 0;

		// generate noise level leaning closer towards the lower en
		// to make planets with only "islands" less likely
		while(20 > noise_scale || noise_scale > 60)
		{
			noise_scale = GenGaussian(35, 5);
		}
		
		temp->SetNoiseScale(noise_scale);
		glm::vec3 landColor;
		glm::vec3 waterColor;

		GenPlanetColors({ 112,175,133 }, landColor, waterColor);
		
		temp->SetLandColor(landColor);
		temp->SetWaterColor(waterColor);
		
		a_Storage->GetPhysicsWorld()->addRigidBody(temp->GetPhysicsObj()->GetRigidBody());
		a_Storage->PushBackPlanet(*temp);
	}

	GenConnections(70);

	GenerateEmpires(a_Storage->max_num_empires);
}

std::vector<glm::vec3>* Generator::GetGeneratedPositions()
{
	return &generated_positions;
}

void Generator::GeneratePositions(const unsigned int a_Amount)
{
	std::cout << "\nStarted Generating Positions..." << std::endl;
	
	const float pi = 3.14159265358979;

	const int numArms = 5;
	const float armDistance = 2 * pi / numArms;
	const float maxOffset = 1.0f;
	const float rotationFactor = 1;
	const float secondaryOffset = 0.02f;

	for (int i = 0; i < a_Amount; i++)
	{
		//std::cout << "Position: " << i << " / " << a_Amount << std::endl;
		float distance = (GenRandFloat() + 0.15f) * 1.65;

		// calculating angle between 0 and 2 * pi
		float angle = GenRandFloat() * 2.0f * pi;

		// calculating offset from central spine
		float offset = GenRandFloat() * maxOffset;
		offset -= maxOffset / 2.0f;
		offset *= 1.0f / distance;

		float offsetSquared = offset * offset;
		if (offset < 0) offsetSquared *= -1.0f;

		offset = offsetSquared;

		// calculating rotation
		const float rotation = distance * rotationFactor;

		angle = static_cast<int>(angle / armDistance) * armDistance + offset + rotation;

		// calculating final vector
		glm::vec3 temp;

		// converting polar to cartesian coordinates
		temp.x = cos(angle) * distance;
		temp.y = sin(angle) * distance;
		temp.z = 0.0f;

		//applying secondary offset
		float xOffset = GenRandFloat() * secondaryOffset;
		float yOffset = GenRandFloat() * secondaryOffset;

		temp.x += xOffset;
		temp.y += yOffset;

		temp *= 500.f;

		// storing vector in data storage
		generated_positions.push_back(temp);
	}
	std::cout << "\nFinished Generating Positions" << std::endl;
}

bool Generator::LoadInputList(const char* a_FilePath, std::vector<std::string>& a_Storage)
{
	// open file, start at beginning
	std::ifstream file(a_FilePath, std::ios::in);

	if (!file.is_open())
	{
		fprintf(stdin, "Could not open file: %s\n", a_FilePath);
		return false;
	}

	// storing the file contents in the storage provided
	std::string temp;
	while (file >> temp)
	{
		a_Storage.push_back(temp);
	}

	file.close();
	return true;
}

// deletes loaded lists from the free store,
// advisable after name generation is complete
void Generator::ClearData()
{
	m_SyllableList_.clear();
	m_SyllableList_.resize(0);

	m_SymbolList_.clear();
	m_SymbolList_.resize(0);
}

// returns a random float between 0.0-1.0
float Generator::GenRandFloat()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

char Generator::GenRandVowel()
{
	char vowels[] = { 'a', 'e', 'i', 'o', 'u' };

	const unsigned int randNmbr = rand() % 5;

	return vowels[randNmbr];
}

char Generator::GenRandConsonant()
{
	char consonants[] = { 'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
						'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z' };

	const unsigned int randNmbr = rand() % 21;

	return consonants[randNmbr];
}


/// <summary>
/// Generates a value distributed with Gaussian Distribution
/// Based on the Marsaglia Polar method
/// </summary>
/// <param name="a_Mean"></param>
/// <param name="a_StandardDeviation"></param>
/// <returns></returns>

// the following two functions were taken
// from https://www.alanzucconi.com/2015/09/16/how-to-sample-from-a-gaussian-distribution/
float Generator::GenGaussian(const double a_Mean, const double a_StandardDeviation)
{
	float v1, v2, s;
	do {
		v1 = (rand() / static_cast<double>(RAND_MAX) * 2.0 - 1.0);
		v2 = (rand() / static_cast<double>(RAND_MAX) * 2.0 - 1.0);
		s = v1 * v1 + v2 * v2;
	} while (s >= 1.0f || s == 0.f);
	s = sqrt((-2.0f * log(s)) / s);

	return a_Mean * v1 * s * a_StandardDeviation;
}

/// <summary>
/// Generates a Level in a given range with a bell curve distribution.
/// </summary>
/// <param name="a_Mean"></param>
/// <param name="a_StandardDeviation"></param>
/// <param name="a_Min"></param>
/// <param name="a_Max"></param>
/// <returns></returns>
int Generator::GenLevel(const float a_Mean, const float a_StandardDeviation, const float a_Min, const float a_Max)
{
	float x;
	do {
		x = GenGaussian(a_Mean, a_StandardDeviation);
	} while (x < a_Min || x > a_Max);
	return static_cast<int>(x);
}

void Generator::GenerateEmpires(int a_Amount)
{
	DataStorage* a_Storage;
	a_Storage = a_Storage->GetInstance();
	
	std::cout << "\nStarted Generating Empires..." << std::endl;
	
	for (int i = 0; i < a_Amount; i++)
	{
		std::cout << "Empire " << i << " / " << a_Amount << std::endl;
		// generate starting credits (between 1 - 100)
		unsigned int startCredits = rand() % (100) + 1;

		// generating tech level and resource level
		unsigned int baseTech = GenLevel(5, 3.5, 1.0f, 15.0f);
		unsigned int baseResource = GenLevel(5, 3.5, 1.0f, 15.0f);

		// generating a number between 1 and the maximum amount of planets
		int planet_position = rand() % (a_Storage->amount_planets) + 1;
		assert(planet_position >= 0 && planet_position <= a_Storage->amount_planets);

		// getting random start planet
		Planet* startplanet = a_Storage->GetPlanet(planet_position);

		std::string name = startplanet->GetName() + " Empire";
		// generating Empire
		glm::vec4 faction_color = GenFactionColor();
		Empire* tEmpire = new Empire{name, faction_color, startplanet, startCredits, baseTech, baseResource};

		// setting start planet owner to generated empire
		startplanet->SetOwner(tEmpire);

		// storing Empire in DataStorage
		a_Storage->PushBackEmpire(tEmpire);
	}

	for (int i = 1; i < a_Amount; i++)
	{
		a_Storage->PushBackEmpireAI(new EmpireAI(a_Storage->GetEmpire(i)));
	}

	std::cout << "\nFinished Generating Empires" << std::endl;
}

glm::vec4 Generator::GenFactionColor()
{
	/*int r = rand() % (210 + 1 - 1) + 1;
	int g = rand() % (210 + 1 - 1) + 1;
	int b = rand() % (210 + 1 - 1) + 1;*/

	float r = static_cast<float>(rand() % 210 + 100);
	float g = static_cast<float>(rand() % 210 + 100);
	float b = static_cast<float>(rand() % 210 + 100);

	assert(100 <= r <= 210);
	assert(100 <= g <= 210);
	assert(100 <= b <= 210);

	return glm::vec4{ r/255.f, g/255.f, b/255.f, 1.f };
}

void Generator::GenPlanetColors(glm::vec3 a_MasterColor, glm::vec3& a_LandColor, glm::vec3& a_WaterColor)
{
	glm::vec3 palette[6] = {
		GenColor(a_MasterColor), GenColor(a_MasterColor), GenColor(a_MasterColor), GenColor(a_MasterColor),
		GenColor(a_MasterColor), GenColor(a_MasterColor)
	};

	glm::vec3* ref = &a_LandColor;
	
	for (int i = 0; i < 2; i++) {
		if (i == 1) ref = &a_WaterColor;
		
		switch (rand() % 6)
		{
		case 0:
			*ref = GenColor(palette[0]);
			break;
		case 1:
			*ref = GenColor(palette[1]);
			break;
		case 2:
			*ref = GenColor(palette[2]);
			break;
		case 3:
			*ref = GenColor(palette[3]);
			break;
		case 4:
			*ref = GenColor(palette[4]);
			break;
		case 5:
			*ref = GenColor(palette[5]);
			break;
		default:
			*ref = GenColor(palette[0]);
			break;
		}
	}
}

glm::vec4 Generator::GenColor(glm::vec3 a_MixColor)
{
	float r = static_cast<float>(rand() % 255 + 1);
	float g = static_cast<float>(rand() % 255 + 1);
	float b = static_cast<float>(rand() % 255 + 1);

	// mix the color
	r = (r + a_MixColor.x) / 2;
	g = (g + a_MixColor.y) / 2;
	b = (b + a_MixColor.z) / 2;

	return { r / 255.f, g / 255.f, b / 255.f, 1.f };
}

//void Generator::GenQuadTree(DataStorage& a_Storage, int screen_width, int screen_height)
//{
//	int temp_width = screen_width / 2;
//	int temp_height = screen_height / 2;
//
//	Rectangle boundary{ temp_width, temp_height, temp_width, temp_height };
//
//	QuadTree* quadTree = new QuadTree{ boundary, 4 };
//
//	for (int i = 0; i < a_Storage.GetPlanetStorageSize(); i++)
//	{
//		const glm::vec3& temp = a_Storage.GetPlanet(i)->WorldPosition;
//		Point p{temp.x, temp.y, *a_Storage.GetPlanet(i)};
//
//		quadTree->InsertPoint(p);
//	};
//
//	// store quad tree
//	a_Storage.SetQuadTree(quadTree);
//}


/// <summary>
/// Generates a Web of Connections between planets, usable for SOI and Pathfinding.
/// This idea was originally thought of by Jan Vijfhuizen (201402) and I decided to
/// try writing my own implementation.
/// </summary>
/// <param name="a_Storage"></param>
/// <param name="a_MaxDistance"></param>
void Generator::GenConnections(int a_MaxDistance)
{
	DataStorage* a_Storage;
	a_Storage = a_Storage->GetInstance();
	
	std::cout << "\nStarted Building Connections between Planets..." << std::endl;
	
	int PlanetCount = a_Storage->GetPlanetStorageSize();

	#pragma omp parallel for
	// for each planet
	for (int i = 0; i < PlanetCount; i++)
	{
		//std::cout << "Building Connections: " << i << " / " << PlanetCount << std::endl;
		
		// for every other planet
		for (int k = 0; k < PlanetCount; k++)
		{
			if (i != k)
			{
				// use pythagoras to calculate distance
				float x = a_Storage->GetPlanet(k)->WorldPosition.x - a_Storage->GetPlanet(i)->WorldPosition.x;
				float y = a_Storage->GetPlanet(k)->WorldPosition.y - a_Storage->GetPlanet(i)->WorldPosition.y;
				float z = a_Storage->GetPlanet(k)->WorldPosition.z - a_Storage->GetPlanet(i)->WorldPosition.z;

				const unsigned int distanceSquared = static_cast<unsigned int>(x * x + y * y + z * z);

				// if distance is smaller than max distance, add connection
				if (distanceSquared < (a_MaxDistance * a_MaxDistance))
				{
					a_Storage->GetPlanet(i)->m_ClosePlanets_.push_back(a_Storage->GetPlanet(k));

					// this will add double connections
					//a_Storage.GetPlanet(k)->m_ClosePlanets_.push_back(a_Storage.GetPlanet(i));
				}
			}
		}
	}
	std::cout << "Finished Building Connections!" << std::endl;

}
