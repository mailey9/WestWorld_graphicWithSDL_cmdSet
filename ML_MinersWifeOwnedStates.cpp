//
//		* ML_MinersWifeOwnedStates.cpp
//
//		������ ���� ��Ʈ..
//			1) �������� ���� (�������̰� �밡����)
//			2) ���¸� �������� �߰����� ���������� 'ȭ��� ���� Ȯ��' �� ����� �������� �ʴ� ���� Ȯ��.
//			  �̸� ���ܽ��� ������, �޽�����ŭ�� ����� �ڵ��ϵ��� ����.
//			3) 'ȭ���'�� DoHouseWork �߿� Ȯ�������� ��.
//
//	<!>		*��Ʈ : 2)�� ���� ���� ���Ŀ� �� �������µ� ����� �ڵ��ϴ� FSM�� ����� ������ ����.
//

#include "ML_MinersWifeOwnedStates.h"	//#include "MinersWifeOwnedStates.h"
#include "ML_MinerOwnedStates.h"		//#include "MinerOwnedStates.h"
#include "ML_MinersWife.h"				//#include "MinersWife.h"
#include "Locations.h"
#include "Time/CrudeTimer.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
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

//-----------------------------------------------------------------------Global state
WifesGlobalState* WifesGlobalState::Instance()
{
  static WifesGlobalState instance;

  return &instance;
}

void WifesGlobalState::Execute(MinersWife* wife)
{
  //1 in 10 chance of needing the bathroom (provided she is not already
  //in the bathroom)
  /*if ( (RandFloat() < 0.1) && 
       !wife->GetFSM()->isInState(*VisitBathroom::Instance()) )
  {
    wife->GetFSM()->ChangeState(VisitBathroom::Instance());
  }*/

	// 1000���� 1 Ȯ����..
	//		'ȭ��� �湮����'�� �ƴϰ�,
	//		'ȭ��Ƿ� ���� �ִ� ����'�� �ƴ϶��.
	//			'ȭ��Ƿ� ���� �ִ� ����'�� �Ǿ��!
	//	*��Ʈ : �ڵ� ����.. �ϴ� ������!
	//			������ �����ѵ�..

	/*if ( ( RandFloat() < 0.001 )
		&& (wife->GetFSM()->isInState( *VisitBathroom::Instance() ) == false)
		&& (wife->GetFSM()->isInState( *GoTo<MinersWife, shack_bathroom, VisitBathroom::Instance>::Instance() ) == false)
		)
	{
		wife->GetFSM()->ChangeState(
			GoTo<MinersWife, shack_bathroom, VisitBathroom::Instance>::Instance()
			);
	}*/
}

bool WifesGlobalState::OnMessage(MinersWife* wife, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	switch(msg.Msg)
	{
	case Msg_HiHoneyImHome:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(wife->ID()) << " at time: " 
		<< Clock->GetCurrentTime();

		SetTextColor(10); //SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(wife->ID()) << 
			": ����, ���ִ� �ð�� ���� ����� �帱����.";

		//wife->GetFSM()->ChangeState(CookStew::Instance());

		// ���� ��ǥ ����
		ML_POSMat	wifePosMat			= wife->GetMatrixPosition();
		ML_POSPixel	wifePosPixelFixer	= { wifePosMat.col*ML_IMAGE_FRAME_SIZE, wifePosMat.row*ML_IMAGE_FRAME_SIZE };

		wife->SetPixelPosition( wifePosPixelFixer );

		// ���� ����
		wife->GetFSM()->ChangeState(
			GoTo<MinersWife, shack_sink, CookStew::Instance>::Instance()
			);
	}

	return true;

	}//end switch

	return false;
}

//-------------------------------------------------------------------------DoHouseWork
DoHouseWork* DoHouseWork::Instance()
{
  static DoHouseWork instance;

  return &instance;
}

void DoHouseWork::Enter(MinersWife* wife)
{
//	cout << "\n" << GetNameOfEntity(wife->ID()) << ": �������� ���� �� �ð��̱�!";
}

