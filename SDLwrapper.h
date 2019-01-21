#ifndef __ML_SDL_WRAPPER_H__
#define __ML_SDL_WRAPPER_H__

#include <string>

#include <SDL.h>

#include "ML_Definitions.h"
#include "ML_EntityManager.h"

//#pragma comment(lib, "SDL2.lib")

class SDLutil
{
public:
	static SDL_Surface* LoadBMP( std::string path )
	{
		SDL_Surface* img = NULL;

		// Load Bitmap from PATH
		img = SDL_LoadBMP( path.c_str() );
		
		// Setup COLOR-KEY
		if ( SDL_SetColorKey(img
							, SDL_TRUE
							, SDL_MapRGB(img->format, 0xFF, 0x00, 0xFF)
							) != 0)
		{
			SDL_FreeSurface( img );

			return NULL;
		}

		return img;
	}
};

class SDLwrapper
{
private:
	bool			m_initFlag;
	bool			m_runFlag;

	SDL_Window*		m_sdlWindow;
	//SDL_Renderer*	m_sdlRenderer;
	SDL_Surface*	m_sdlSurface;
	SDL_Rect		m_rcSurface;

	SDL_Event		e;

	SDL_Surface*	m_bmpCharacterBob;
	SDL_Surface*	m_bmpCharacterElsa;
	SDL_Surface*	m_bmpCharacterRob;

	//int				m_arrMap[20][25];
	SDL_Rect		m_rcTile[ML_BOARD_SIZE_ROW][ML_BOARD_SIZE_COL];
public:
	CONSTRUCTOR		SDLwrapper() :
						m_initFlag( false )
						, m_sdlWindow( NULL )
						//, m_sdlRenderer( NULL )
						, m_sdlSurface( NULL )
					{
						m_bmpCharacterBob	= NULL;
						m_bmpCharacterElsa	= NULL;
						m_bmpCharacterRob	= NULL;
					}
	DESTRUCTOR		~SDLwrapper()
					{
						this->Finalize();	// to be sure, -- actually --, isn't needed.
					}

public:
	int Initialize()
	{
		// org code. changed it to HW Render -> SW Draw
		//retVal = SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_BORDERLESS, &m_sdlWindow, &m_sdlRenderer);
		if ( m_initFlag == false )
		{
			SDL_SetMainReady();

			if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
			{
				return -1;
			}

			// Create Window
			m_sdlWindow = SDL_CreateWindow( "Hello SDL"
										, SDL_WINDOWPOS_CENTERED
										, SDL_WINDOWPOS_CENTERED
										, ML_WINDOW_SIZE_WIDTH
										, ML_WINDOW_SIZE_HEIGHT
										, SDL_WINDOW_SHOWN );
			if ( m_sdlWindow == NULL )
			{
				return -2;
			}

			m_rcSurface.x = m_rcSurface.y = 0;
			m_rcSurface.w = ML_WINDOW_SIZE_WIDTH;
			m_rcSurface.h = ML_WINDOW_SIZE_WIDTH;

			// Create Surface
			m_sdlSurface = SDL_GetWindowSurface( m_sdlWindow );
			if ( m_sdlSurface == NULL )
			{
				return -3;
			}

			m_runFlag = true;
		}

		// LOAD IMAGES..
		m_bmpCharacterBob = SDLutil::LoadBMP("resources/miner_96x128.bmp");
		m_bmpCharacterElsa = SDLutil::LoadBMP("resources/elsa_96x128.bmp");
		m_bmpCharacterRob = SDLutil::LoadBMP("resources/rob_96x128.bmp");

		// Setup tile vars..
		for ( int r = 0; r < ML_BOARD_SIZE_ROW; ++r )
		{
			for ( int c = 0; c < ML_BOARD_SIZE_COL; ++c )
			{
				m_rcTile[r][c].x = c*32;
				m_rcTile[r][c].y = r*32;
				m_rcTile[r][c].w = 32;
				m_rcTile[r][c].h = 32;
			}
		}

		return 0;
	}

	void Finalize()
	{
		if ( m_bmpCharacterBob != NULL )
		{
			SDL_FreeSurface( m_bmpCharacterBob );
			m_bmpCharacterBob = NULL;
		}
		if ( m_bmpCharacterElsa != NULL )
		{
			SDL_FreeSurface( m_bmpCharacterElsa );
			m_bmpCharacterElsa = NULL;
		}
		if (m_bmpCharacterRob != NULL)
		{
			SDL_FreeSurface( m_bmpCharacterRob );
			m_bmpCharacterRob = NULL;
		}

		/*if ( m_sdlRenderer != NULL )
		{
			SDL_DestroyRenderer(m_sdlRenderer);
			m_sdlRenderer = NULL;
		}*/

		if ( m_sdlSurface != NULL )
		{
			SDL_FreeSurface( m_sdlSurface );
			m_sdlSurface = NULL;
		}
		if ( m_sdlWindow != NULL )
		{
			SDL_DestroyWindow( m_sdlWindow );
			m_sdlWindow = NULL;
		}

		if ( m_initFlag == true )
		{
			SDL_Quit();
			m_initFlag = false;
		}
	}

