#ifndef __ML_COMMAND_H__
#define __ML_COMMAND_H__

#include "ML_Definitions.h"
//#include "ML_BaseGameEntity.h"
class BaseGameEntity;
		// Fixed.. 전방선언을 통한 해결,
		// 당초 설계가 상호 의존적이기 때문에 문제가 생긴 문제이다.
		// 꽤 오래전부터 겪던 문제인데 아직도 해법을 모르겠다..
		// 공부를 할 필요가 있는 내용.

//
// @ Command Class
//
//	desc. Command .. Abstract Class for Decalring Interface..
//		 기반 Command 클래스로 추상화 인터페이스를 제공합니다.
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
//		 입력(디폴트= 8)된 횟수만큼 실행하고 완료 플래그를 저장해둡니다.
//		 32픽셀을 8스텝으로 가는 것을 전제한 만큼 4픽셀씩 이동합니다.
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
//		 입력(디폴트= 5)된 횟수만큼 실행하고 완료 플래그를 저장해둡니다.
//		 제자리에 애니메이션을 실행합니다.
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