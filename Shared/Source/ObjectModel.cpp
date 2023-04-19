#include "../Headers/ObjectModel.h"
#include "OGLGraphics.h"

using namespace glm;

float Rand(float a_Range)
{
	return ((float)rand() / RAND_MAX) * a_Range;
}

// Not really doing anything, has to be handled in the derived class
ObjectModel::ObjectModel() {}
ObjectModel::~ObjectModel() {}

//this loads a standard OBJ file, since many types of object are likely to use OBJ's we allow the base model class (ObjectModel) to be able to load and store a standard OBJ model
bool  ObjectModel::LoadandConvert(
	std::vector<DrawObject>* drawObjects,
	std::vector<tinyobj::material_t>& materials,
	std::map<std::string,
	GLuint>& textures,
	const char* filename,
	MyFiles* FH) {

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;

	std::string err;
	std::string warn;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, "../Shared/Resources/Objects/");

	if (!err.empty())
	{
		char* cstr = &err[0u];   // printf needs chars
		printf("oh Bugger can't load this obj file %s file returns error %s \n", filename, cstr);
	}

	if (!ret)
	{
		printf("The %s.obj seems to be corrupt or incorrectly formated\n", filename);
		return false;
	}


	// output the data, but wrap in a debug when not used
	printf("Attrib data for %s is :-\n", filename);
	printf("# vertices  : %d\n", (int)(attrib.vertices.size()) / 3);
	printf("# normals   : %d\n", (int)(attrib.normals.size()) / 3);
	printf("# texcoords : %d\n", (int)(attrib.texcoords.size()) / 2);
	printf("Other data  :-\n");

	printf("# materials : %d\n", (int)materials.size());
	printf("# shapes    : %d\n", (int)shapes.size());

	// Append `default` material
	materials.push_back(tinyobj::material_t());

	char* base_dir = (char*)"../Shared/Resources/Objects/";   //<< this pre-supposes you store your material files with your obj and texture files
	// Parse the materials and Load and create textures
	{

		for (size_t m = 0; m < materials.size(); m++) {
			tinyobj::material_t* mp = &materials[m];

			if (mp->diffuse_texname.length() > 0) {
				// Only load the texture if it is not already loaded
				if (textures.find(mp->diffuse_texname) == textures.end()) {
					GLuint texture_id;
					int w, h;

					std::string texture_filename = mp->diffuse_texname;
					texture_filename = base_dir + mp->diffuse_texname;

					char* cstr = &texture_filename[0u];    // embarrasing side effect of the decision to use char.....

					char* image = FH->Load(cstr, &w, &h);

					if (!image) {
						printf("Unable to load texture:%s \n", cstr);
						exit(1);
					}

					printf("Texture info for %s :-\n", cstr);

					printf("# width   : %d\n", w);
					printf("# Height  : %d\n", h);
					printf("# comp    : %d\n", FH->comp);


					glGenTextures(1, &texture_id);
					glBindTexture(GL_TEXTURE_2D, texture_id);

					// set up some paramaters
					// Set our texture parameters
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   	// Set texture wrapping to GL_REPEAT
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					// Set texture filtering
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

					if (FH->comp == 4)	// check if we had an alpha texture or a plane (strictly speaking any value not 3 is wrong)	
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
					}
					else
					{
						if (FH->comp != 3) printf("Made a default non alpha texture, comp was :%d\n", FH->comp);
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
					}

					if (glGetError() != GL_NO_ERROR)
					{
						printf("Oh bugger:- Model texture creation error, but attempting to carry on.\n");

					}

					glBindTexture(GL_TEXTURE_2D, 0);
					free(image);    // release the cpu memory once its been put in the GPU
					// store the resulting texture ID in a map, associated with its filename
					textures.insert(std::make_pair(mp->diffuse_texname, texture_id));
				}
			}
		}
	}


	{
		// now lets put lots of data out of the vertext list, 	
		for (size_t s = 0; s < shapes.size(); s++) {
			DrawObject o;
			std::vector<float> vb;     // pos(3float), normal(3float), color(3float)
			for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
				tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
				tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
				tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

				int current_material_id = shapes[s].mesh.material_ids[f];

				if ((current_material_id < 0) || (current_material_id >= static_cast<int>(materials.size()))) {
					// Invaid material ID. Use default material.
					current_material_id = materials.size() - 1;    // Default material is added to the last item in `materials`.
				}

				float diffuse[3];
				for (size_t i = 0; i < 3; i++) {
					diffuse[i] = materials[current_material_id].diffuse[i];
				}
				float tc[3][2];
				if (attrib.texcoords.size() > 0) {
					assert(attrib.texcoords.size() > 2 * idx0.texcoord_index + 1);
					assert(attrib.texcoords.size() > 2 * idx1.texcoord_index + 1);
					assert(attrib.texcoords.size() > 2 * idx2.texcoord_index + 1);
					tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
					tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
					tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
					tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
					tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
					tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
				}
				else {
					tc[0][0] = 0.0f;
					tc[0][1] = 0.0f;
					tc[1][0] = 0.0f;
					tc[1][1] = 0.0f;
					tc[2][0] = 0.0f;
					tc[2][1] = 0.0f;
				}

				float v[3][3];
				for (int k = 0; k < 3; k++) {
					int f0 = idx0.vertex_index;
					int f1 = idx1.vertex_index;
					int f2 = idx2.vertex_index;
					assert(f0 >= 0);
					assert(f1 >= 0);
					assert(f2 >= 0);

					v[0][k] = attrib.vertices[3 * f0 + k];
					v[1][k] = attrib.vertices[3 * f1 + k];
					v[2][k] = attrib.vertices[3 * f2 + k];

					// this will later be used to get bounding boxes, but not yet			
								//	bmin[k] = std::min(v[0][k], bmin[k]);
								//	bmin[k] = std::min(v[1][k], bmin[k]);
								//	bmin[k] = std::min(v[2][k], bmin[k]);
								//	bmax[k] = std::max(v[0][k], bmax[k]);
								//	bmax[k] = std::max(v[1][k], bmax[k]);
								//	bmax[k] = std::max(v[2][k], bmax[k]);
				}

				float n[3][3];
				if (attrib.normals.size() > 0) {
					int f0 = idx0.normal_index;
					int f1 = idx1.normal_index;
					int f2 = idx2.normal_index;
					assert(f0 >= 0);
					assert(f1 >= 0);
					assert(f2 >= 0);
					for (int k = 0; k < 3; k++) {
						n[0][k] = attrib.normals[3 * f0 + k];
						n[1][k] = attrib.normals[3 * f1 + k];
						n[2][k] = attrib.normals[3 * f2 + k];
					}
				}
				else {
					// compute geometric normal
					CalcNormal(n[0], v[0], v[1], v[2]);
					n[1][0] = n[0][0];
					n[1][1] = n[0][1];
					n[1][2] = n[0][2];
					n[2][0] = n[0][0];
					n[2][1] = n[0][1];
					n[2][2] = n[0][2];
				}

				for (int k = 0; k < 3; k++) {
					vb.push_back(v[k][0]);
					vb.push_back(v[k][1]);
					vb.push_back(v[k][2]);

					//	printf("v1 v2 v3 =%f  %f  %f\n", v[k][0], v[k][1], v[k][2]);

					vb.push_back(n[k][0]);
					vb.push_back(n[k][1]);
					vb.push_back(n[k][2]);
					// Combine normal and diffuse to get color.
					float normal_factor = 0.2;
					float diffuse_factor = 1 - normal_factor;
					float c[3] = {
						n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
						n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
						n[k][2] * normal_factor + diffuse[2] * diffuse_factor
					};
					float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
					if (len2 > 0.0f) {
						float len = sqrtf(len2);

						c[0] /= len;
						c[1] /= len;
						c[2] /= len;
					}
					vb.push_back(c[0] * 0.5 + 0.5);
					vb.push_back(c[1] * 0.5 + 0.5);
					vb.push_back(c[2] * 0.5 + 0.5);

					vb.push_back(tc[k][0]);
					vb.push_back(tc[k][1]);
				}
			}

			o.vb = 0;
			o.numTriangles = 0;


			if (shapes[s].mesh.material_ids.size() > 0 && shapes[s].mesh.material_ids.size() > s)
			{
				// Base case
				o.material_id = shapes[s].mesh.material_ids[s];
			}
			else
			{
				o.material_id = materials.size() - 1;    // = ID for default material.
			}


			if (vb.size() > 0)
			{
				glGenBuffers(1, &o.vb);
				glBindBuffer(GL_ARRAY_BUFFER, o.vb);
				glBufferData(GL_ARRAY_BUFFER,
					vb.size() * sizeof(float),
					&vb.at(0),
					GL_STATIC_DRAW);

				o.numTriangles = vb.size() / (3 + 3 + 3 + 2);    // note we have vertices, normals, colour, and uv, we proably won't use normals and colour yet

				printf("shape[%d] # of triangles = %d\n",
					static_cast<int>(s),
					o.numTriangles);

				GLsizei size;
				glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
				printf("VB is reported as %d \n", size);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			drawObjects->push_back(o);
		}
	}
	return true;
}