void DoHouseWork::Execute(MinersWife* wife)
{
	//	Ȯ�� �й�
	//		0~9 �� �̰�
	//		0-2 : �ٴ��� û���Ѵ� -- 30%
	//				���������� 0-2�� �õ带 �̾Ƽ�,
	//				�ٴ� 1-2-3 �� �� 33% Ȯ���� û���Ѵ�. -- �� �ٴ��� û���� Ȯ���� �� 10%
	//		3-5 : ���ø� �۴´� -- 30%
	//		6-8 : �̺��ڸ��� �����Ѵ� -- 30%
	//		 9  : ȭ����� ���� -- 10%
	switch( RandInt(0,9) )
	{
	case 0: case 1: case 2:	// 0 ~ 2, �ٴ��� û���Ѵ�.
	{
		//cout << "\n" << GetNameOfEntity(wife->ID()) << ": �ٴ��� �۴´�.";
		int random_floor = RandInt(0,2);

		switch (random_floor)
		{
		case 0:
			if ( wife->Location() != shack_floor_1 )
			{
				wife->GetFSM()->ChangeState(
					GoTo<MinersWife, shack_floor_1, CleanFloor::Instance>::Instance()
					);
			}
			break;
		case 1:
			if ( wife->Location() != shack_floor_2 )
			{
				wife->GetFSM()->ChangeState(
					GoTo<MinersWife, shack_floor_2, CleanFloor::Instance>::Instance()
					);
			}
			break;
		case 2:
			if ( wife->Location() != shack_floor_3 )
			{
				wife->GetFSM()->ChangeState(
					GoTo<MinersWife, shack_floor_3, CleanFloor::Instance>::Instance()
					);
			}
			break;
		}//inner switch-case
	}
		break;

	case 3: case 4: case 5:	// 3 ~ 5, ���ø� �۴´�.
		//cout << "\n" << GetNameOfEntity(wife->ID()) << ": ���ø� �۴´�.";
		if ( wife->Location() != shack_sink )
		{
			wife->GetFSM()->ChangeState(
					GoTo<MinersWife, shack_sink, Dishwash::Instance>::Instance()
					);
		}
		break;

	case 6: case 7: case 8:	// 6 ~ 8, �̺��ڸ��� �����Ѵ�.
		//cout << "\n" << GetNameOfEntity(wife->ID()) << ": ���ڸ��� �۴´�. (�̺��ڸ��� �����Ѵ�.)";
		if ( wife->Location() != shack_bed_for_wife )
		{
			wife->GetFSM()->ChangeState(
					GoTo<MinersWife, shack_bed_for_wife, MakeBed::Instance>::Instance()
					);
		}
		break;

	case 9:
		if ( wife->Location() != shack_bathroom )
		{
			wife->GetFSM()->ChangeState(
					GoTo<MinersWife, shack_bathroom, VisitBathroom::Instance>::Instance()
					);
		}
		break;
	}//outer switch-case
}

void DoHouseWork::Exit(MinersWife* wife)
{
}

bool DoHouseWork::OnMessage(MinersWife* wife, const Telegram& msg)
{
  return false;
}

//------------------------------------------------------------------------VisitBathroom

VisitBathroom* VisitBathroom::Instance()
{
  static VisitBathroom instance;

  return &instance;
}

void VisitBathroom::Enter(MinersWife* wife)
{  
	//cout << "\n" << GetNameOfEntity(wife->ID()) << ": ������ �ɾ��. ������ �Һ��� ���߰ڱ���."; 
	// STEP 01 -- Emptying First.
	wife->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('�Ѵ�')
	wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_UP) );

	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,			//time delay
							wife->ID(),			//sender ID
							ent_Miner_Bob,			//receiver ID
							Msg_HoneyINeedTissue,		//msg
							NO_ADDITIONAL_INFO);
}

void VisitBathroom::Execute(MinersWife* wife)
{
	if ( wife->m_pSetCommand->IsEmpty() == false )
	{
		wife->m_pSetCommand->Update();
	}

	//wife->GetFSM()->RevertToPreviousState();
}

void VisitBathroom::Exit(MinersWife* wife)
{
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": ȭ��ǿ��� ���´�.";
}

bool VisitBathroom::OnMessage(MinersWife* wife, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_TissueDelivered:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(wife->ID()) << " at time: "
			<< Clock->GetCurrentTime();

		SetTextColor(10); //SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(wife->ID()) <<
			": (���Ͼ���) �� �ÿ��ϴ�!";

		//wife->GetFSM()->ChangeState(CookStew::Instance());

		// ���� ��ǥ ����
		ML_POSMat	wifePosMat = wife->GetMatrixPosition();
		ML_POSPixel	wifePosPixelFixer = { wifePosMat.col*ML_IMAGE_FRAME_SIZE, wifePosMat.row*ML_IMAGE_FRAME_SIZE };

		wife->SetPixelPosition(wifePosPixelFixer);

		// ���� ����
		wife->GetFSM()->ChangeState( DoHouseWork::Instance() );
	}

	return true;

	}//end switch

	return false;
}

//------------------------------------------------------------------------CookStew
CookStew* CookStew::Instance()
{
	static CookStew instance;

	return &instance;
}

