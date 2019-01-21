//
//		* ML_MinersWife.cpp
//
//		������ ���� ��Ʈ..
//			1) Update() ���� �����ӿ� idx�� �ѱ⵵�� ����
//			  AnimStateMachine ������ ������ ���� �� ��. -- miner�� ����.
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