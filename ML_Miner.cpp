//
//		* ML_Miner.cpp
//
//		������ ���� ��Ʈ..
//			1) Update() ���� �����ӿ� idx�� �ѱ⵵�� ����
//			  AnimStateMachine ������ ������ ���� �� ��. -- wife�� ����.
//

#include "ML_Miner.h"	//#include "Miner.h"

bool Miner::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void Miner::Update()
{
  //SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
	SetTextColor(15);		// BRIGHT-WHITE|BLACK
  m_iThirst += 1;
  
  m_pStateMachine->Update();
  //m_pSetCommand->Update();
  //m_idxFrame = (m_idxFrame + 1) % 3;	//
}



void Miner::AddToGoldCarried(const int val)
{
  m_iGoldCarried += val;

  if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Miner::AddToWealth(const int val)
{
  m_iMoneyInBank += val;

  if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

bool Miner::Thirsty()const
{
  if (m_iThirst >= ThirstLevel){return true;}

  return false;
}

bool Miner::Fatigued()const
{
  if (m_iFatigue > TirednessThreshold)
  {
    return true;
  }

  return false;
}