	// Press 'q' to Exit, using pointer.
	int HandleSDL()
	{
		while ( SDL_PollEvent(&e) )
		{
			switch (e.type)
			{
			case SDL_QUIT:
				m_runFlag = false;
				break;
			case SDL_KEYDOWN:
				if ( e.key.keysym.sym == SDLK_q )
				{
					/*SDL_Event ev_quit;
					ev_quit.type = SDL_QUIT;

					SDL_PushEvent(&ev_quit);*/
					m_runFlag = false;
				} // if ( key == Q )
				break;
			}//switch-case
		}//while ( event )

		return 0;
	}

	int Draw()
	{
		if ( SDL_FillRect( m_sdlSurface
						, &m_rcSurface
						, SDL_MapRGB(m_sdlSurface->format, 0xA0, 0xA0, 0xA0)
						) != 0 )
		{
			return -1;
		}

		// Draw Things on "the Surface".
		if ( DrawTiles() != 0 )
		{
			return -2;
		}//DrawTiles()

		if ( DrawCharacters() != 0 )
		{
			return -2;
		}//DrawCharacters()

		// end of Draw.

		if ( SDL_UpdateWindowSurface( m_sdlWindow ) != 0)
		{
			return -3;
		}

		return 0;	// all succeed. good to go.
	}

	bool SetRunflag(bool b)	{			m_runFlag = b;				}	// Runflag-Setter
	bool GetRunflag() const	{			return m_runFlag;			}	// Runflag-Getter

private:
	inline int DrawCharacters()
	{
		for ( auto it = EntityMgr->m_EntityMap.begin()
			; it != EntityMgr->m_EntityMap.end()
			; ++it )
		{
			eDirection		dir = it->second->GetDirection();
			int				idx = it->second->GetFrameIndex();
			ML_POSPixel		pos = it->second->GetPixelPosition();

			SDL_Surface*	img = NULL;
			SDL_Rect		rc_src = { idx*ML_IMAGE_FRAME_SIZE
									, dir*ML_IMAGE_FRAME_SIZE
									, ML_IMAGE_FRAME_SIZE
									, ML_IMAGE_FRAME_SIZE };
			SDL_Rect		rc_pos = { pos.x
									, pos.y
									, ML_IMAGE_FRAME_SIZE
									, ML_IMAGE_FRAME_SIZE };

			switch ( it->first )
			{
			case ent_Miner_Bob:
				img = m_bmpCharacterBob;
				break;
			case ent_Elsa:
				img = m_bmpCharacterElsa;
				break;
			case ent_DrunkenMiner_Rob:
				img = m_bmpCharacterRob;
				break;
			}

			SDL_BlitSurface( img, &rc_src, m_sdlSurface, &rc_pos );
		}

		return 0;
	}

	//
	// @ DrawTiles
	//		*노트 : 2017-11-03
	//			그냥 '색칠' .. 이미지를 사용하면 예뻐 보일 듯.
	//			이미지도 RPGMakerVX 의 타일셋을 이용하자.
	//			여러 이미지가 겹치는 형태인 만큼, BITFLAG 등을 이용하면 코드가 예뻐보일 것 같다.
	//
	inline int DrawTiles()
	{
		for ( int r = 0; r < ML_BOARD_SIZE_ROW; ++r )
		{
			for ( int c = 0; c < ML_BOARD_SIZE_COL; ++c )
			{
				if ( g_boardLogic[r][c] == 1 )
				{
					SDL_FillRect( m_sdlSurface
								, &m_rcTile[r][c]
								, SDL_MapRGB( m_sdlSurface->format, 0x00, 0x00, 0x00 ) ); // errchk?
				}
			}
		}

		return 0;
	}
};

#endif // __ML_SDL_WRAPPER_H__





//int Render()
//{
//	if ( SDL_SetRenderDrawColor(m_sdlRenderer, 0xA0, 0xA0, 0xA0, SDL_ALPHA_OPAQUE) != 0)
//		return -1;

//	if ( SDL_RenderClear(m_sdlRenderer) != 0 )
//		return -1;


//	// Entity's Draw

//	SDL_RenderPresent(m_sdlRenderer);

//	return 0;
//}