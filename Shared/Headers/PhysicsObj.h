#pragma once
#include <glm/glm.hpp>

class btRigidBody;
class btCollisionShape;
class btQuaternion;
class btVector3;

class PhysicsObj
{
public:
	PhysicsObj();
	PhysicsObj(const char* a_Name, float a_Mass, bool a_Convex, btVector3 a_Position, btQuaternion a_Rotation, btCollisionShape* obj);
	~PhysicsObj();

	glm::vec3 GetPosition();
	btRigidBody* GetRigidBody();

private:
	btCollisionShape* m_Shape;
	btRigidBody* m_Body;
	
	int m_Tag;
};
