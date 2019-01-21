//
//		* ML_MinersWife.h
//
//		숙제용 변경 노트..
//			1) EntityManager.h의 변경에 의해 베이스클래스의 생성자 호출법,
//			  명시적 생성자에 생성변수를 추가. -- Miner도 동일.
//			2) 초기위치를 shack_sink로 변경.		//		초기위치와 다른 좌표문제가 발생해서..
//


#ifndef MINERSWIFE_H
#define MINERSWIFE_H
//------------------------------------------------------------------------
//
//  Name: MinersWife.h
//
//  Desc: class to implement Miner Bob's wife.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <string>

#include "fsm/State.h"
#include "ML_BaseGameEntity.h"			// #include "BaseGameEntity.h"
#include "Locations.h"
#include "ML_MinersWifeOwnedStates.h"	// #include "MinersWifeOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "ML_Miner.h"					// #include "Miner.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"

class MinersWife : public BaseGameEntity
{
private:

  //an instance of the state machine class
  StateMachine<MinersWife>* m_pStateMachine;

  location_type   m_Location;

  //is she presently cooking?
  bool            m_bCooking;


public:

  MinersWife(int id, ML_POSMat posMat) : m_Location(shack_sink),
										 m_bCooking(false),
										 BaseGameEntity(id, posMat)
                                        
  {
    //set up the state machine
    m_pStateMachine = new StateMachine<MinersWife>(this);

    m_pStateMachine->SetCurrentState(DoHouseWork::Instance());
    m_pStateMachine->SetGlobalState(WifesGlobalState::Instance());
  }

  ~MinersWife(){delete m_pStateMachine;}


  //this must be implemented
  void          Update();

  //so must this
  virtual bool  HandleMessage(const Telegram& msg);

  StateMachine<MinersWife>* GetFSM()const{return m_pStateMachine;}

  //----------------------------------------------------accessors
  location_type Location()const{return m_Location;}
  void          ChangeLocation(location_type loc){m_Location=loc;}

  bool          Cooking()const{return m_bCooking;}
  void          SetCooking(bool val){m_bCooking = val;}
   
};

#endif
