#ifndef __ML_SET_COMMAND_H__
#define __ML_SET_COMMAND_H__

#include <deque>

#include "ML_Command.h"

class BaseGameEntity;

//
// @ SetCommand
//
//	desc. CommandSet .. Collector of Command
//		 'Ŀ�ǵ�' Ŭ������ �÷��� �� ���� �������̽��� �����ϴ� �����̳�.
//		 ���������δ� ����� ����Ʈ�� std::deque�� �̿��մϴ�.
//
class SetCommand
{
private:
	std::deque<Command*>	m_deqCommand;
	BaseGameEntity&			m_owner;
public:	
	CONSTRUCTOR		SetCommand(BaseGameEntity& owner)
						: m_owner( owner )
					{}

	DESTRUCTOR		~SetCommand()
					{}


	//
	//	desc. Execute(); element on deque sequentially. (@ end one then another)
	//
	int Update();

	//
	//	desc. Add with std::deque<enumerations>.
	//
	int AddFromDirectionDeque( std::deque<eDirection>& deque_eDir, int exeCount = 8, int moveSpeed = 4 );

	//
	//	desc. Add *Command at BACK
	//
	int Add( Command* pCmd );

	//
	//	desc. Free all.. and Empty the collector.
	//
	int Empty();

	//
	//
	//
	bool IsEmpty();
};

#endif // __ML_SET_COMMAND_H__