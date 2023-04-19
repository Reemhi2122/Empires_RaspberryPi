#include "Headers/EmpireAI.h"
#include "Headers/objects/Planet.h"
#include "Headers/utilities/DataStorage.h"
#include "Headers/utilities/Timer.h"

EmpireAI::EmpireAI(Empire* a_Empire)
{
	m_Empire = a_Empire;
	m_State = AIState::Neutral;
	m_DataStorage = m_DataStorage->GetInstance();

	AITimer = new Timer();
	AITimer->StartCountDown(std::chrono::seconds(5));
}

EmpireAI::~EmpireAI()
{}

void EmpireAI::UpdateAI() {
	if (m_Empire->IsDefeat()) return;
	if (AITimer->CheckCountDown()) {
		m_State = UpdateEmpireState();

		switch (m_State)
		{
		case AIState::Aggressive:
			AggressiveUpdate();
			break;
		case AIState::Neutral:
			NeutralUpdate();
			break;
		case AIState::Defensive:
			DefensiveUpdate();
			break;
		default:
			std::cout << "EmpireAI " << m_Empire->GetName().c_str() << " has no state." << std::endl;
			break;
		}
	}
}

void EmpireAI::AggressiveUpdate()
{
	printf("Aggressive state \n");
	CheckAddFighters(FighterBuyStrat::BuyHighest);
	UpdateBuyShip(ShipClass::ECarrierVessel);
	UpdateSendShip(ShipClass::ECarrierVessel);
	UpdateSendShip(ShipClass::EDiscoveryVessel);
}

void EmpireAI::NeutralUpdate()
{
	if (m_Empire->GetSupplyVessels()->size() > 0) {
		if (m_Empire->GetCarrierVessel()->size() > 0)
			UpdateBuyShip(ShipClass::EDiscoveryVessel);

		UpdateBuyShip(ShipClass::ECarrierVessel);
	}

	UpdateBuyShip(ShipClass::ESupplyVessel);
	UpdateSendShip(ShipClass::EDiscoveryVessel);
	CheckAddFighters(FighterBuyStrat::BuyLowestHighest);
}

void EmpireAI::DefensiveUpdate()
{
	UpdateSendShip(ShipClass::EDiscoveryVessel);

	if (m_Planets->size() > 2)
		CheckAddFighters(FighterBuyStrat::BuyHighest);
	else
		CheckAddFighters(FighterBuyStrat::BuyLowest);
}

void EmpireAI::UpdateBuyShip(ShipClass a_ShipClass)
{
	Vessel* m_PossibleVessel;
	switch (a_ShipClass)
	{
	case EDiscoveryVessel:
		if (m_Empire->GetDiscoveryVessels()->size() >= 3) return;
		m_PossibleVessel = m_Empire->GetFreeDiscoveryVessel();
		if (m_PossibleVessel == nullptr)
			if (m_Credits > m_DataStorage->GetDiscoveryVesselCost())
				m_Empire->CreateShip(ShipClass::EDiscoveryVessel);
		break;
	case ECarrierVessel:
		if (m_Empire->GetDiscoveryVessels()->size() >= 3) return;
		m_PossibleVessel = m_Empire->GetFreeCarrierVessel();
		if (m_Credits > m_DataStorage->GetCarrierVesselCost())
			if (m_PossibleVessel == nullptr)
				m_Empire->CreateShip(ShipClass::ECarrierVessel);
		break;
	case ESupplyVessel:
		if (m_Empire->GetSupplyVessels()->size() >= 1 || m_Credits < m_DataStorage->GetSupplyVesselCost()) return;
		m_Empire->CreateShip(ShipClass::ESupplyVessel);
		break;
	}
}

