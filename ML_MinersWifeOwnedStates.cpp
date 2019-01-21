//
//		* ML_MinersWifeOwnedStates.cpp
//
//		숙제용 변경 노트..
//			1) 여러가지 수정 (상태전이가 노가다임)
//			2) 상태를 여러가지 추가하자 전역상태의 '화장실 가는 확률' 이 제대로 동작하지 않는 것을 확인.
//			  이를 제외시켜 버리고, 메시지만큼은 제대로 핸들하도록 놔둠.
//			3) '화장실'은 DoHouseWork 중에 확률적으로 감.
//
//	<!>		*노트 : 2)에 대해 과제 이후에 꼭 전역상태도 제대로 핸들하는 FSM을 만들어 보도록 하자.
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

	// 1000분의 1 확률로..
	//		'화장실 방문상태'가 아니고,
	//		'화장실로 가고 있는 상태'가 아니라면.
	//			'화장실로 가고 있는 상태'가 되어라!
	//	*노트 : 코드 에러.. 일단 꺼버려!
	//			이유는 심플한데..

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
			": 여보, 맛있는 시골식 국을 만들어 드릴께요.";

		//wife->GetFSM()->ChangeState(CookStew::Instance());

		// 강제 좌표 수정
		ML_POSMat	wifePosMat			= wife->GetMatrixPosition();
		ML_POSPixel	wifePosPixelFixer	= { wifePosMat.col*ML_IMAGE_FRAME_SIZE, wifePosMat.row*ML_IMAGE_FRAME_SIZE };

		wife->SetPixelPosition( wifePosPixelFixer );

		// 상태 전이
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
//	cout << "\n" << GetNameOfEntity(wife->ID()) << ": 집안일을 좀더 할 시간이군!";
}

void DoHouseWork::Execute(MinersWife* wife)
{
	//	확률 분배
	//		0~9 를 뽑고
	//		0-2 : 바닥을 청소한다 -- 30%
	//				내부적으로 0-2의 시드를 뽑아서,
	//				바닥 1-2-3 을 각 33% 확률로 청소한다. -- 각 바닥을 청소할 확률은 약 10%
	//		3-5 : 접시를 닦는다 -- 30%
	//		6-8 : 이부자리를 정리한다 -- 30%
	//		 9  : 화장실을 간다 -- 10%
	switch( RandInt(0,9) )
	{
	case 0: case 1: case 2:	// 0 ~ 2, 바닥을 청소한다.
	{
		//cout << "\n" << GetNameOfEntity(wife->ID()) << ": 바닥을 닦는다.";
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

	case 3: case 4: case 5:	// 3 ~ 5, 접시를 닦는다.
		//cout << "\n" << GetNameOfEntity(wife->ID()) << ": 접시를 닦는다.";
		if ( wife->Location() != shack_sink )
		{
			wife->GetFSM()->ChangeState(
					GoTo<MinersWife, shack_sink, Dishwash::Instance>::Instance()
					);
		}
		break;

	case 6: case 7: case 8:	// 6 ~ 8, 이부자리를 정리한다.
		//cout << "\n" << GetNameOfEntity(wife->ID()) << ": 잠자리를 닦는다. (이부자리를 정리한다.)";
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
	//cout << "\n" << GetNameOfEntity(wife->ID()) << ": 통으로 걸어간다. 깨끗이 소변을 봐야겠구나."; 
	// STEP 01 -- Emptying First.
	wife->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('싼다')
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
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": 화장실에서 나온다.";
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
			": (쓱싹쓱싹) 아 시원하다!";

		//wife->GetFSM()->ChangeState(CookStew::Instance());

		// 강제 좌표 수정
		ML_POSMat	wifePosMat = wife->GetMatrixPosition();
		ML_POSPixel	wifePosPixelFixer = { wifePosMat.col*ML_IMAGE_FRAME_SIZE, wifePosMat.row*ML_IMAGE_FRAME_SIZE };

		wife->SetPixelPosition(wifePosPixelFixer);

		// 상태 전이
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
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": 국을 오븐에 넣는다.";
  
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

	// STEP 02 -- Add an Action.. ('스튜를 끓인다')
	wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_DOWN) );
  }
}

void CookStew::Execute(MinersWife* wife)
{
	wife->m_pSetCommand->Update();

	if ( wife->m_pSetCommand->IsEmpty() == true )
	{
		cout << "\n" << GetNameOfEntity(wife->ID()) << ": 저녁밥을 차려 놓고 안달복달한다.";

		// 메시지가 불려지면 빠져나가므로, '계속' 반복한다.
		wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_DOWN) );
	}

}

void CookStew::Exit(MinersWife* wife)
{
	SetTextColor(10); //SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);
  
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": 식탁 위에 국을 올려놓는다.";
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": 집안일을 좀더 할 시간이군!";
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
			cout << "\n" << GetNameOfEntity(wife->ID()) << ": 국이 준비됐군요! 듭시다.";

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











////////////////////////////	숙제를 하자 숙제를		///////////////////////////
// 1. 바닥을 닦는다. -------------------- CleanFloor -------------------------------
CleanFloor* CleanFloor::Instance()
{
	static CleanFloor instance;

	return &instance;
}

void CleanFloor::Enter(MinersWife* wife)
{
	// STEP 01 -- Emptying First.
	wife->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('바닥을 닦는다')
	wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_UP) );
}

void CleanFloor::Execute(MinersWife* wife)
{
	wife->m_pSetCommand->Update();

	if ( wife->m_pSetCommand->IsEmpty() == true )
	{
		SetTextColor(10); //SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity( wife->ID() ) << " : "
			<< GetNameOfLocation( wife->Location() )<< " 을 닦는다.";

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



// 2. 접시를 닦는다. -------------------- Dishwash --------------------------------
Dishwash* Dishwash::Instance()
{
	static Dishwash instance;

	return &instance;
}

void Dishwash::Enter(MinersWife* wife)
{
	// STEP 01 -- Emptying First.
	wife->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('접시를 닦는다')
	wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_DOWN) );
}

void Dishwash::Execute(MinersWife* wife)
{
	wife->m_pSetCommand->Update();

	if ( wife->m_pSetCommand->IsEmpty() == true )
	{
		SetTextColor(10); //SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity( wife->ID() ) << " : 접시를 닦는다.";

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



// 3. 이부자리를 정리한다. -------------------- MakeBed --------------------------------
MakeBed* MakeBed::Instance()
{
	static MakeBed instance;

	return &instance;
}

void MakeBed::Enter(MinersWife* wife)
{
	// STEP 01 -- Emptying First.
	wife->m_pSetCommand->Empty();

	// STEP 02 -- Add an Action.. ('싼다')
	wife->m_pSetCommand->Add( new CommandStandMove(*wife, DIR_RIGHT) );
}

void MakeBed::Execute(MinersWife* wife)
{
	wife->m_pSetCommand->Update();

	if ( wife->m_pSetCommand->IsEmpty() == true )
	{
		SetTextColor(10); //SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity( wife->ID() ) << " : 이부자리를 정리한다.";

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