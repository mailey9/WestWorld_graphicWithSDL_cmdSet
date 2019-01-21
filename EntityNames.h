#ifndef NAMES_H
#define NAMES_H

#include <string>

#include <sstream>
#include <iomanip>
#include "Time/CrudeTimer.h"	// 이름에 타임 스탬프 추가를 위해

enum 
{
  ent_Miner_Bob = 0,
  ent_Elsa,
  ent_DrunkenMiner_Rob
};

inline std::string GetNameOfEntity(int n)
{
	std::string retStr;

	std::ostringstream ss;
#ifdef ML_SHOW_TIMESTAMP
	ss << '<' << std::fixed << std::setprecision(3) << Clock->GetCurrentTime() << "> ";

	retStr += ss.str();
#endif

	switch(n)
	{
	case ent_Miner_Bob:
		//return "Miner Bob";
		retStr += "광부 Bob";
		break;

	case ent_Elsa:
		//return "Elsa"; 
		retStr += "Elsa";
		break;

	case ent_DrunkenMiner_Rob:
		retStr += "주정뱅이 Rob";
		break;

	default:
		//return "UNKNOWN!";
		retStr += "UNKNOWN!";
		break;
	}

	return retStr;
}

#endif