void CookStew::Enter(MinersWife* wife)
{
  //if not already cooking put the stew in the oven
  if ( !wife->Cooking() )
  {
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": ���� ���쿡 �ִ´�.";
  
	//send a delayed message myself so that I know when to take the stew
	//out of the oven
	Dispatch->DispatchMessage( 4.0,					//time delay
							  wife->ID(),			//sender ID
							  wife->ID(),			//receiver ID
							  Msg_StewReady,		//msg
							  NO_ADDITIONAL_INFO); 

	wife->SetCooking(true);

	// STEP 01 -- Emptying First.
	wife->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('��Ʃ�� ���δ�')
	wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_DOWN) );
  }
}

void CookStew::Execute(MinersWife* wife)
{
	wife->m_pSetCommand->Update();

	if ( wife->m_pSetCommand->IsEmpty() == true )
	{
		cout << "\n" << GetNameOfEntity(wife->ID()) << ": ������� ���� ���� �ȴ޺����Ѵ�.";

		// �޽����� �ҷ����� ���������Ƿ�, '���' �ݺ��Ѵ�.
		wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_DOWN) );
	}

}

void CookStew::Exit(MinersWife* wife)
{
	SetTextColor(10); //SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);
  
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": ��Ź ���� ���� �÷����´�.";
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": �������� ���� �� �ð��̱�!";
}

bool CookStew::OnMessage(MinersWife* wife, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	switch( msg.Msg )
	{
		case Msg_StewReady:
		{
			cout << "\nMessage received by " << GetNameOfEntity(wife->ID()) <<
				" at time: " << Clock->GetCurrentTime();

			SetTextColor(10); //SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);
			cout << "\n" << GetNameOfEntity(wife->ID()) << ": ���� �غ�Ʊ���! ��ô�.";

			//let hubby know the stew is ready
			Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
									wife->ID(),
									ent_Miner_Bob,
									Msg_StewReady,
									NO_ADDITIONAL_INFO);

			wife->SetCooking(false);

			wife->GetFSM()->ChangeState(DoHouseWork::Instance());               
		}

	return true;
	}//end switch

	return false;
}











////////////////////////////	������ ���� ������		///////////////////////////
// 1. �ٴ��� �۴´�. -------------------- CleanFloor -------------------------------
CleanFloor* CleanFloor::Instance()
{
	static CleanFloor instance;

	return &instance;
}

void CleanFloor::Enter(MinersWife* wife)
{
	// STEP 01 -- Emptying First.
	wife->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('�ٴ��� �۴´�')
	wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_UP) );
}

void CleanFloor::Execute(MinersWife* wife)
{
	wife->m_pSetCommand->Update();

	if ( wife->m_pSetCommand->IsEmpty() == true )
	{
		SetTextColor(10); //SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity( wife->ID() ) << " : "
			<< GetNameOfLocation( wife->Location() )<< " �� �۴´�.";

		wife->GetFSM()->ChangeState( DoHouseWork::Instance() );
	}
}

void CleanFloor::Exit(MinersWife* wife)
{
}

bool CleanFloor::OnMessage(MinersWife* wife, const Telegram& msg)
{
	return false;
}



// 2. ���ø� �۴´�. -------------------- Dishwash --------------------------------
Dishwash* Dishwash::Instance()
{
	static Dishwash instance;

	return &instance;
}

void Dishwash::Enter(MinersWife* wife)
{
	// STEP 01 -- Emptying First.
	wife->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('���ø� �۴´�')
	wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_DOWN) );
}

void Dishwash::Execute(MinersWife* wife)
{
	wife->m_pSetCommand->Update();

	if ( wife->m_pSetCommand->IsEmpty() == true )
	{
		SetTextColor(10); //SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity( wife->ID() ) << " : ���ø� �۴´�.";

		wife->GetFSM()->ChangeState( DoHouseWork::Instance() );
	}
}

void Dishwash::Exit(MinersWife* wife)
{
}

bool Dishwash::OnMessage(MinersWife* wife, const Telegram& msg)
{
	return false;
}



// 3. �̺��ڸ��� �����Ѵ�. -------------------- MakeBed --------------------------------
MakeBed* MakeBed::Instance()
{
	static MakeBed instance;

	return &instance;
}

void MakeBed::Enter(MinersWife* wife)
{
	// STEP 01 -- Emptying First.
	wife->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('�Ѵ�')
	wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_RIGHT) );
}

void MakeBed::Execute(MinersWife* wife)
{
	wife->m_pSetCommand->Update();

	if ( wife->m_pSetCommand->IsEmpty() == true )
	{
		SetTextColor(10); //SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity( wife->ID() ) << " : �̺��ڸ��� �����Ѵ�.";

		wife->GetFSM()->ChangeState( DoHouseWork::Instance() );
	}
}

void MakeBed::Exit(MinersWife* wife)
{
}

bool MakeBed::OnMessage(MinersWife* wife, const Telegram& msg)
{
	return false;
}