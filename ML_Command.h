#ifndef __ML_COMMAND_H__
#define __ML_COMMAND_H__

#include "ML_Definitions.h"
//#include "ML_BaseGameEntity.h"
class BaseGameEntity;
		// Fixed.. ���漱���� ���� �ذ�,
		// ���� ���谡 ��ȣ �������̱� ������ ������ ���� �����̴�.
		// �� ���������� �޴� �����ε� ������ �ع��� �𸣰ڴ�..
		// ���θ� �� �ʿ䰡 �ִ� ����.

//
// @ Command Class
//
//	desc. Command .. Abstract Class for Decalring Interface..
//		 ��� Command Ŭ������ �߻�ȭ �������̽��� �����մϴ�.
//
class Command
{
protected:
	BaseGameEntity&		m_entity;
	bool				m_isFinished;
public:
	Command(BaseGameEntity& entity)
		: m_entity( entity )
		, m_isFinished( false )
	{}
	virtual ~Command(){}

	virtual int Execute() = 0;

	bool GetFinished() const	{ return m_isFinished; }
	void SetFinished(bool arg)	{ m_isFinished = arg; }
};

//
// @ CommandMove Class
//
//	desc. CommandMove
//		 �Է�(����Ʈ= 8)�� Ƚ����ŭ �����ϰ� �Ϸ� �÷��׸� �����صӴϴ�.
//		 32�ȼ��� 8�������� ���� ���� ������ ��ŭ 4�ȼ��� �̵��մϴ�.
//		 4 px * 8 time = 32 px
//
class CommandMove : public Command 
{
private:
	eDirection			m_eDir;
	int					m_exeCount;
	const int			m_exeMaxCount;
	const int			m_moveSpeed;
public:
	CONSTRUCTOR		CommandMove( BaseGameEntity& entity
								, eDirection eDir
								, int exeMaxCount = 8
								, int moveSpeed = 4
								);
	DESTRUCTOR		virtual ~CommandMove();

	//
	//	OVERRIDED... and it is FINAL...
	//
	virtual int Execute() final;
};


//
// @ CommandStandMove Class
//
//	desc. CommandStandMove
//		 �Է�(����Ʈ= 5)�� Ƚ����ŭ �����ϰ� �Ϸ� �÷��׸� �����صӴϴ�.
//		 ���ڸ��� �ִϸ��̼��� �����մϴ�.
//
class CommandStandMove : public Command 
{
private:
	eDirection			m_eDir;
	int					m_exeCount;
	const int			m_exeMaxCount;
public:
	CONSTRUCTOR		CommandStandMove(BaseGameEntity& entity
									, eDirection eDir
									, int exeMaxCount = 30
									);
	DESTRUCTOR		virtual ~CommandStandMove();

	//
	//	OVERRIDED... and it is FINAL...
	//
	virtual int Execute() final;
};

//
//	and so on.. CommandShoot, CommandDig, CommandCook ...

#endif // __ML_COMMAND_H__