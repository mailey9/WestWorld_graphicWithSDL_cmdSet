#ifndef __ML_ROB_OWNED_STATES_H__
#define __ML_ROB_OWNED_STATES_H__

#include "fsm/State.h"
#include "ML_Command.h"

class Rob;
struct Telegram;

//
//	RobEnterMineAndDigForNugget
//
class RobEnterMineAndDigForNugget : public State<Rob>
{
private:

	RobEnterMineAndDigForNugget(){}

	//copy ctor and assignment should be private
	RobEnterMineAndDigForNugget(const RobEnterMineAndDigForNugget&);
	RobEnterMineAndDigForNugget& operator=(const RobEnterMineAndDigForNugget&);

public:

	//this is a singleton
	static RobEnterMineAndDigForNugget* Instance();

	virtual void Enter(Rob* miner);

	virtual void Execute(Rob* miner);

	virtual void Exit(Rob* miner);

	virtual bool OnMessage(Rob* agent, const Telegram& msg);

};

//
//	RobVisitBankAndDepositGold
//
class RobVisitBankAndDepositGold : public State<Rob>
{
private:

	RobVisitBankAndDepositGold(){}

	//copy ctor and assignment should be private
	RobVisitBankAndDepositGold(const RobVisitBankAndDepositGold&);
	RobVisitBankAndDepositGold& operator=(const RobVisitBankAndDepositGold&);

public:

	//this is a singleton
	static RobVisitBankAndDepositGold* Instance();

	virtual void Enter(Rob* miner);

	virtual void Execute(Rob* miner);

	virtual void Exit(Rob* miner);

	virtual bool OnMessage(Rob* agent, const Telegram& msg);
};

//
//	RobQuenchThirst
//

class RobQuenchThirst : public State<Rob>
{
private:

	RobQuenchThirst(){}

	//copy ctor and assignment should be private
	RobQuenchThirst(const RobQuenchThirst&);
	RobQuenchThirst& operator=(const RobQuenchThirst&);

public:

	//this is a singleton
	static RobQuenchThirst* Instance();

	virtual void Enter(Rob* miner);

	virtual void Execute(Rob* miner);

	virtual void Exit(Rob* miner);

	virtual bool OnMessage(Rob* agent, const Telegram& msg);
};

//
//	RobSleepOnBedTilRested
//
class RobSleepOnBedTilRested : public State<Rob>
{
private:
	RobSleepOnBedTilRested(){}

	//copy ctor and assignment should be private
	RobSleepOnBedTilRested(const RobSleepOnBedTilRested&);
	RobSleepOnBedTilRested& operator=(const RobSleepOnBedTilRested&);

public:
	//this is a singleton
	static RobSleepOnBedTilRested* Instance();

	virtual void Enter(Rob* miner);

	virtual void Execute(Rob* miner);

	virtual void Exit(Rob* miner);

	virtual bool OnMessage(Rob* agent, const Telegram& msg);
};

#endif	// __ML_ROB_OWNED_STATES_H__