#include "ML_Rob.h"

bool Rob::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void Rob::Update()
{
	//SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	SetTextColor(11);	// SKYBLUE|BLACK
	m_pStateMachine->Update();
}

void Rob::AddToGoldCarried(const int val)
{
	m_iGoldCarried += val;

	if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Rob::AddToWealth(const int val)
{
	m_iMoneyInBank += val;

	if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

bool Rob::Fatigued()const
{
	if (m_iFatigue > RobTirednessThreshold)
	{
		return true;
	}

	return false;
}