void ObjectModel::CalcNormal(float N[3], float v0[3], float v1[3], float v2[3])
{
	float v10[3];
	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	float v20[3];
	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	N[0] = v20[1] * v10[2] - v20[2] * v10[1];
	N[1] = v20[2] * v10[0] - v20[0] * v10[2];
	N[2] = v20[0] * v10[1] - v20[1] * v10[0];

	float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
	if (len2 > 0.0f)
	{
		float len = sqrtf(len2);

		N[0] /= len;
		N[1] /= len;
	}
}

// set up an already established set of vertices
bool ObjectModel::LoadModel(GLvoid* a_Vertices)
{
	this->Vertices = a_Vertices;
	return true;
}

void ObjectModel::ChangeModule(GeometryModule* a_Module)
{
	m_CurrentModule_ = a_Module;
}

void ObjectModel::ChangeMaterial(Material* a_Material)
{
	m_CurrentMaterial_ = a_Material;
}

void ObjectModel::SetModelMatrix()
{
	//set the matrices we use to I	
	mTranslationMatrix = glm::mat4(1.0f);
	mRotationMatrix = glm::mat4(1.0f);
	mScaleMatrix = glm::mat4(1.0f);

	// set scale matrix
	mScaleMatrix = glm::scale(mScaleMatrix, Scales);  // nice easy function
	// set Rotation Matrix	
	MakeRotationMatrix();  // this is a bit complex so go do them
	// set translation
	mTranslationMatrix = glm::translate(mTranslationMatrix, WorldPosition);  //use the glm translate

	// now make the Model for use in render
	ModelMatrix = mTranslationMatrix * mRotationMatrix * mScaleMatrix;
}

