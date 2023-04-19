#include "Headers/modules/ModelModule.h"

#include <cstdio>
#include "OGLGraphics.h"
#include "Headers/MyFiles.h"
#include <glm/glm.hpp>

ModelModule::ModelModule(const char* a_File)
{
	LoadModel(a_File);
	Init();
	m_VertexArrSize_ = m_Model.GetVertexData()->size();
	m_IndexArrSize_ = 36;
}

ModelModule::~ModelModule()
{
}

bool ModelModule::Init()
{
	// generating buffers & vertex arrays
	glGenVertexArrays(1, &m_Vao_);
	glGenBuffers(1, &m_Vbo_);
	//glGenBuffers(1, &m_Ebo_);

	glBindVertexArray(m_Vao_);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo_);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo_);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBufferData(GL_ARRAY_BUFFER, m_Model.GetVertexData()->size() * sizeof(float), &m_Model.GetVertexData()->at(0), GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexArrSize_ * sizeof(unsigned int), m_Indices_, GL_STATIC_DRAW);

	return true;
}

void ModelModule::LoadModel(const char* a_ModelFile)
{
	tinyobj::attrib_t attribute;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	std::string warn;

#ifdef _RASPARM
	const char* filepos = "../../../Shared/Resources/Objects/";
#else
	const char* filepos = "../Shared/Resources/Objects/";
#endif

	bool ret = tinyobj::LoadObj(&attribute, &shapes, &materials, &warn, &err, a_ModelFile, filepos);

	if (!err.empty())
	{
		char* cstr = &err[0u];   // printf needs chars
		printf("oh Bugger can't load this obj file %s file returns error %s \n", a_ModelFile, cstr);
	}

	if (!ret)
	{
		printf("The %s.obj seems to be corrupt or incorrectly formated\n", a_ModelFile);
	}

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				m_Model.GetVertexData()->push_back(attribute.vertices[3 * size_t(idx.vertex_index) + 0]);
				m_Model.GetVertexData()->push_back(attribute.vertices[3 * size_t(idx.vertex_index) + 1]);
				m_Model.GetVertexData()->push_back(attribute.vertices[3 * size_t(idx.vertex_index) + 2]);

				if (idx.normal_index >= 0) {
					m_Model.GetVertexData()->push_back(1);
					m_Model.GetVertexData()->push_back(1);
					m_Model.GetVertexData()->push_back(1);
				}
				else {
					glm::vec3 normalized = glm::vec3(attribute.vertices[3 * size_t(idx.vertex_index) + 0],
						attribute.vertices[3 * size_t(idx.vertex_index) + 1],
						attribute.vertices[3 * size_t(idx.vertex_index) + 2]);

					normalized = glm::normalize(normalized);

					m_Model.GetVertexData()->push_back(normalized.x);
					m_Model.GetVertexData()->push_back(normalized.y);
					m_Model.GetVertexData()->push_back(normalized.z);
				}

				if (idx.texcoord_index >= 0) {
					m_Model.GetVertexData()->push_back(attribute.texcoords[2 * size_t(idx.texcoord_index) + 0]);
					m_Model.GetVertexData()->push_back(attribute.texcoords[2 * size_t(idx.texcoord_index) + 1]);
				}
				else {
					printf("Sad");
				}
			}
			index_offset += fv;
		}
	}

	{
		if (materials.size() <= 0) return;
		MyFiles fileManager;
		std::string texture_filename = filepos + materials.at(0).diffuse_texname;
		char* imagePath = &texture_filename[0u];

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		int width, height;
		char* textureData = fileManager.Load(imagePath, &width, &height);

		if (textureData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		}
		else
		{
			printf("Failed to load: texture");
			GLubyte errorTexture[] = { 255, 0, 255, 255 };
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, errorTexture);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
		free(textureData);
	}
}