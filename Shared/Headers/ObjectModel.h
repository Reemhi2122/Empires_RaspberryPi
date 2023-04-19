#pragma once
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "Headers/MyFiles.h"

#include "OGLGraphics.h"

#include "TOL/tiny_obj_loader.h"

/* Object model is a base class to represent the most basic form of models
its update and draw classes are pure virtual so any object derived from this must supply those function
It contains simplified matrices for orientation, position and scaling
Student must supply the functions needed to create an use these values
*/

#define PI				3.14159265358979323846264338327950288419716939937510582097494459072381640628620899862803482534211706798f
#define DEG2RAD(x) (x*PI)/180
#define RAD2DEG(x) x*(180/PI)

class Camera;
class Material;
class GeometryModule;
class Graphics;

inline float Rand(float a_Range);

//using namespace glm;

class ObjectModel 
{
public:
	typedef struct {
		GLuint vb;
		int numTriangles;
		size_t material_id;
	} DrawObject;

	ObjectModel();
	~ObjectModel();

	virtual bool Update() = 0;  // we need to have an Update to move and create the ModelView
	virtual bool Draw(Camera* a_ActiveCamera) = 0;  // and we need to draw from the vertices

	bool LoadandConvert(  // a complex routine which actually simplifies the loading of date using Tinyobj
		std::vector<DrawObject>* drawObjects,
		std::vector<tinyobj::material_t>& materials,
		std::map<std::string,
		GLuint>& textures,
		const char* filename,
		MyFiles* FH);

	vector<float> LoadModel(const char* a_ModelFile);

	void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);

	// a few basic utility functions are expected to be done by students
	glm::vec3	GetPosition();
	void		SetPosition(glm::vec3*);
	void		SetPosition(glm::vec3);
	
	glm::vec3	GetRotations();
	void		SetRotations(glm::vec3*);
	void		SetRotations(glm::vec3);
	
	void		SetXRotation(float);  // if you need to set indivual rotations
	void		SetYRotation(float);
	void		SetZRotation(float);
	
	void		SetModelMatrix();  // initialises and resets
	void	 	MakeModelMatrix();  // makes and returns the model matrix
	void		MakeRotations();
	void		MakeTranslationMatrix();

	void		MakeRotationMatrix();  // since these get altered a lot we have a function to remake the matrix
	bool		LoadModel(GLvoid* vertices);  // needs expanding 
	
	void		ChangeModule(GeometryModule* a_Module);
	void		ChangeMaterial(Material* a_Material);

	glm::vec3 LinearInterplation(glm::vec3 a, glm::vec3 b, float t);

	glm::vec3 LerpBetween(glm::vec3 point_a, glm::vec3 point_b, float t);
	
	glm::vec3 QuadraticInterpolate(glm::vec3 point_a, glm::vec3 point_b, glm::vec3 point_c, glm::vec3 point_d, float time);

	glm::vec3 CubicBezier(glm::vec3 point_a, glm::vec3 point_b, glm::vec3 point_c, glm::vec3 point_d,
		float time);

	// member values for the class hold info that is used for movement and drawing	
	// weve given you what you should expect to have....you can add more
	std::vector<tinyobj::material_t> materials;
	std::vector<DrawObject> gdrawObjects;
	std::map<std::string, GLuint> textures;

	glm::vec3	WorldPosition;   // where am I in the world?
	glm::vec3	Rotations; 		// what are my rotations? Stored as Radians!
	glm::vec3	DegreeRotations;
	glm::vec3	Scales;			
	
	glm::mat4 mTranslationMatrix;
	glm::mat4 mRotationMatrix;
	glm::mat4 mScaleMatrix;
	
	glm::mat4 RotationMatrixX;
	glm::mat4 RotationMatrixY;
	glm::mat4 RotationMatrixZ;
	
	// the model matrix will effectivly hold all the rotation and positional data for the object
	glm::mat4 ModelMatrix;
	
	//GLuint texture1;  // a handle to the texture of the mesh/model (can add more if needed or make into an array)

	glm::vec4	Colour;  

	GLvoid*	 Vertices;  // a pointer to our model array, which should contain vertices and texture coords / expand change as needed 
	GLuint	vbo;

	protected:
	Material* m_CurrentMaterial_;
	GeometryModule* m_CurrentModule_;
};