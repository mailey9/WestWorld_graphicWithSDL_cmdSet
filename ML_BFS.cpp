#include "ML_BFS.h"

int minorlife::BFS( MLGameBoard gameBoard[ML_BOARD_SIZE_ROW][ML_BOARD_SIZE_COL]
					, ML_POSMat orgPosMat
					, ML_POSMat destPosMat
					, OUT std::deque<eDirection>* p_deque_route
					)
{
	if ( destPosMat.row < 0 || destPosMat.row >= ML_BOARD_SIZE_ROW
		|| destPosMat.col < 0 || destPosMat.col >= ML_BOARD_SIZE_COL )
	{
		return -1;	// given destination index error.
	}

	if ( gameBoard[ destPosMat.row ][ destPosMat.col ] == 1 )
	{
		return -2;	// there's NO ROUTE for this coord.
	}

	if ( (destPosMat.row == orgPosMat.row) && (destPosMat.col == orgPosMat.col) )
	{
		return -3;	// given destination is the origin.
	}

	// initializing variables.
	std::vector<ML_BFSPath>		Q;
	unsigned int				Q_front = 0;
	unsigned int				Q_tail = 1;
	MLGameBoard					BFS_map[ML_BOARD_SIZE_ROW][ML_BOARD_SIZE_COL];

	// preparing map & Q.
	ML_BFSPath origin = { orgPosMat.row, orgPosMat.col, ML_BFS_NO_PREV_IDX, ML_BFS_NO_DIR };
	Q.reserve( ML_BOARD_SIZE_ROW*ML_BOARD_SIZE_COL );		// v.01 : Let's reserve it for better performance.
	Q.push_back( origin );
	memcpy( BFS_map, gameBoard, sizeof(MLGameBoard)*ML_BOARD_SIZE_ROW*ML_BOARD_SIZE_COL );
	BFS_map[ orgPosMat.row ][ orgPosMat.col ] = ML_BFS_VISITED;

	//// make offset-table first.
	const ML_POSMat offset_table[SIZE_OF_DIR] = {
		{ -1, 0 },	//UP
		{ 0, 1 },	//RIGHT
		{ 1, 0 },	//DOWN
		{ 0, -1 }	//LEFT		.. the order follows enum DIR.
	};
	// make offset-table first.
	//const ML_POSMat offset_table[SIZE_OF_DIR] = {
	//	{ 1, 0 },	//DOWN
	//	{ 0, -1 },	//LEFT
	//	{ 0, 1 },	//RIGHT
	//	{ 1, 0 }	//UP		.. the order follows enum DIR.
	//};


	while ( Q_front < Q_tail )
	{
		for ( int dir = 0; dir < 4; ++dir )		// dir < SIZE_OF_DIR
		{
			ML_POSMat coord = { Q[Q_front].row_pos, Q[Q_front].col_pos };
			coord.row += offset_table[dir].row;
			coord.col += offset_table[dir].col;

			//	if ... DESTINATION
			//		Create Route then Return it.
			if ( (coord.row == destPosMat.row) && (coord.col == destPosMat.col) )
			{
				if ( p_deque_route == NULL )
				{
					return -4;	// No OUT param given.
				}

				//	first, record current DIR
				p_deque_route->push_front( eDirection(dir) );							//q_route->push( eDirection(dir) );						// was on v.01
				//	then, loop while..
				//		..til meets 'NO_DIR' && 'NO_PREV_IDX'
				while ( (Q[Q_front].cmd_dir != ML_BFS_NO_DIR)
						&& (Q[Q_front].prev_idx != ML_BFS_NO_PREV_IDX) )
				{
					p_deque_route->push_front( eDirection(Q[Q_front].cmd_dir) );		//q_route->push( eDirection(Q[Q_front].cmd_dir) );		// was on v.01
					Q_front = Q[Q_front].prev_idx;
				}

				return 0;	// route found!, ESTABLISH ROUTE then return success code(0).
			}
			//	else if ... EMPTY && NOT VISITED
			else if ( (BFS_map[coord.row][coord.col] == 0)
					&& (BFS_map[coord.row][coord.col] != ML_BFS_VISITED) )
			{
				BFS_map[coord.row][coord.col] = ML_BFS_VISITED;
				ML_BFSPath new_coord = { coord.row, coord.col, Q_front, dir };
				Q.push_back( new_coord );

				++Q_tail;
			}
			//	else if ... WALL ( == 1 )
			//		; -- pass!

		}//for -4dir-

		++Q_front;
	}


	return -5;		// couldn't find ROUTE,		-- logical error.		<Q. is it possible?>
					// <!>	indexing error must be checked in loop though..
}