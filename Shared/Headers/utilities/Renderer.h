#pragma once
#include "glm/glm.hpp"
#include <vector>

#ifdef _RASPARM
#include <bullet/btBulletDynamicsCommon.h>
#else
#include "../Empires(Windows)/PyBullet/src/btBulletDynamicsCommon.h"
#endif

struct ImGuiIO;
class ObjectModel;
class Manager;
class Camera;
class MyFiles;
class Timer;
class Model;
class DebugModel;
class Empire;
class MenuStateMachine;
class GLDebugDrawer;
class CarrierVessel;
class Planet;

enum class KeyInput {
	W,
	A,
	S,
	D,
	SPACE,
	SCROLLUP,
	SCROLLDOWN,
	NONE
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init(ImGuiIO& a_IO, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation);
	void Update(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset, float delta_time);
	
	void CreatePhysicsWorld();
	void UpdateImGui(ImGuiIO& a_IO, float a_DeltaTime, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput);
	void UpdateObjects() const;
	void BulletPicking(glm::vec2 a_MousePos);

	void Input(KeyInput a_KeyPressed) const;
	void DebugCollision();

	float GetDeltaTime() const;

	Camera& GetCamera() const;

	Planet* GetCurrentPlanet() const;

private:
	Manager* m_Manager;
	MyFiles* m_FileManager;

	Camera* main_camera;
	Timer* timer;

	float deltatime = 0.3;
	float totaltime = 0.0f;
	unsigned int frames = 0;

	bool m_DebugCollision;

	btBroadphaseInterface* m_BroadPhase;
	btDefaultCollisionConfiguration* m_CollisionConfiguration;
	btCollisionDispatcher* m_Dispatcher;
	btSequentialImpulseConstraintSolver* m_ConstraintSolver;
	btDiscreteDynamicsWorld* m_DynamicPhysicsWorld;
	GLDebugDrawer* m_DebugDrawer;
	DebugModel* debugModel;

	MenuStateMachine* m_MenuMachine_;
	Planet* m_planet = nullptr;
};