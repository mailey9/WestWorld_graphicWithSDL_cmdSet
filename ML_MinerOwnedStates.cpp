//
//		* ML_MinerOwnedStates.cpp
//
//		������ ���� ��Ʈ..
//			1) �������� ���� (�������̰� �밡����)
//			2) '����'���� ���°� �� ���� �ٲ�� �����,
//			  �켱������ �ξ� ������.
//				-- if �ָӴϰ� ��á�ٸ� -- ������ ����.
//				-- else if ���� �����ٸ� -- ������ ����.
//			  ������, �ָӴϰ� ������ && ���� ���� �� ���, ������ ������ ���� ������.
//

#include "ML_MinerOwnedStates.h"	//#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "ML_Miner.h"				//#include "Miner.h"
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
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
  static EnterMineAndDigForNugget instance;

  return &instance;
}

void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
			/*if ( pMiner->Location() != goldmine )
			{*/
	pMiner->ChangeLocation(goldmine_nugget);	// ���� �ڵ�. �̹� GoTo�� Exit���� �ٲ�� ������ ����� ����.

	// STEP 01 -- Emptying First.
	pMiner->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('dig')
	pMiner->m_pSetCommand->Add( new CommandStandMove(*pMiner, DIR_RIGHT) );
			//}
}

void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{  
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.

	//State<Miner>* pTest = VisitBankAndDepositGold::Instance();

	//	Enter()���� �߰��� '���'�� �����մϴ�. -- �� �� ����.
	pMiner->m_pSetCommand->Update();

	if ( pMiner->m_pSetCommand->IsEmpty() == true )
	{
		pMiner->AddToGoldCarried(1);

		pMiner->IncreaseFatigue();
		
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�ݵ���� ���´�.";

		//if enough gold mined, go and put it in the bank
		if ( pMiner->PocketsFull() )
		{
			pMiner->GetFSM()->ChangeState(
				GoTo< Miner, bank_account, VisitBankAndDepositGold::Instance >::Instance()
				);
		}
		else if ( pMiner->Thirsty() )
		{
			pMiner->GetFSM()->ChangeState(
				GoTo< Miner, saloon_bartender, QuenchThirst::Instance >::Instance()
				);
		}
		else
		{
			// Pocket is not full && Miner is not thirsty, so - adding a StandMove again.
			// '�ָӴϰ� �� ������ �ʾҰ�', '���� �������� ������', ���� ���ĵ��.
			pMiner->m_pSetCommand->Add( new CommandStandMove(*pMiner, DIR_RIGHT) );
		}
	}
}

void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
		<< "���� �ָӴϿ� �Ƹ��ٿ� �ݵ���� �Ѱ��� ä��� �ݱ��� ������.";
}

bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
  static VisitBankAndDepositGold instance;

  return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{  
  //on entry the miner makes sure he is located at the bank
			/*if ( pMiner->Location() != bank )
			{*/
	//cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�������� ����. ��, ����";

	pMiner->ChangeLocation(bank_account);

	// STEP 01 -- Emptying First.
	pMiner->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('�ñ��')
	pMiner->m_pSetCommand->Add( new CommandStandMove(*pMiner, DIR_UP) );
			//}
}

void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
	pMiner->m_pSetCommand->Update();

	if ( pMiner->m_pSetCommand->IsEmpty() == true )
	{
		//deposit the gold
		pMiner->AddToWealth(pMiner->GoldCarried());

		pMiner->SetGoldCarried(0);

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "���� �ñ��. �� ���෮�� ���� " << pMiner->Wealth() << " �̴�.";

		//wealthy enough to have a well earned rest?
		if ( pMiner->Wealth() >= ComfortLevel )
		{
			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
				<< "����! ���� ����� ���ڰ� �Ǿ���. ���� �Ϳ��� �Ƴ����Է� ���ư���.";

			//pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
			pMiner->GetFSM()->ChangeState(
				GoTo< Miner, shack, GoHomeAndSleepTilRested::Instance >::Instance()
				);
		}

		//otherwise get more gold
		else
		{
			//pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
			pMiner->GetFSM()->ChangeState(
				GoTo< Miner, goldmine_nugget, EnterMineAndDigForNugget::Instance >::Instance()
				);
		}
	}

}

