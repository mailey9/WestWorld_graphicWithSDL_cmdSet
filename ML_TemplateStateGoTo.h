#ifndef __ML_STATE_COMMON_H__
#define __ML_STATE_COMMON_H__

#include "FSM/State.h"
#include "Locations.h"			//	enum location_type,
#include "EntityNames.h"

#include "ML_Definitions.h"		//	enum eDirection, 
#include "ML_BFS.h"				//	function BFS();
#include "ML_BaseGameEntity.h"	//	class BaseGameEntity, 

#include "ML_Miner.h"
#include "ML_MinersWife.h"
#include "ML_Rob.h"

class Miner;
class MinersWife;
class Rob;

template <typename entity_type, location_type eLocation, void* nextState = nullptr, int exeCount = 8, int moveSpeed = 4>
class GoTo : public State<entity_type>
{
private:
	GoTo()
	{
		//m_instance = static_cast <entity_type, eLocation>* (this);

	}

	GoTo(const GoTo&);
	GoTo& operator=(const GoTo&);

	static GoTo*			m_instance;
public:
	//
	// @ Singleton Instance(), yes this is Singleton
	//
	static GoTo* Instance()
	{
		if ( m_instance == nullptr )
		{
			m_instance = new GoTo;
		}

		return m_instance;
	}

	//
	// @ Enter()
	//		desc. Find the path to eLocation then save it on Entity's CommandSet.
	//
	virtual void Enter(entity_type* entity)
	{
		BaseGameEntity* pBaseGameEntity = nullptr;
		pBaseGameEntity = dynamic_cast<BaseGameEntity*>(entity);

		if ( pBaseGameEntity != nullptr )
		{
			//	STEP 01 -- Empyting Set of Commands
			pBaseGameEntity->m_pSetCommand->Empty();

			//	STEP 02 -- Find the Path to Destination
			std::deque<eDirection>	dequeDiretions;
			ML_POSMat				orgPosMat		=	pBaseGameEntity->GetMatrixPosition();
			ML_POSMat				destPosMat		=	GetCoodOfLocation( eLocation );

			int retVal = minorlife::BFS( g_boardLogic
										, orgPosMat
										, destPosMat
										, &dequeDiretions );

			//	STEP 03 --	if path is found,
			//				Add this Path to Entity's set commands.
			if ( retVal == 0 )
			{
				pBaseGameEntity->m_pSetCommand->AddFromDirectionDeque( dequeDiretions, exeCount, moveSpeed );
			}
			else
			{
				std::cout << "\n" << "BFS Failure! ret code <" << retVal << ">";
				//exit(1);
			}

			std::cout << "\n" << GetNameOfEntity( pBaseGameEntity->ID() ) << ": "
						<< GetNameOfEntity( pBaseGameEntity->ID() ) << "은 "
						<< GetNameOfLocation( eLocation ) << "으로 걸어간다.";
		}
	}

	//
	// @ Execute()
	//		desc. Call commands on set of those, til 'empty'.
	//
	virtual void Execute(entity_type* entity)
	{
		BaseGameEntity* pBaseGameEntity = nullptr;
		pBaseGameEntity = dynamic_cast<BaseGameEntity*>( entity );

		if ( pBaseGameEntity != nullptr )
		{
			//std::cout << __FUNCTION__;
			if ( pBaseGameEntity->m_pSetCommand->IsEmpty() == false )
			{
				pBaseGameEntity->m_pSetCommand->Update();
			}
			else
			{
				//
				//	아! 도저히 모르겠다.. 과제는 해야지!
				//
				MinersWife* pWife	= dynamic_cast<MinersWife*>( entity );
				Miner*		pMiner	= dynamic_cast<Miner*>( entity );
				Rob*		pRob	= dynamic_cast<Rob*>( entity );

				if ( pWife != nullptr )
				{
					State<MinersWife>* (*pfStateInstanceCaller) () = (State<MinersWife>*(*)())nextState;
					State<MinersWife>* pNextState = (*pfStateInstanceCaller)();

					pWife->GetFSM()->ChangeState( pNextState );
					//std::cout << "와이프니?";
				}

				if ( pMiner != nullptr )
				{
					State<Miner>* (*pfStateInstanceCaller) () = (State<Miner>*(*)())nextState;
					State<Miner>* pNextState = (*pfStateInstanceCaller)();

					pMiner->GetFSM()->ChangeState( pNextState );
					//std::cout << pNextState << "밥이니?";
				}

				if ( pRob != nullptr )
				{
					State<Rob>* (*pfStateInstanceCaller) () = (State<Rob>*(*)())nextState;
					State<Rob>* pNextState = (*pfStateInstanceCaller)();

					pRob->GetFSM()->ChangeState( pNextState );
					//std::cout << pNextState << "롭이니?";
				}
			}
		}
	}

	//
	// @ Exit()
	//		desc. Change location on here.
	//
	virtual void Exit(entity_type* entity)
	{
		MinersWife* pWife = dynamic_cast<MinersWife*>(entity);
		Miner*		pMinor = dynamic_cast<Miner*>(entity);
		Rob*		pRob = dynamic_cast<Rob*>(entity);

		if ( pWife != nullptr )
		{
			pWife->ChangeLocation( eLocation );
		}

		if ( pMinor != nullptr )
		{
			pMinor->ChangeLocation( eLocation );
		}

		if ( pRob != nullptr )
		{
			pRob->ChangeLocation( eLocation );
		}
	}

	virtual bool OnMessage(entity_type* agent, const Telegram& msg)
	{
		return false;
	}
};

template <typename entity_type, location_type eLocation, void* nextState, int exeCount, int moveSpeed>
GoTo<entity_type, eLocation, nextState, exeCount, moveSpeed>*
	GoTo<entity_type, eLocation, nextState, exeCount, moveSpeed>::m_instance = nullptr;

#endif	 // __ML_STATE_COMMON_H__