//
//		* ML_EntityManager.h
//
//		숙제용 변경 노트..
//			1) 그래픽용 Wrapper에서 Entity들을 참조할 수 있도록
//			  Wrapper를 friend class로 등록.. (map에 직접접근하기 위해서)
//						--> 별도의 컨테이너를 따로 가지는 게 불합리한 것 같아서...
//


#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   EntityManager.h
//
//  Desc:   Singleton class to handle the  management of Entities.          
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <map>
#include <cassert>
#include <string>

class SDLwrapper;

class BaseGameEntity;

//provide easy access
#define EntityMgr EntityManager::Instance()



class EntityManager
{
	friend class SDLwrapper;
private:

  typedef std::map<int, BaseGameEntity*> EntityMap;

private:

  //to facilitate quick lookup the entities are stored in a std::map, in which
  //pointers to entities are cross referenced by their identifying number
  EntityMap m_EntityMap;

  EntityManager(){}

  //copy ctor and assignment should be private
  EntityManager(const EntityManager&);
  EntityManager& operator=(const EntityManager&);

public:

  static EntityManager* Instance();

  //this method stores a pointer to the entity in the std::vector
  //m_Entities at the index position indicated by the entity's ID
  //(makes for faster access)
  void            RegisterEntity(BaseGameEntity* NewEntity);

  //returns a pointer to the entity with the ID given as a parameter
  BaseGameEntity* GetEntityFromID(int id)const;

  //this method removes the entity from the list
  void            RemoveEntity(BaseGameEntity* pEntity);
};







#endif