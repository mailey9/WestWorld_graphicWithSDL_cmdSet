//
//		* ML_BaseGameEntity.h
//
//		숙제용 변경 노트..
//			1) public 스코프에 '2차원 배열 위치 정보' 를 추가.
//			2) public 스코프에 '이미지 관련 정보' 를 추가.
//			3) 생성자에 위 두 정보를 명시적으로 하도록 변경.
//
//			4) 커맨드의 콘테이너(==콜렉터) 인 SetCommand를 가짐.
//


#ifndef ENTITY_H
#define ENTITY_H
//------------------------------------------------------------------------
//
//  Name:   BaseGameEntity.h
//
//  Desc:   Base class for a game object
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>

#include "messaging/Telegram.h"

#include "ML_Definitions.h"
#include "ML_SetCommand.h"

class BaseGameEntity
{
protected:
	ML_POSMat		m_posMat;
	ML_POSPixel		m_posPixel;
	eDirection		m_eDir;
	int				m_idxFrame;

	int				m_idxFrameMax;
public:
	SetCommand*		m_pSetCommand;
private:

  //every entity must have a unique identifying number
  int          m_ID;

  //this is the next valid ID. Each time a BaseGameEntity is instantiated
  //this value is updated
  static int  m_iNextValidID;

  //this must be called within the constructor to make sure the ID is set
  //correctly. It verifies that the value passed to the method is greater
  //or equal to the next valid ID, before setting the ID and incrementing
  //the next valid ID
  void SetID(int val);

public:

  BaseGameEntity(int id, ML_POSMat posMat, int frameMax = 3)
	  : m_pSetCommand( nullptr )
  {
    SetID(id);

	m_posMat.row = posMat.row;
	m_posMat.col = posMat.col;
	m_posPixel.x = posMat.col * ML_IMAGE_FRAME_SIZE;
	m_posPixel.y = posMat.row * ML_IMAGE_FRAME_SIZE;

	m_eDir = DIR_UP;
	m_idxFrame = 1;

	m_idxFrameMax = frameMax;

	m_pSetCommand = new SetCommand(*this);
  }

  virtual ~BaseGameEntity()
  {
	  if ( m_pSetCommand != nullptr )
	  {
		  delete m_pSetCommand;
		  m_pSetCommand = nullptr;
	  }
  }

  //all entities must implement an update function
  virtual void  Update()=0;

  //all entities can communicate using messages. They are sent
  //using the MessageDispatcher singleton class
  virtual bool  HandleMessage(const Telegram& msg)=0;

  int           ID()const{return m_ID;}  


//
//
//		HOMEWORK!
//
//
public:
	// the Fancy Function!
	void Animate()
	{
		if ( m_idxFrame < 1 )
		{
			m_idxFrame = 2;
		}
		else
		{
			m_idxFrame = 0;
		}
	}

	// Getters for Moving Sprite
	eDirection	GetDirection() const		{ return m_eDir; }
	int			GetFrameIndex() const		{ return m_idxFrame; }
	ML_POSPixel	GetPixelPosition() const	{ return m_posPixel; }
	ML_POSMat	GetMatrixPosition() const	{ return m_posMat; }

	// Setters for Moving Sprite
	void	SetDirection(eDirection eDir)			{ m_eDir = eDir; }
	void	SetFrameIndex(int idx)					{ m_idxFrame = idx; }
	void	SetPixelPosition(ML_POSPixel posPixel)	{ m_posPixel = posPixel; }
	void	SetMatrixPosition(ML_POSMat posMat)		{ m_posMat = posMat; }
};


#endif