#include "Headers/GLDebugDrawer.h"

GLDebugDrawer::GLDebugDrawer()
{}

void GLDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	vertices.push_back(from.getX());
	vertices.push_back(from.getY());
	vertices.push_back(from.getZ());
	vertices.push_back(to.getX());
	vertices.push_back(to.getY());
	vertices.push_back(to.getZ());
}

void GLDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{}

void GLDebugDrawer::reportErrorWarning(const char* warningString)
{}

void GLDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{}

void GLDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

std::vector<GLfloat>* GLDebugDrawer::GetVertices()
{
	return &vertices;
}
