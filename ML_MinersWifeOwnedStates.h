#ifndef MINERSWIFE_OWNED_STATES_H
#define MINERSWIFE_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinersWifeOwnedStates.h
//
//  Desc:   All the states that can be assigned to the MinersWife class
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"

class MinersWife;

//----WifeGlobalState----
class WifesGlobalState : public State<MinersWife>
{  
private:
  
  WifesGlobalState(){}

  //copy ctor and assignment should be private
  WifesGlobalState(const WifesGlobalState&);
  WifesGlobalState& operator=(const WifesGlobalState&);
 
public:

  //this is a singleton
  static WifesGlobalState* Instance();
  
  virtual void Enter(MinersWife* wife){}

  virtual void Execute(MinersWife* wife);

  virtual void Exit(MinersWife* wife){}

  virtual bool OnMessage(MinersWife* wife, const Telegram& msg);
};

//----DoHouseWork----
class DoHouseWork : public State<MinersWife>
{
private:

  DoHouseWork(){}
  
  //copy ctor and assignment should be private
  DoHouseWork(const DoHouseWork&);
  DoHouseWork& operator=(const DoHouseWork&);

public:

  //this is a singleton
  static DoHouseWork* Instance();
  
  virtual void Enter(MinersWife* wife);

  virtual void Execute(MinersWife* wife);

  virtual void Exit(MinersWife* wife);
  
  virtual bool OnMessage(MinersWife* wife, const Telegram& msg);

};


//----VisitBathRoom----
class VisitBathroom : public State<MinersWife>
{
private:
  
  VisitBathroom(){}

  //copy ctor and assignment should be private
  VisitBathroom(const VisitBathroom&);
  VisitBathroom& operator=(const VisitBathroom&);
 
public:

  //this is a singleton
  static VisitBathroom* Instance();
  
  virtual void Enter(MinersWife* wife);

  virtual void Execute(MinersWife* wife);

  virtual void Exit(MinersWife* wife);

  virtual bool OnMessage(MinersWife* wife, const Telegram& msg);

};


//----CookStew----
class CookStew : public State<MinersWife>
{
private:
  CookStew(){}

  //copy ctor and assignment should be private
  CookStew(const CookStew&);
  CookStew& operator=(const CookStew&);
 
public:

  //this is a singleton
  static CookStew* Instance();
  
  virtual void Enter(MinersWife* wife);

  virtual void Execute(MinersWife* wife);

  virtual void Exit(MinersWife* wife);

  virtual bool OnMessage(MinersWife* wife, const Telegram& msg);
};



////////////////////////////	숙제를 하자 숙제를		///////////////////////////
// 1. 바닥을 닦는다.
class CleanFloor : public State<MinersWife>
{
private:
	CleanFloor(){}

	CleanFloor(const CleanFloor&);
	CleanFloor& operator=(const CleanFloor&);
public:
	static CleanFloor* Instance();

	virtual void Enter(MinersWife* wife);
	virtual void Execute(MinersWife* wife);
	virtual void Exit(MinersWife* wife);

	virtual bool OnMessage(MinersWife* wife, const Telegram& msg);
};

// 2. 접시를 닦는다.
class Dishwash : public State<MinersWife>
{
private:
	Dishwash(){}

	Dishwash(const Dishwash&);
	Dishwash& operator=(const Dishwash&);
public:
	static Dishwash* Instance();

	virtual void Enter(MinersWife* wife);
	virtual void Execute(MinersWife* wife);
	virtual void Exit(MinersWife* wife);

	virtual bool OnMessage(MinersWife* wife, const Telegram& msg);
};

// 3. 이부자리를 정리한다.
class MakeBed : public State<MinersWife>
{
private:
	MakeBed(){}

	MakeBed(const MakeBed&);
	MakeBed& operator=(const MakeBed&);
public:
	static MakeBed* Instance();

	virtual void Enter(MinersWife* wife);
	virtual void Execute(MinersWife* wife);
	virtual void Exit(MinersWife* wife);

	virtual bool OnMessage(MinersWife* wife, const Telegram& msg);
};
#endif