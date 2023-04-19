#include "Headers/modules/InstanceModelModule.h"

#include <cstdio>
#include "OGLGraphics.h"
#include "Headers/MyFiles.h"

InstanceModelModule::InstanceModelModule(const char* a_File, std::vector<glm::mat4>* a_Positions)
{
	LoadModel(a_File);
	Init();
	m_VertexArrSize_ = m_Model.GetVertexData()->size();
	m_IndexArrSize_ = 36;
	positions = a_Positions;
}

InstanceModelModule::~InstanceModelModule()
{
}

bool InstanceModelModule::Init()
{
	glGenVertexArrays(1, &m_Vao_);
	glGenBuffers(1, &m_Vbo_);
	glGenBuffers(1, &buffer);

	glBindVertexArray(m_Vao_);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo_);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBufferData(GL_ARRAY_BUFFER, m_Model.GetVertexData()->size() * sizeof(float), &m_Model.GetVertexData()->at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 8192 * sizeof(glm::mat4), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(0);
	return true;
}

void InstanceModelModule::LoadModel(const char* a_ModelFile)
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

	printf("Attrib data for %s is :-\n", a_ModelFile);
	printf("# vertices  : %d\n", (int)(attribute.vertices.size()) / 3);
	printf("# normals   : %d\n", (int)(attribute.normals.size()) / 3);
	printf("# texcoords : %d\n", (int)(attribute.texcoords.size()) / 2);
	printf("Other data  :-\n");

	printf("# materials : %d\n", (int)materials.size());
	printf("# shapes    : %d\n", (int)shapes.size());

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

				//// Check if `normal_index` is zero or positive. negative = no normal data
				//if (idx.normal_index >= 0) {
				//	m_Model.GetNormalData()->push_back(attribute.normals[3 * size_t(idx.normal_index) + 0]);
				//	m_Model.GetNormalData()->push_back(attribute.normals[3 * size_t(idx.normal_index) + 1]);
				//	m_Model.GetNormalData()->push_back(attribute.normals[3 * size_t(idx.normal_index) + 2]);
				//}

				if (idx.texcoord_index >= 0) {
					m_Model.GetVertexData()->push_back(attribute.texcoords[2 * size_t(idx.texcoord_index) + 0]);
					m_Model.GetVertexData()->push_back(attribute.texcoords[2 * size_t(idx.texcoord_index) + 1]);
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