void ObjectModel::MakeTranslationMatrix()
{
	// this won't work
	mTranslationMatrix = glm::mat4(1.0f);
	mTranslationMatrix[3][0] = WorldPosition.x;
	mTranslationMatrix[3][1] = WorldPosition.y;
	mTranslationMatrix[3][2] = WorldPosition.z;
}

void ObjectModel::MakeRotationMatrix()
{
	// set rotationx
	RotationMatrixX = glm::mat4(1.0f);
	RotationMatrixX[1][1] = cosf(Rotations.x);
	RotationMatrixX[2][1] = -sinf(Rotations.x);
	RotationMatrixX[1][2] = sinf(Rotations.x);
	RotationMatrixX[2][2] = cosf(Rotations.x);
	//set rotationy
	RotationMatrixY = glm::mat4(1.0f);
	RotationMatrixY[0][0] = cosf(Rotations.y);
	RotationMatrixY[2][0] = sinf(Rotations.y);
	RotationMatrixY[0][2] = -sinf(Rotations.y);
	RotationMatrixY[2][2] = cosf(Rotations.y);
	//set rotationz
	RotationMatrixZ = glm::mat4(1.0f);
	RotationMatrixZ[0][0] = cosf(Rotations.z);
	RotationMatrixZ[1][0] = -sinf(Rotations.z);
	RotationMatrixZ[0][1] = sinf(Rotations.z);
	RotationMatrixZ[1][1] = cosf(Rotations.z);

	// now the combined rotation	
	mRotationMatrix = RotationMatrixX * RotationMatrixY * RotationMatrixZ;
}

