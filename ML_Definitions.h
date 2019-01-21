#ifndef __ML_DEFINITIONS_H__
#define __ML_DEFINITIONS_H__

#define		ML_BOARD_SIZE_ROW	18
#define		ML_BOARD_SIZE_COL	25

#define		ML_WINDOW_SIZE_WIDTH		800
#define		ML_WINDOW_SIZE_HEIGHT		600

#define		ML_IMAGE_FRAME_SIZE			32

// defs. for Breadth First Search (BFS)
#define ML_BFS_VISITED		-1
#define ML_BFS_NO_PREV_IDX	-1
#define ML_BFS_NO_DIR		-1

typedef int MLGameBoard;

#ifndef CONSTRUCTOR
#define CONSTRUCTOR
#endif

#ifndef DESTRUCTOR
#define DESTRUCTOR
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

enum eDirection{
	/*DIR_DOWN = 0,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,*/
	DIR_UP = 0,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,

	SIZE_OF_DIR
};	//	in ccw from 12 o'clock.

struct ML_POSMat{
	int row;
	int col;

	ML_POSMat(){}
	ML_POSMat(int _row, int _col)
		: row(_row)
		, col(_col)
	{}
};

struct ML_POSPixel{
	int x;
	int y;

	ML_POSPixel(){}
	ML_POSPixel(int _x, int _y)
		: x(_x)
		, y(_y)
	{}
};

typedef struct tagML_BFSPath{
	int	row_pos;
	int	col_pos;
	int	prev_idx;
	int	cmd_dir;
}ML_BFSPath;

extern MLGameBoard g_boardLogic[ML_BOARD_SIZE_ROW][ML_BOARD_SIZE_COL];

#endif // __ML_DEFINITIONS_H__



//typedef struct tgML_POSMat{
//	int row;
//	int col;
//}ML_POSMat;

//typedef struct tgML_POSPixel{
//	int x;
//	int y;
//}ML_POSPixel;