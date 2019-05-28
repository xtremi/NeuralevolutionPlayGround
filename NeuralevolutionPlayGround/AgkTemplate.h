#ifndef _H_AGK_TEMPLATE_
#define _H_AGK_TEMPLATE_

// Link to GDK libraries
#include "AGK.h"
#include "GamePlay.h"

/*#include <QWidget>
#include <QAbstractNativeEventFilter>*/


int APIENTRY agkMain(HWND windowHandle);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


/*class MyEventFilter : public QAbstractNativeEventFilter
{
public:
	MyEventFilter(HWND _hwnd) { hwnd = _hwnd; }

public:
	HWND hwnd;

	bool nativeEventFilter(const QByteArray &eventType, void *message, long* res) override
	{
		if (eventType == "windows_generic_MSG") {

			HWND hwnd1 = hwnd;
			HWND hwnd2 = ((MSG*)message)->hwnd;

			res = (long*)WndProc(hwnd1, ((MSG*)message)->message, ((MSG*)message)->wParam, ((MSG*)message)->lParam);
			if (!res)return true;
		}
		return false;
	}
};*/


#define DEVICE_WIDTH 1000
#define DEVICE_HEIGHT 800
#define DEVICE_POS_X 32
#define DEVICE_POS_Y 32
#define FULLSCREEN false

// used to make a more unique folder for the write path
#define COMPANY_NAME "My Company"

enum class game_state { NONE, PLAYING, EDITING, PAUSE };

// Global values for the app
class app
{
	public:

		// constructor
		app() { memset ( this, 0, sizeof(app)); }

		// main app functions - mike to experiment with a derived class for this..
		void Begin( void );
		int Loop( void );
		void End( void );

		void stop() { endAgkLoop = true; }
		bool endAgkLoop = false;

		game_state currentGameState() { return gameState; }

public:
	GamePlay gamePlay;

	void tooglePause();

private:
	game_state gameState;

};

extern app App;

#endif

// Allow us to use the LoadImage function name
#ifdef LoadImage
 #undef LoadImage
#endif