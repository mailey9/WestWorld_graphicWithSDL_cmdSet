#ifndef LOCATIONS_H
#define LOCATIONS_H

#include <cassert>

#include "ML_Definitions.h"

enum location_type
{
	shack = 0,
	shack_bed,
	shack_table,
	goldmine,
	goldmine_nugget,
	bank,
	bank_account,
	saloon,
	saloon_bartender,

	shack_floor_1,
	shack_floor_2,
	shack_floor_3,
	shack_bathroom,
	shack_bed_for_wife,
	shack_sink,

	rob_shack_bed,
	rob_goldmine_nugget,
	rob_bank_account,
	rob_saloon_bartender,

};

inline std::string GetNameOfLocation(int n)
{
	std::string retStr;

	switch (n)
	{
	case shack:				retStr += "집";			break;
	case shack_bed:			retStr += "침대";		break;
	case shack_table:		retStr += "테이블";		break;
	case goldmine:			retStr += "광산";		break;
	case goldmine_nugget:	retStr += "금광(광석)";	break;
	case bank:				retStr += "은행";		break;
	case bank_account:		retStr += "은행원";		break;
	case saloon:			retStr += "술집";		break;
	case saloon_bartender:	retStr += "바텐더";		break;

	case shack_floor_1:		retStr += "집바닥(1)";	break;
	case shack_floor_2:		retStr += "집바닥(2)";	break;
	case shack_floor_3:		retStr += "집바닥(3)";	break;

	case shack_bathroom:	retStr += "화장실통";	break;
	case shack_bed_for_wife:retStr += "이부자리";	break;
	case shack_sink:		retStr += "싱크대";		break;

	//////////////////////////////////////////////////////

	case rob_shack_bed:			retStr += "침대";	break;
	case rob_goldmine_nugget:	retStr += "광산";	break;
	case rob_bank_account:		retStr += "은행";	break;
	case rob_saloon_bartender:	retStr += "술집";	break;

	default:
		retStr += "UNKNOWN!";
	}

	return retStr;
}

inline ML_POSMat GetCoodOfLocation(int n)
{
	ML_POSMat retPosMat = { -1, -1 };

	switch (n)
	{
	case shack:
		retPosMat.row = 3;
		retPosMat.col = 4;
		break;
	case goldmine:
		retPosMat.row = 14;
		retPosMat.col = 11;
		break;
	case bank:
		retPosMat.row = 10;
		retPosMat.col = 15;
		break;
	case saloon:
		retPosMat.row = 13;
		retPosMat.col = 5;
		break;
	case shack_bed:
		retPosMat.row = 3;
		retPosMat.col = 8;
		break;
	case shack_table:		//'s right
		retPosMat.row = 2;
		retPosMat.col = 4;
		break;
	case goldmine_nugget:	//'s left
		retPosMat.row = 13;
		retPosMat.col = 11;
		break;
	case bank_account:		//'s down
		retPosMat.row = 9;
		retPosMat.col = 16;
		break;
	case saloon_bartender:
		retPosMat.row = 10;
		retPosMat.col = 6;
		break;
	case shack_floor_1:
		//retStr += "집바닥(1)";
		retPosMat.row = 1;
		retPosMat.col = 1;
		break;
	case shack_floor_2:
		//retStr += "집바닥(2)";
		retPosMat.row = 1;
		retPosMat.col = 4;
		break;
	case shack_floor_3:		
		//retStr += "집바닥(3)";	
		retPosMat.row = 3;
		retPosMat.col = 1;
		break;
	case shack_bathroom:
		//retStr += "화장실통";	
		retPosMat.row = 3;
		retPosMat.col = 5;
		break;
	case shack_bed_for_wife:
		//retStr += "이부자리";	
		retPosMat.row = 3;
		retPosMat.col = 7;
		break;
	case shack_sink:		
		//retStr += "싱크대";		
		retPosMat.row = 1;
		retPosMat.col = 2;
		break;
	////////////////////////////////////////////////////////////////

	case rob_shack_bed:			
		retPosMat.row = 1;
		retPosMat.col = 22;
		break;
	case rob_goldmine_nugget:	
		retPosMat.row = 14;
		retPosMat.col = 11;
		break;
	case rob_bank_account:		
		retPosMat.row = 9;
		retPosMat.col = 17;
		break;
	case rob_saloon_bartender:	
		retPosMat.row = 10;
		retPosMat.col = 5;
		break;

	default:
		// an error..
		assert( (retPosMat.row != -1) && (retPosMat.col != -1) );
		break;
	}

	return retPosMat;
}


//uncomment this to send the output to a text file
//#define TEXTOUTPUT




#endif