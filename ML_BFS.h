#ifndef __ML_BFS_H__
#define __ML_BFS_H__

#include <vector>
#include <deque>

#include "ML_Definitions.h"

//#define ML_BFS_DEBUG

namespace minorlife{
	//
	// @ BFS function
	//
	//	desc.	write DEQUE<DIRECTION>(==std::deque<eDir>) on given pointer.
	//			returns (0) on success
	//			returns (< 0) on failed. (err code)
	//			
	//	param.	IN	MLGameBoard			logical gameboard ( 1 == wall, 0 == empty )
	//			IN	ML_POSMatrix		coordinates of origin
	//			IN	ML_POSMatrix		coordinates of destination
	//			OUT	DEQUE<DIRECTION>	command list
	//
	int BFS(  IN  MLGameBoard gameBoard[ML_BOARD_SIZE_ROW][ML_BOARD_SIZE_COL]
			, IN  ML_POSMat orgPosMat
			, IN  ML_POSMat destPosMat
			, OUT std::deque<eDirection>* p_deque_route );
}
#endif // __ML_BFS_H__