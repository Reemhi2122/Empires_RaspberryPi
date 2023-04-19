#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	~Timer();

	void Reset();								// call to reset start time, generally: timer.Reset(), do whatever, timer.Stop() to measure time

	float GetDeltaTime() const;					// returns delta time (requires call to both reset && stop to function)

	void SetTimeScale(float a_Scale = 1.0f);	// incomplete, will be used to set timescale to ms, s or min
	float GetTimeScale() const;					

	void Stop();								// stops current timer event and calculates the events duration

	void StartCountDown(std::chrono::seconds a_Length);
	void StopCountDown();
	bool CheckCountDown();

private:
	void ResetCountdown();
	
	Timer* m_Instance_ = nullptr;
	
	std::chrono::system_clock::time_point m_StartTime_;
	std::chrono::system_clock::time_point m_EndTime_;
	std::chrono::duration<double> m_DeltaTime_;

	std::chrono::system_clock::time_point m_CountdownStart_;
	bool m_CountdownActive_;
	std::chrono::duration<double> m_CountdownLength_;

	float m_Scale_;
};



