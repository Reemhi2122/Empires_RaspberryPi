#pragma once
#include <vector>
#include "Headers/Empire.h"

class Empire;
class Planet;
class DataStorage;
class Timer;

enum class AIState {
	Aggressive,
	Neutral,
	Defensive
};

enum class FighterBuyStrat {
	BuyHighest,
	BuyLowestHighest,
	BuyLowest
};

class EmpireAI
{
public:
	EmpireAI(Empire* a_Empire);
	~EmpireAI();

	void UpdateAI();

private:
	void AggressiveUpdate();
	void NeutralUpdate();
	void DefensiveUpdate();

	void UpdateBuyShip(ShipClass a_ShipClass);
	void UpdateSendShip(ShipClass a_ShipClass);
	void CheckAddFighters(FighterBuyStrat a_FighterBuyStrat);

	Planet* GetLowFighterPlanet();
	Planet* GetHighFighterPlanet();

	AIState UpdateEmpireState();

	std::vector<Planet*>* m_Planets;
	Empire* m_Empire;
	AIState m_State;

	DataStorage* m_DataStorage;

	unsigned int m_Credits;
	unsigned int m_AmountOfPlanets;
	unsigned int m_AmountOfCarrierVessels;
	unsigned int m_AmountOfSupplyVessels;
	unsigned int m_AmountOfDiscoveryVessels;
	unsigned int m_Fighters;

	Timer* AITimer;
};