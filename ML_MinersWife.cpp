//
//		* ML_MinersWife.cpp
//
//		숙제용 변경 노트..
//			1) Update() 에서 프레임용 idx를 넘기도록 변경
//			  AnimStateMachine 같은게 있으면 좋긴 할 듯. -- miner도 동일.
//

#include "ML_MinersWife.h"	//#include "MinersWife.h"

bool MinersWife::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void MinersWife::Update()
{
  //set text color to green
  //SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	SetTextColor(10);		// GREEN|BLACK
 
  m_pStateMachine->Update();
  //m_pSetCommand->Update();

  //m_idxFrame = (m_idxFrame + 1) % 3;	//
}