// simply create the model matrix, assumes the translations and others have been set
void ObjectModel::MakeModelMatrix()
{
	ModelMatrix = mTranslationMatrix * mRotationMatrix * mScaleMatrix;
}

// return the position info from worldposition
glm::vec3 ObjectModel::GetPosition()
{
	return WorldPosition;
}

//set the word position
void ObjectModel::SetPosition(glm::vec3* a_Pos)
{
	WorldPosition.x = a_Pos->x;
	WorldPosition.y = a_Pos->y;
	WorldPosition.z = a_Pos->z;
}

//set the world position
void ObjectModel::SetPosition(glm::vec3 a_Pos)
{
	WorldPosition = a_Pos;
}

glm::vec3 ObjectModel::GetRotations()
{
	return glm::vec3();
}

void ObjectModel::SetRotations(glm::vec3 a_RotationAngles)
{
	glm::mat4 identity(1);
	RotationMatrixX = glm::rotate(identity, a_RotationAngles.x, glm::vec3(1, 0, 0));
	RotationMatrixY = glm::rotate(identity, a_RotationAngles.y, glm::vec3(0, 1, 0));
	RotationMatrixZ = glm::rotate(identity, a_RotationAngles.z, glm::vec3(0, 0, 1));
	mRotationMatrix = RotationMatrixX * RotationMatrixY * RotationMatrixZ;
}

glm::vec3 ObjectModel::LinearInterplation(glm::vec3 a, glm::vec3 b, float t)
{
	return a + t * (b - a);
}

/// <summary>
/// Does a simple linear interpolation between two points at a given time (t).
/// Taken from https://graphicscompendium.com/opengl/22-interpolation
/// </summary>
/// <param name="point_a"></param>
/// <param name="point_b"></param>
/// <param name="t"></param>
/// <returns></returns>
glm::vec3 ObjectModel::LerpBetween(glm::vec3 point_a, glm::vec3 point_b, float t)
{
	return point_a * (1.f - t) + point_b * t;
}

/// <summary>
/// Function for quadratic interpolation between a set of points taken from
/// https://graphicscompendium.com/opengl/22-interpolation
/// </summary>
/// <param name="point_a"></param>
/// <param name="point_b"></param>
/// <param name="point_c"></param>
/// <param name="point_d"></param>
/// <param name="time"></param>
/// <returns></returns>
glm::vec3 ObjectModel::QuadraticInterpolate(glm::vec3 point_a, glm::vec3 point_b, glm::vec3 point_c, glm::vec3 point_d, float time)
{
	glm::vec3 a = (3.0f * point_b - 3.0f * point_c + point_d - point_a) / 2.0f;
	glm::vec3 b = (2.0f * point_a - 5.0f * point_b + 4.0f * point_c - point_d) / 2.0f;
	glm::vec3 c = (point_c - point_a) / 2.0f;
	glm::vec3 d = point_b;

	float time_cube = time * time * time;
	float time_squared = time * time;

	return a * time_cube + b * time_squared + c * time + d;
}

glm::vec3 ObjectModel::CubicBezier(glm::vec3 point_a, glm::vec3 point_b, glm::vec3 point_c, glm::vec3 point_d,
	float time)
{
	float temp = (1 - time);
	float temp_square = temp * temp;
	float temp_cube = temp * temp * temp;

	float t_cube = time * time * time;
	float t_square = time * time;

	return temp_cube * point_a + 3 * time * temp_square * point_b + 3 * t_square * temp * point_c + t_cube * point_d;
}

void ObjectModel::SetXRotation(float a_x)
{
	Rotations.x = a_x;
}

void ObjectModel::SetYRotation(float a_y)
{
	Rotations.y = a_y;
}

void ObjectModel::SetZRotation(float a_z)
{
	Rotations.z = a_z;
}