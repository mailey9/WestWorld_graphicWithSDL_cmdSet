#ifndef __ML_ROB_THE_DRUNKEN_MINER_H__
#define __ML_ROB_THE_DRUNKEN_MINER_H__

#include <string>
#include <cassert>
#include <iostream>

#include "ML_BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "ML_RobOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State;

struct Telegram;

const int RobComfortLevel = 5;
const int RobMaxNuggets = 1;
const int RobTirednessThreshold = 5;

class Rob : public BaseGameEntity
{
private:
	StateMachine<Rob>*	  m_pStateMachine;

	location_type         m_Location;

	int                   m_iGoldCarried;
	int                   m_iMoneyInBank;
	int                   m_iFatigue;

	bool				  m_isDrunken;
public:

	Rob(int id, ML_POSMat posMat) : m_Location(rob_shack_bed),
									m_iGoldCarried(0),
									m_iMoneyInBank(0),
									m_iFatigue(0),
									BaseGameEntity(id, posMat)
	{
		m_pStateMachine = new StateMachine<Rob>(this);

		m_pStateMachine->SetCurrentState(RobSleepOnBedTilRested::Instance());
	}

	~Rob(){ delete m_pStateMachine; }

	//this must be implemented
	void Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);


	StateMachine<Rob>* GetFSM()const{ return m_pStateMachine; }



	//-------------------------------------------------------------accessors
	location_type Location()const{ return m_Location; }
	void          ChangeLocation(location_type loc){ m_Location = loc; }

	int           GoldCarried()const{ return m_iGoldCarried; }
	void          SetGoldCarried(int val){ m_iGoldCarried = val; }
	void          AddToGoldCarried(int val);
	bool          PocketsFull()const{ return m_iGoldCarried >= RobMaxNuggets; }

	bool          Fatigued()const;
	void          DecreaseFatigue(){ m_iFatigue -= 1; }
	void          IncreaseFatigue(){ m_iFatigue += 1; }

	int           Wealth()const{ return m_iMoneyInBank; }
	void          SetWealth(int val){ m_iMoneyInBank = val; }
	void          AddToWealth(int val);

	void          BuyAndDrinkAWhiskey(){ m_iMoneyInBank -= 5; }

	//
	bool IsDrunken() const
	{
		return m_isDrunken;
	}
	void SetDrunken(bool arg)
	{
		m_isDrunken = arg;
	}

};

#endif	// __ML_ROB_THE_DRUNKEN_MINER_H__