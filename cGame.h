#pragma once

#include "cScene.h"
#include "cData.h"
#include "cPlayer.h"
#include "cSound.h"
#include "utils.h"
#include <map>
#include <shlobj.h>


#define TRANS_MAINMENU	7

#define NEW_GAME		0
#define LOAD_GAME		3
#define OPTIONS			1
#define EXIT			2


#define MUSIC_VOLUME	0
#define EFFECTS_VOLUME	1
#define BACK			2

using namespace std;

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	bool startGame();
	bool startInstructions();
	bool Loop();
	void Finalize();
	void GameOver();
	void saveSettings();
	void loadSettings();

	//Input
	void ReadKeyboard(unsigned char key, bool press);
	void ReadSpecialKeyboard(unsigned char specialkey, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	//Output
	void Render();

private:
	void drawInstructions(float r, float g, float b);
	void drawCredits(char *text[], float r, float g, float b);
	bool finalGame();
	void saveGame();
	void loadGame();
	bool checkSavedGame();

	void loadPathSaveGame();
	void loadPathSaveOptions();

	unsigned char keys[256];
	unsigned char specialKeys[256];
	cScene Scene;
	cData Data;
	cPlayer Link;
	cSound sound;
	map<char,int> sounds;

	bool mainMenu;
	bool optMenu;
	int nTransMM;
	int currentMM;
	bool up;
	String menuText[4];
	String unLockedLevels[NUM_MAX_GATES];
	String triforcesCollected[NUM_MAX_GATES];
	int currentOptMM;
	int currentPauseOpt;

	bool pause;
	bool gameOver;
	bool mainMenuProcess();
	bool instructions;
	bool gameFinal;

	int nSecInstructions;
	int nSecCreditsDuration;

	struct Options {
		float effectVolume;
		float musicVolume;
	} options;

	char *credits[18];	//3 lines per 6 views
	int actualCreditView;
	
	bool existsSavedGame;

	char *pathSaveGame;
	char *pathSaveOptions;
};
