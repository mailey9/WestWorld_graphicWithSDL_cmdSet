#include "ML_RobOwnedStates.h"
#include "fsm/State.h"
#include "ML_Rob.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>

#include "ML_BFS.h"
#include "ML_Definitions.h"

#include "ML_BaseGameEntity.h"
#include "ML_TemplateStateGoTo.h"

using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
RobEnterMineAndDigForNugget* RobEnterMineAndDigForNugget::Instance()
{
	static RobEnterMineAndDigForNugget instance;

	return &instance;
}

void RobEnterMineAndDigForNugget::Enter(Rob* pRob)
{
	pRob->ChangeLocation(rob_goldmine_nugget);

	pRob->m_pSetCommand->Empty();
	pRob->m_pSetCommand->Add(new CommandStandMove(*pRob, DIR_RIGHT));
}

void RobEnterMineAndDigForNugget::Execute(Rob* pRob)
{
	pRob->m_pSetCommand->Update();

	if (pRob->m_pSetCommand->IsEmpty() == true)
	{
		pRob->AddToGoldCarried(1);
		pRob->IncreaseFatigue();

		cout << "\n" << GetNameOfEntity(pRob->ID()) << ": " << "�ݵ���� ���´�.";
		cout << "\n" << GetNameOfEntity(pRob->ID()) << ": " << "Ȳ�� ���⸦ �� ���� �϶�� ���� �ִ�.";

		if (pRob->PocketsFull())
		{
			pRob->GetFSM()->ChangeState(
				GoTo< Rob, rob_bank_account, RobVisitBankAndDepositGold::Instance >::Instance()
				);
		}
		else
		{
			pRob->m_pSetCommand->Add(new CommandStandMove(*pRob, DIR_RIGHT));
		}
	}
}

void RobEnterMineAndDigForNugget::Exit(Rob* pRob)
{
	cout << "\n" << GetNameOfEntity(pRob->ID()) << ": "
		<< "����! �ָӴϴ� �� �� �� ����.";
}

bool RobEnterMineAndDigForNugget::OnMessage(Rob* pRob, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

RobVisitBankAndDepositGold* RobVisitBankAndDepositGold::Instance()
{
	static RobVisitBankAndDepositGold instance;

	return &instance;
}

void RobVisitBankAndDepositGold::Enter(Rob* pRob)
{
	pRob->ChangeLocation(rob_bank_account);

	pRob->m_pSetCommand->Empty();
	pRob->m_pSetCommand->Add(new CommandStandMove(*pRob, DIR_UP));
}

void RobVisitBankAndDepositGold::Execute(Rob* pRob)
{
	pRob->m_pSetCommand->Update();

	if (pRob->m_pSetCommand->IsEmpty() == true)
	{
		pRob->AddToWealth(pRob->GoldCarried());
		pRob->SetGoldCarried(0);

		cout << "\n" << GetNameOfEntity(pRob->ID()) << ": "
			<< "���� �ñ��. �� ���෮�� ���� " << pRob->Wealth() << " �̴�.";

		if (pRob->Wealth() >= RobComfortLevel)
		{
			cout << "\n" << GetNameOfEntity(pRob->ID()) << ": "
				<< "��ȣ! ���� ����� ���ڰ� �Ǿ���. ���� ���� ������ ����.";

			pRob->GetFSM()->ChangeState(
				GoTo< Rob, rob_saloon_bartender, RobQuenchThirst::Instance, 4, 8 >::Instance()
				);
		}
		else
		{
			//pRob->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
			pRob->GetFSM()->ChangeState(
				GoTo< Rob, rob_goldmine_nugget, RobEnterMineAndDigForNugget::Instance >::Instance()
				);
		}
	}

}

void RobVisitBankAndDepositGold::Exit(Rob* pRob)
{
	cout << "\n" << GetNameOfEntity(pRob->ID()) << ": " << "������ ������.";
}

bool RobVisitBankAndDepositGold::OnMessage(Rob* pRob, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}


//------------------------------------------------------------------------QuenchThirst

RobQuenchThirst* RobQuenchThirst::Instance()
{
	static RobQuenchThirst instance;

	return &instance;
}

void RobQuenchThirst::Enter(Rob* pRob)
{
	pRob->ChangeLocation(rob_saloon_bartender);

	pRob->m_pSetCommand->Empty();
	pRob->m_pSetCommand->Add(new CommandStandMove(*pRob, DIR_UP));
}

void RobQuenchThirst::Execute(Rob* pRob)
{
	pRob->m_pSetCommand->Update();

	if (pRob->m_pSetCommand->IsEmpty() == true)
	{
		pRob->BuyAndDrinkAWhiskey();

		for (int num = 0; num < 10; ++num)
		{
			pRob->IncreaseFatigue();
		}

		cout << "\n" << GetNameOfEntity(pRob->ID()) << ": " << "�ų��ϰ� ���غ���!";

		pRob->GetFSM()->ChangeState(
			GoTo< Rob, rob_shack_bed, RobSleepOnBedTilRested::Instance, 16, 2 >::Instance()
			);
	}
}

void RobQuenchThirst::Exit(Rob* pRob)
{
	cout << "\n" << GetNameOfEntity(pRob->ID()) << ": " << "��..���Ѵ�.";
	pRob->SetDrunken(true);
}

bool RobQuenchThirst::OnMessage(Rob* pRob, const Telegram& msg)
{
	return false;
}

//------------------------------------------------------------------------EatStew





///////////////////////////////////////////
RobSleepOnBedTilRested* RobSleepOnBedTilRested::Instance()
{
	static RobSleepOnBedTilRested instance;

	return &instance;
}

void RobSleepOnBedTilRested::Enter(Rob* pRob)
{
	pRob->m_pSetCommand->Add(new CommandStandMove(*pRob, DIR_DOWN));
	pRob->SetDrunken(false);
}

void RobSleepOnBedTilRested::Execute(Rob* pRob)
{
	pRob->m_pSetCommand->Update();

	if (pRob->m_pSetCommand->IsEmpty() == true)
	{
		if (!pRob->Fatigued())
		{
			cout << "\n" << GetNameOfEntity(pRob->ID()) << ": "
				<< "���� ���� ������!!";
			cout << "\n" << GetNameOfEntity(pRob->ID()) << ": "
				<< "����! �������� ���ؾ߰ڴ�!";

			pRob->GetFSM()->ChangeState(
				GoTo< Rob, rob_goldmine_nugget, RobEnterMineAndDigForNugget::Instance, 4, 8 >::Instance()
				);
		}
		else
		{
			pRob->DecreaseFatigue();
			cout << "\n" << GetNameOfEntity(pRob->ID()) << ": " << "�帣��... ���...";
			pRob->m_pSetCommand->Add(new CommandStandMove(*pRob, DIR_DOWN));
		}
	}
}

void RobSleepOnBedTilRested::Exit(Rob* pRob)
{
}

bool RobSleepOnBedTilRested::OnMessage(Rob* pRob, const Telegram& msg)
{
	return false;
}