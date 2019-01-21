#include "ML_SetCommand.h"

#include "ML_BaseGameEntity.h"
//
int SetCommand::Update()
{
	if ( m_deqCommand.empty() == false )
	{
		int retVal = -1;

		Command* pFirstCommand = m_deqCommand.front();

		pFirstCommand->Execute();

		if ( pFirstCommand->GetFinished() == true )	// if - finished.
		{
			delete pFirstCommand;					//		free pointer.
			m_deqCommand.pop_front();				//		then pop() from collector.
		}

		return retVal;
	}
	else
	{
		return 0;
	}
}

//
int SetCommand::AddFromDirectionDeque(std::deque<eDirection>& deque_eDir, int exeCount/*=8*/, int moveSpeed/*=4*/)
{
	for ( auto it = deque_eDir.begin()
		; it != deque_eDir.end()
		; ++it )
	{
		Add( new CommandMove( m_owner, *it, exeCount, moveSpeed ) );
	}

	return 0;
}

//
int SetCommand::Add(Command* pCmd)
{
	m_deqCommand.push_back( pCmd );

	return 0;	// how it can be ERR? -- std:: is made by the best programmers in the planet.
}

//
int SetCommand::Empty()
{
	for ( auto it = m_deqCommand.begin()
		; it != m_deqCommand.end()
		; ++it )
	{
		delete *it;
	}

	m_deqCommand.clear();

	return 0;
}

//
bool SetCommand::IsEmpty()
{
	if ( m_deqCommand.empty() == true )
	{
		return true;
	}

	return false;
}