void EmpireAI::UpdateSendShip(ShipClass a_ShipClass)
{
	DiscoveryVessel* m_PossibleDiscoveryVessel = m_Empire->GetFreeDiscoveryVessel();
	switch (a_ShipClass)
	{
	case EDiscoveryVessel:
		if (m_PossibleDiscoveryVessel != nullptr) {
			std::vector<Planet*>* planets = m_Empire->GetPlanets();
			for (int i = 0; i < planets->size(); i++) {
				for (int x = 0; x < planets->at(i)->m_ClosePlanets_.size(); x++) {
					if (planets->at(i)->m_ClosePlanets_.at(x)->GetOwner() == nullptr) {
						bool picked = false;
						for (int z = 0; z < m_Empire->GetDiscoveryVessels()->size(); z++) {
							if (m_Empire->GetDiscoveryVessels()->at(z)->GetTargetPlanet() == planets->at(i)->m_ClosePlanets_.at(x))
								picked = true;
						}
						if (!picked) {
							m_PossibleDiscoveryVessel->SetTargetPlanet(m_Empire->GetPlanets()->at(i)->m_ClosePlanets_.at(x));
							return;
						}
					}
				}
			}
		}
		break;
	case ECarrierVessel:
		CarrierVessel* PossibleCarrierVessel = m_Empire->GetFreeCarrierVessel();
		if (PossibleCarrierVessel != nullptr) {
			Planet* myPlanet = nullptr;
			Empire* TargetEmpire = nullptr;
			int amount = 0;

			for (int i = 0; i < m_Planets->size(); i++)
			{
				if (m_Planets->at(i)->GetFighterAmount() > amount) {
					myPlanet = m_Planets->at(i);
					amount = m_Planets->at(i)->GetFighterAmount();
				}
			}

			do {
				int curRandom = rand() % m_DataStorage->max_num_empires;
				TargetEmpire = m_DataStorage->GetEmpire(curRandom);
			} while (TargetEmpire == m_Empire);
			if (TargetEmpire == nullptr || myPlanet == nullptr) return;

			amount = 999999999;
			Planet* TargetPlanet = nullptr;
			for (int i = 0; i < TargetEmpire->GetPlanets()->size(); i++)
			{
				if (TargetEmpire->GetPlanets()->at(i)->GetFighterAmount() < amount) {
					if (i == 0 && TargetEmpire->GetPlanets()->size() <= 1 || i > 0) {
						TargetPlanet = TargetEmpire->GetPlanets()->at(i);
						amount = TargetEmpire->GetPlanets()->at(i)->GetFighterAmount();
					}
				}
			}

			if(TargetPlanet != nullptr){
				for (int i = 0; i < m_Empire->GetCarrierVessel()->size(); i++) 
					if (m_Empire->GetCarrierVessel()->at(0)->GetTargetPlanet() == TargetPlanet) return;

				if (TargetPlanet->GetFighterAmount() < myPlanet->GetFighterAmount()) {
					m_Empire->SetSelectedPlanet(myPlanet);
					m_Empire->Attack(TargetPlanet, myPlanet->GetFighterAmount());
				}
			}
		}
		break;
	}
}

void EmpireAI::CheckAddFighters(FighterBuyStrat a_FighterBuyStrat)
{
	int AmountOfBuy = 5;
	if (m_Credits < AmountOfBuy * m_DataStorage->GetFighterShipCost()) return;
	Planet* curPlanet = nullptr;
	Planet* SecondPlanet = nullptr;

	switch (a_FighterBuyStrat)
	{
	case FighterBuyStrat::BuyHighest:
		int curFighters;
		curPlanet = GetHighFighterPlanet();
		break;
	case FighterBuyStrat::BuyLowestHighest:
		if (m_Credits < (AmountOfBuy * 2) * m_DataStorage->GetFighterShipCost()) return;
		curPlanet = GetHighFighterPlanet();
		SecondPlanet = GetLowFighterPlanet();
		break;
	case FighterBuyStrat::BuyLowest:
		curPlanet = GetLowFighterPlanet();
		break;
	default:
		break;
	}

	if (a_FighterBuyStrat != FighterBuyStrat::BuyLowestHighest) {
		curPlanet->AddFighterAmount(5);
		m_Credits -= AmountOfBuy * m_DataStorage->GetFighterShipCost();
	}
	else {
		curPlanet->AddFighterAmount(5);
		SecondPlanet->AddFighterAmount(5);
		m_Credits -= (AmountOfBuy * 2) * m_DataStorage->GetFighterShipCost();
	}
}

Planet* EmpireAI::GetLowFighterPlanet()
{
	Planet* curPlanet = nullptr;
	int curFighters = 999999999;
	for (int i = 0; i < m_Planets->size(); i++) {
		if (m_Planets->at(i)->GetFighterAmount() < curFighters) {
			curFighters = m_Planets->at(i)->GetFighterAmount();
			curPlanet = m_Planets->at(i);
		}
	}
	return curPlanet;
}

Planet* EmpireAI::GetHighFighterPlanet()
{
	Planet* curPlanet = nullptr;
	int curFighters = 0;
	for (int i = 0; i < m_Planets->size(); i++) {
		if (m_Planets->at(i)->GetFighterAmount() > curFighters) {
			curFighters = m_Planets->at(i)->GetFighterAmount();
			curPlanet = m_Planets->at(i);
		}
	}
	return curPlanet;
}

AIState EmpireAI::UpdateEmpireState()
{
	m_Planets = m_Empire->GetPlanets();
	m_Credits = m_Empire->GetCredits();
	m_AmountOfPlanets = m_Planets->size();
	m_AmountOfCarrierVessels = m_Empire->GetCarrierVessel()->size();
	m_AmountOfSupplyVessels = m_Empire->GetSupplyVessels()->size();
	m_AmountOfDiscoveryVessels = m_Empire->GetDiscoveryVessels()->size();
	m_Fighters = 0;

	for (int i = 0; i < m_AmountOfPlanets; i++)
		m_Fighters += m_Planets->at(i)->GetFighterAmount();

	if (m_AmountOfPlanets < 5 || (m_Fighters / m_AmountOfPlanets) < 5)
		return AIState::Defensive;

	if (m_AmountOfPlanets > 15 || (m_Fighters / m_AmountOfPlanets) > 30)
		return AIState::Aggressive;

	return AIState::Neutral;
}