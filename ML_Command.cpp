#include <iostream>

#include "EntityNames.h"

#include "ML_Command.h"
#include "ML_BaseGameEntity.h"

#include "ML_Rob.h"

CommandMove::CommandMove(BaseGameEntity& entity, eDirection eDir, int exeMaxCount /*= 8*/, int moveSpeed /*= 4*/)
	: m_eDir ( eDir )
	, m_exeCount ( 0 )
	, m_exeMaxCount ( exeMaxCount )
	, m_moveSpeed( moveSpeed )
	, Command ( entity )
{
	
}

CommandMove::~CommandMove()
{
	//
}

int CommandMove::Execute()
{
	if ( m_isFinished != true )
	{
		m_entity.SetDirection( m_eDir );

		//int currIdx = m_entity.GetFrameIndex();
		ML_POSPixel posPixel = m_entity.GetPixelPosition();

		switch ( m_eDir )
		{
		case DIR_UP:	posPixel.y -= m_moveSpeed;	break;
		case DIR_DOWN:	posPixel.y += m_moveSpeed;	break;
		case DIR_LEFT:	posPixel.x -= m_moveSpeed;	break;
		case DIR_RIGHT:	posPixel.x += m_moveSpeed;	break;
		default:
			return -1;	// err... not valid enum given.
		}

		m_entity.SetPixelPosition( posPixel ); 
		m_entity.Animate();

		m_exeCount += 1;

		if ( m_exeCount >= m_exeMaxCount )
		{
			ML_POSMat posMat = m_entity.GetMatrixPosition();

			switch (m_eDir)
			{
			case DIR_UP:	posMat.row -= 1;	break;
			case DIR_DOWN:	posMat.row += 1;	break;
			case DIR_LEFT:	posMat.col -= 1;	break;
			case DIR_RIGHT:	posMat.col += 1;	break;
			default:
				return -1;	// err... not valid enum given.
			}

			m_entity.SetMatrixPosition( posMat );
			m_entity.SetFrameIndex( 1 );
			this->SetFinished( true );

			//
			//		과제용으로 대충 !!		-- on 2017-11-06-11:59
			//
			Rob* pRob = dynamic_cast<Rob*>( &m_entity );
			if ( pRob != nullptr )
			{
				if ( pRob->IsDrunken() == true )
				{
					std::cout << "\n" << GetNameOfEntity(pRob->ID()) << ": " << "비틀..";
				}
			}
		}
	}

	return 0;
}




//
CommandStandMove::CommandStandMove(BaseGameEntity& entity, eDirection eDir, int exeMaxCount /*= 30*/)
	: m_eDir ( eDir )
	, m_exeCount ( 0 )
	, m_exeMaxCount ( exeMaxCount )
	, Command ( entity )
{
	
}

CommandStandMove::~CommandStandMove()
{
	//
}

int CommandStandMove::Execute()
{
	if ( m_isFinished != true )
	{
		m_entity.SetDirection( m_eDir );

		m_entity.Animate();

		m_exeCount += 1;

		if ( m_exeCount >= m_exeMaxCount )
		{
			this->SetFinished( true );
		}
	}

	return 0;
}