void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ������.";
}

bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
  static GoHomeAndSleepTilRested instance;

  return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
	//if ( pMiner->Location() != shack )
	//{
	//cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ �ɾ��.";

	pMiner->ChangeLocation(shack); 

	//let the wife know I'm home
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
								pMiner->ID(),        //ID of sender
								ent_Elsa,            //ID of recipient
								Msg_HiHoneyImHome,   //the message
								NO_ADDITIONAL_INFO);  

	pMiner->m_pSetCommand->Empty();
	pMiner->m_pSetCommand->Add( new CommandStandMove(*pMiner, DIR_UP) );
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{
	pMiner->m_pSetCommand->Update();

	if ( pMiner->m_pSetCommand->IsEmpty() == true )
	{
			pMiner->GetFSM()->ChangeState(
				GoTo< Miner, shack_bed, SleepOnBedTilRested::Instance >::Instance()
				);


		////if miner is not fatigued start to dig for nuggets again.
		//if ( !pMiner->Fatigued() )
		//{
		//	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
		//		<< "���� ȯ������ �����̾�����! ���� �� ĳ�� �� �ð��̴�.";

		//	//pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
		//	pMiner->GetFSM()->ChangeState(
		//		GoTo< Miner, goldmine_nugget, EnterMineAndDigForNugget::Instance >::Instance()
		//		);
		//}
		//else 
		//{
		//	//sleep
		//	pMiner->DecreaseFatigue();

		//	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "����... ";

		//	// ���� �ȳ������� �߰�.
		//	pMiner->m_pSetCommand->Add( new CommandStandMove(*pMiner, DIR_DOWN) );
		//}
	}
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{ 
}

bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
	return false;
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
  static QuenchThirst instance;

  return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
	//if ( pMiner->Location() != saloon )
	//{    
	pMiner->ChangeLocation(saloon_bartender);

	pMiner->m_pSetCommand->Empty();
	pMiner->m_pSetCommand->Add( new CommandStandMove(*pMiner, DIR_UP) );
	//cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ������! �������� �ɾ��.";
	//}
}

void QuenchThirst::Execute(Miner* pMiner)
{
	pMiner->m_pSetCommand->Update();

	if ( pMiner->m_pSetCommand->IsEmpty() == true )
	{
		pMiner->BuyAndDrinkAWhiskey();

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� Ȧ¦Ȧ¦ ���ñ⿡ ���� ���̱�.";

		//pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());  
		pMiner->GetFSM()->ChangeState(
					GoTo< Miner, goldmine_nugget, EnterMineAndDigForNugget::Instance >::Instance()
									);
	}
}

void QuenchThirst::Exit(Miner* pMiner)
{ 
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ������, ��е� ����.";
}

bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
  static EatStew instance;

  return &instance;
}

void EatStew::Enter(Miner* pMiner)
{
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ���� ������, Elsa";

	pMiner->ChangeLocation(shack_table);

	pMiner->m_pSetCommand->Empty();
	pMiner->m_pSetCommand->Add( new CommandStandMove(*pMiner, DIR_LEFT) );
}

void EatStew::Execute(Miner* pMiner)
{
	pMiner->m_pSetCommand->Update();

	if ( pMiner->m_pSetCommand->IsEmpty() == true )
	{
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ���� ����!";

		//pMiner->GetFSM()->RevertToPreviousState();

		pMiner->GetFSM()->ChangeState(
					GoTo< Miner, shack_bed, SleepOnBedTilRested::Instance >::Instance()
								);
	}
}

void EatStew::Exit(Miner* pMiner)
{ 
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ����. �ϴ� ���� �ٽ� �ϴ� ���� ���ڱ�.";
}

bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}




///////////////////////////////////////////
SleepOnBedTilRested* SleepOnBedTilRested::Instance()
{
	static SleepOnBedTilRested instance;

	return &instance;
}

