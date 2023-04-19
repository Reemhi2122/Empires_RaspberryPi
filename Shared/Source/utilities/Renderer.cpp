#include "IMGUI/imgui.h"
#include "Headers/objects/Model.h"
#include "Headers/utilities/Renderer.h"
#include "Headers/utilities/Manager.h"
#include "Headers/Camera.h"
#include "Headers/utilities/Timer.h"
#include "Headers/MyFiles.h"
#include "Headers/objects/Planet.h"
#include "Headers/Empire.h"
#include "Headers/utilities/Shader.h"
#include "Headers/modules/CubeModule.h"
#include "Headers/objects/DebugModel.h"
#include "Headers/modules/DebugModule.h"
#include "Headers/materials/Material.h"
#include "Headers/utilities/Manager.h"
#include "Headers/utilities/DataStorage.h"
#include "Headers/GLDebugDrawer.h"
#include "PhysicsObj.h"

#include "Headers/modules/ModelModule.h"

#include "state_machines/MenuStateMachine.h"

Renderer::Renderer() {}
Renderer::~Renderer()
{
	delete m_MenuMachine_;
	delete m_Manager;
}

void Renderer::Init(ImGuiIO& a_IO, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation)
{
	DataStorage* storage;
	storage = storage->GetInstance();

	m_DebugCollision = false;
	CreatePhysicsWorld();
	m_DynamicPhysicsWorld->setGravity(btVector3(0, 0, 0));
	
	a_IO.DisplaySize.x = 1024;
	a_IO.DisplaySize.y = 764;

	storage->SetPhysicsWorld(m_DynamicPhysicsWorld);

	timer = new Timer();
	main_camera = new Camera();
	m_FileManager = new MyFiles();
	m_Manager = new Manager(true, a_FragmentShaderLocation, a_VertexShaderLocation);
	//m_MenuMachine_ = new MenuStateMachine();

	if (m_DebugCollision) {
		DebugCollision();
	}
}

void Renderer::CreatePhysicsWorld() {
	m_BroadPhase = new btDbvtBroadphase();
	m_CollisionConfiguration = new btDefaultCollisionConfiguration();
	m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
	m_ConstraintSolver = new btSequentialImpulseConstraintSolver;
	m_DynamicPhysicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_BroadPhase, m_ConstraintSolver, m_CollisionConfiguration);
}

void Renderer::Update(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset, float delta_time)
{
	main_camera->Update();
	deltatime = delta_time;

	if (m_DebugCollision) {
		debugModel->Update();
		debugModel->Draw(main_camera);
	}

	BulletPicking(a_MousePos);

	UpdateObjects();
	totaltime += deltatime;
	frames++;
	if (totaltime > 1.0f)
	{
		printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, frames / totaltime);  // uncomment to see frames
		totaltime -= 1.0f;
		frames = 0;
	}
}

void Renderer::UpdateImGui(ImGuiIO& a_IO, float a_DeltaTime, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput)
{
	static float a = 0.0f;
	static char b;
	
	static float c = 0.0f;
	static char d;

	//IO for IMGUI
	a_IO.DeltaTime = a_DeltaTime;
	a_IO.MouseDown[0] = a_MousePressed;

	a_IO.MousePos = ImVec2(a_MousePos.x, a_MousePos.y);

	if (!a_IO.MouseDown[0] && a_CurInput != 0)
		a_IO.AddInputCharacter(a_CurInput);
}

void Renderer::UpdateObjects() const
{
	//instructs the manager to update
	m_Manager->Update();
	m_Manager->Draw(main_camera);
}

void Renderer::BulletPicking(glm::vec2 a_MousePos)
{
	glm::vec3 ScreenPos = main_camera->GetPosition();

#ifdef _RASPARM
	glm::vec2 ScreenSize = glm::vec2(1024, 768);
#else
	glm::vec2 ScreenSize = glm::vec2(800, 600);
#endif

	glm::vec2 MousePos = glm::vec2(((a_MousePos.x / ScreenSize.x - 0.5f) * 2.0f), ((a_MousePos.y / ScreenSize.y - 0.5f) * 2.0f));
	MousePos.x *= ScreenPos.z / (800.f / 600.f);
	MousePos.y *= ScreenPos.z / 1.83f;

	glm::vec3 out_origin = glm::vec3(ScreenPos.x + MousePos.x, ScreenPos.y + MousePos.y, ScreenPos.z);
	glm::vec3 out_end = glm::vec3(ScreenPos.x + MousePos.x, ScreenPos.y + MousePos.y, 0);

	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(out_origin.x, out_origin.y, out_origin.z),
		btVector3(out_end.x, out_end.y, out_end.z)
	);

	m_DynamicPhysicsWorld->rayTest(btVector3(out_origin.x, out_origin.y, out_origin.z), btVector3(out_end.x, out_end.y, out_end.z), RayCallback);
	if (RayCallback.hasHit() && ScreenPos.z > -100)
		m_planet = ((Planet*)RayCallback.m_collisionObject->getUserPointer());
	else
		m_planet = nullptr;
}

void Renderer::Input(KeyInput a_KeyPressed) const
{
	float scaler = -(main_camera->GetPosition().z / 10);

	switch (a_KeyPressed)
	{
	case KeyInput::W:
		main_camera->TransformPosition(scaler * glm::vec3(0, 2, 0));
		break;
	case KeyInput::A:
		main_camera->TransformPosition(scaler * glm::vec3(2, 0, 0));
		break;
	case KeyInput::S:
		main_camera->TransformPosition(scaler * glm::vec3(0, -2, 0));
		break;
	case KeyInput::D:
		main_camera->TransformPosition(scaler * glm::vec3(-2, 0, 0));
		break;
	case KeyInput::SPACE:
		main_camera->ResetTranform();
		break;
	case KeyInput::SCROLLUP:
		if (main_camera->GetPosition().z > -2) return;
		main_camera->TransformPosition(scaler * glm::vec3(0, 0, 2));
		break;
	case KeyInput::SCROLLDOWN:
		if (main_camera->GetPosition().z < -1800) return;
		main_camera->TransformPosition(scaler * glm::vec3(0, 0, -2));
		break;
	default:
		break;
	}
}

void Renderer::DebugCollision()
{
	m_DebugDrawer = new GLDebugDrawer();
	m_DebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_DynamicPhysicsWorld->setDebugDrawer(m_DebugDrawer);
	m_DynamicPhysicsWorld->debugDrawWorld();

#ifdef _RASPARM
	Shader* default_shader = new Shader("../../../HelloCubes/Resources/Shaders/debugshader.frag", "../../../HelloCubes/Resources/Shaders/debugshader.vert");
#else
	Shader* default_shader = new Shader("../Empires(Windows)/Resources/Shaders/debugshader.frag", "../Empires(Windows)/Resources/Shaders/debugshader.vert");
#endif

	Material* default_material = new Material(*default_shader);
	GeometryModule* default_module = new DebugModule(m_DebugDrawer->GetVertices());
	debugModel = new DebugModel(default_material, default_module, m_DebugDrawer->GetVertices()->size());
}

float Renderer::GetDeltaTime() const
{
	return deltatime;
}

Camera& Renderer::GetCamera() const
{
	return *main_camera;
}

Planet* Renderer::GetCurrentPlanet() const
{
	return m_planet;
}
