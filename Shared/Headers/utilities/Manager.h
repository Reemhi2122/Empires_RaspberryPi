#pragma once

class Generator;
class DataStorage;
class Camera;
class Timer;

class Manager
{
public:
	Manager(bool a_Graphicsinitialized, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation);
	~Manager();

	void Update() const;
	void Draw(Camera* a_Camera) const;

	DataStorage* GetStorage() const;
	
private:
	Camera* m_Camera_;
	Timer* m_PlanetTimer_;
	Timer* m_EmpireTimer_;
	Timer* m_SoiTimer_;
};