void SleepOnBedTilRested::Enter(Miner* pMiner)
{
	pMiner->m_pSetCommand->Add( new CommandStandMove(*pMiner, DIR_DOWN) );
}

void SleepOnBedTilRested::Execute(Miner* pMiner)
{
	pMiner->m_pSetCommand->Update();

	if ( pMiner->m_pSetCommand->IsEmpty() == true )
	{
		//if miner is not fatigued start to dig for nuggets again.
		if ( !pMiner->Fatigued() )
		{
			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
				<< "���� ȯ������ �����̾�����! ���� �� ĳ�� �� �ð��̴�.";

			//pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
			pMiner->GetFSM()->ChangeState(
				GoTo< Miner, goldmine_nugget, EnterMineAndDigForNugget::Instance >::Instance()
				);
		}
		else 
		{
			//sleep
			pMiner->DecreaseFatigue();

			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "����... ";

			// ���� �ȳ������� �߰�.
			pMiner->m_pSetCommand->Add( new CommandStandMove(*pMiner, DIR_DOWN) );
		}
	}
}

void SleepOnBedTilRested::Exit(Miner* pMiner)
{ 
}

bool SleepOnBedTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	switch(msg.Msg)
	{
	case Msg_StewReady:

		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) 
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(15); //SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMiner->ID()) 
			<< ": ���ƿ� ����, �� �� ������!";

		//pMiner->GetFSM()->ChangeState(EatStew::Instance());
		pMiner->GetFSM()->ChangeState(
				GoTo< Miner, shack_table, EatStew::Instance >::Instance()
				);

		return true;
	}//end switch

	return false; //send message to global message handler
}





///////////////////////////////////////////
DeliverTissue* DeliverTissue::Instance()
{
	static DeliverTissue instance;

	return &instance;
}

void DeliverTissue::Enter(Miner* pMiner)
{
	pMiner->m_pSetCommand->Add( new CommandStandMove(*pMiner, DIR_RIGHT) );
}

void DeliverTissue::Execute(Miner* pMiner)
{
	pMiner->m_pSetCommand->Update();

	if ( pMiner->m_pSetCommand->IsEmpty() == true )
	{
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
				<< "��! ���� ���� ������ �ٽ� ���̳� �Ϸ� ���߰ڴ�.";

		pMiner->GetFSM()->ChangeState(
				GoTo< Miner, goldmine_nugget, EnterMineAndDigForNugget::Instance >::Instance()
				);
	}
}

void DeliverTissue::Exit(Miner* pMiner)
{ 
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,			//time delay
							pMiner->ID(),			//sender ID
							ent_Elsa,			//receiver ID
							Msg_TissueDelivered,		//msg
							NO_ADDITIONAL_INFO);
}

bool DeliverTissue::OnMessage(Miner* pMiner, const Telegram& msg)
{
	return false;
}





///////////////////////////////////////////
MinerGlobalState* MinerGlobalState::Instance()
{
	static MinerGlobalState instance;

	return &instance;
}

void MinerGlobalState::Enter(Miner* pMiner)
{

}

void MinerGlobalState::Execute(Miner* pMiner)
{

}

void MinerGlobalState::Exit(Miner* pMiner)
{

}

bool MinerGlobalState::OnMessage(Miner* pMiner, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	switch(msg.Msg)
	{
	case Msg_HoneyINeedTissue:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) << " at time: " 
		<< Clock->GetCurrentTime();

		SetTextColor(15); //SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << 
			": ���尩�ϴ�!";

		//wife->GetFSM()->ChangeState(CookStew::Instance());

		// ���� ��ǥ ����
		ML_POSMat	minerPosMat			= pMiner->GetMatrixPosition();
		ML_POSPixel	minerPosPixelFixer	= { minerPosMat.col*ML_IMAGE_FRAME_SIZE, minerPosMat.row*ML_IMAGE_FRAME_SIZE };

		pMiner->SetPixelPosition( minerPosPixelFixer );

		// ���� ����
		pMiner->GetFSM()->ChangeState(
			GoTo<Miner, shack, DeliverTissue::Instance>::Instance()
			);
	}

	return true;

	}//end switch




	return false;
}