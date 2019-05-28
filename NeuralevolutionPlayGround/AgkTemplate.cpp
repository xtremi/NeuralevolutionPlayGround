// Includes
#include "AgkTemplate.h"

// Namespace
using namespace AGK;

app App;


void app::Begin(void)
{
	int vwdim[2] = { DEVICE_WIDTH, DEVICE_HEIGHT };
	agk::SetVirtualResolution(vwdim[0], vwdim[1]);
	//agk::SetDisplayAspect(1024.0f/768.0f);
	agk::UseNewDefaultFonts(1);
	agk::SetClearColor(151, 170, 204); // light blue
	agk::SetSyncRate(60, 0);
	agk::SetScissor(0, 0, 0, 0);

	gamePlay.init(vwdim[0], vwdim[1]);

	gameState = game_state::PAUSE;
}

int app::Loop (void)
{
	agk::Print(agk::Str(agk::ScreenFPS(), 0));

	if(gameState == game_state::PLAYING){
		gamePlay.update();
	}
	else if (gameState == game_state::PAUSE) {
		agk::StepPhysics(0);
	}


	agk::Sync();
	return 0; // return 1 to close a|pp
}

void app::End (void){}

void app::tooglePause() {
	if (gameState == game_state::PAUSE)
		gameState = game_state::PLAYING;
	else if (gameState == game_state::PLAYING)
		gameState = game_state::PAUSE;		
	
}
