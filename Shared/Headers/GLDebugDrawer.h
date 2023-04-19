#pragma once
#include "OGLGraphics.h"
#include <vector>

#ifdef _RASPARM
#include <bullet/btBulletDynamicsCommon.h>
#else
#include "../Empires(Windows)/PyBullet/src/btBulletDynamicsCommon.h"
#endif

class GLDebugDrawer : public btIDebugDraw
{
public:
    GLDebugDrawer();
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
    void reportErrorWarning(const char* warningString);
    void draw3dText(const btVector3& location, const char* textString);
    void setDebugMode(int debugMode);
    int getDebugMode() const { return m_debugMode; }
    
    std::vector<GLfloat>* GetVertices();

private:
    int m_debugMode;
    std::vector<GLfloat> vertices;
};