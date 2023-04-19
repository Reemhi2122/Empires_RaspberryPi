#include "utilities/Timer.h"


#include <cassert>
#include <iostream>

void Timer::Reset()
{
	m_StartTime_ = std::chrono::system_clock::now();
}

float Timer::GetDeltaTime() const
{
	return m_DeltaTime_.count();
}

void Timer::SetTimeScale(const float a_Scale)
{
	m_Scale_ = a_Scale;
}

float Timer::GetTimeScale() const
{
	return m_Scale_;
}

void Timer::Stop()
{
	m_EndTime_ = std::chrono::system_clock::now();
	m_DeltaTime_ = m_EndTime_ - m_StartTime_;
}

	/// <summary>
	/// starts a countdown for the given duration (in sec)
	/// </summary>
	/// <param name="a_Length"></param>
void Timer::StartCountDown(std::chrono::seconds a_Length)
{
#ifdef DEBUG
	if (m_CountdownActive_)
	{
		std::cout <<
			"\nA countdown is already active. If another one is started, the data of \n the old timer will not work as intended anymore"
			<< std::endl;
	}
#endif
	
	m_CountdownStart_ = std::chrono::system_clock::now();
	m_CountdownActive_ = true;
	m_CountdownLength_ = a_Length;
}

void Timer::StopCountDown()
{
	m_CountdownActive_ = false;
}

	/// <summary>
	/// Checks the active countdown. If the current time point is
	/// </summary>
	/// <returns></returns>
bool Timer::CheckCountDown()
{
	assert(m_CountdownActive_);
	
	std::chrono::duration<double> duration = std::chrono::system_clock::now() - m_CountdownStart_;

	if (duration >= m_CountdownLength_)
	{
		ResetCountdown();
		return true;
	}
	return false;
}

void Timer::ResetCountdown()
{
		m_CountdownStart_ = std::chrono::system_clock::now();
}

Timer::Timer()
{
	Reset();
	m_Scale_ = 1.0f;
	m_DeltaTime_ = std::chrono::duration<float>(0.0f);
}

Timer::~Timer()
= default;
