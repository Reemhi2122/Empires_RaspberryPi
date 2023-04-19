#include "PhysicsObj.h"

#ifdef _RASPARM
#include <bullet/btBulletDynamicsCommon.h>
#else
#include <btBulletDynamicsCommon.h>
#endif


PhysicsObj::PhysicsObj()
{}

PhysicsObj::PhysicsObj(const char* a_Name, float a_Mass, bool a_Convex, btVector3 a_Position, btQuaternion a_Rotation, btCollisionShape* obj)
{
	m_Shape = obj;
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(a_Rotation, a_Position));

    btScalar bodyMass = a_Mass;
    btVector3 bodyInertia;
    m_Shape->calculateLocalInertia(bodyMass, bodyInertia);

    btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, m_Shape, bodyInertia);

    bodyCI.m_restitution = 0.9f;
    bodyCI.m_friction = 0.0f;

    m_Body = new btRigidBody(bodyCI);

    //m_Body->setUserPointer();
    //m_Body->setLinearFactor(btVector3(1, 1, 0)); We won't use this, this removes movement on z axis
}

PhysicsObj::~PhysicsObj()
{}

glm::vec3 PhysicsObj::GetPosition()
{
    float x = m_Body->getWorldTransform().getOrigin().getX();
    float y = m_Body->getWorldTransform().getOrigin().getY();
    float z = m_Body->getWorldTransform().getOrigin().getZ();

    return glm::vec3(x, y, z);
}

btRigidBody* PhysicsObj::GetRigidBody()
{
	return m_Body;
}
