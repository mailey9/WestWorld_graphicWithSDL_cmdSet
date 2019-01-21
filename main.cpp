#define ML_SHOW_SDL_WINDOW
#define ML_SHOW_TIMESTAMP


//////////////////////////////////////////////////////////////////////////////////
//							SDL license agreement.								//
//////////////////////////////////////////////////////////////////////////////////
/*	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.	*/
//////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <time.h>

#include "Locations.h"

#include "ML_Miner.h"			//#include "Miner.h"
#include "ML_MinersWife.h"		//#include "MinersWife.h"
#include "ML_Rob.h"

#include "ML_EntityManager.h"	//#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

#ifdef ML_SHOW_SDL_WINDOW
	#define SDL_MAIN_HANDLED
	#include "SDLwrapper.h"
#endif

#include <Windows.h>

std::ofstream os;

int main(int argc, char* args[])
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
	os.open("output.txt");
#endif
#ifdef ML_SHOW_SDL_WINDOW
	SDLwrapper tst;
	tst.Initialize();
#endif
	system("color 07");

	//seed random number generator
	srand((unsigned) time(NULL));

	//create a miner
	Miner* Bob = new Miner( ent_Miner_Bob, ML_POSMat(3,4) );

	//create his wife
	MinersWife* Elsa = new MinersWife( ent_Elsa, ML_POSMat(1,2) );

	Rob* drunkenMiner = new Rob( ent_DrunkenMiner_Rob, ML_POSMat(1,22) );

	//register them with the entity manager
	EntityMgr->RegisterEntity( Bob );
	EntityMgr->RegisterEntity( Elsa );
	EntityMgr->RegisterEntity( drunkenMiner );

	////run Bob and Elsa through a few Update calls -- EDITED with BusyLoop
	int			updateCount		= 0;
	DWORD		timeLast		= GetTickCount();

	while ( updateCount < 30000 )				// Loop it 30 times.
	{ 
		DWORD timeCurr = GetTickCount();

		if ( (timeCurr - timeLast) >= 30 )	// Updates Every 800ms.
		{
			timeLast = timeCurr;

			Bob->Update();			//printf("[Bob]  %i %i\n", Bob->GetMatrixPosition().row, Bob->GetMatrixPosition().col);
			Elsa->Update();			//printf("[Elsa] %i %i\n", Elsa->GetMatrixPosition().row, Elsa->GetMatrixPosition().col);
			drunkenMiner->Update();

			//dispatch any delayed messages
			Dispatch->DispatchDelayedMessages();

			++updateCount;
		}

#ifdef ML_SHOW_SDL_WINDOW
		// for Rendering! /w SDL		..
		if ( tst.GetRunflag() == true )
		{
			int retVal = -1;

			if (tst.HandleSDL() != 0)
				; // errchk
			if (tst.Draw() != 0)				//if (tst.Render() != 0)
				; // errchk
		}
		else
		{
			tst.Finalize();
		}
#endif

		if (_kbhit())
		{
			char ch = _getch();

			if (ch == 'c')
			{
				break;
			}
			else if (ch == 's')
			{
				Sleep( 3000 );
			}
		}
	}

	//tidy up
	delete Bob;
	delete Elsa;

#ifdef ML_SHOW_SDL_WINDOW
	if ( tst.GetRunflag() == true )
	{
		tst.Finalize();
	}
#endif


	//wait for a keypress before exiting
	PressAnyKeyToContinue();


	return 0;
}

////		Original Loop			//
//for (int i=0; i<30; ++i)
//{ 
//  Bob->Update();
//  Elsa->Update();

//  //dispatch any delayed messages
//  Dispatch->DispatchDelayedMessages();

//  Sleep(800);
//}
///////////////////////////////////////