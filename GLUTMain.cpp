
#include "Globals.h"
#include "cGame.h"

//Delete console
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

cGame Game;

void AppRender()
{
	Game.Render();
}
void AppKeyboard(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key,true);
}
void AppKeyboardUp(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key,false);
}
void AppSpecialKeys(int key, int x, int y)
{
	Game.ReadSpecialKeyboard(key,true);
}
void AppSpecialKeysUp(int key, int x, int y)
{
	Game.ReadSpecialKeyboard(key,false);
}
void AppMouse(int button, int state, int x, int y)
{
	Game.ReadMouse(button,state,x,y);
}
void AppIdle()
{
	if(!Game.Loop()) Game.Finalize();
}

void createWindow(bool fullScreen,int x,int y){
	if(fullScreen){
		char buffer[42];
		sprintf(buffer,"%dx%d:32",x,y);
		glutGameModeString(buffer);
		glutEnterGameMode();
		//PlaySound("sounds\\02_overworld_theme.wav",NULL,SND_LOOP|SND_FILENAME|SND_ASYNC);
	}
	else{
		glutInitWindowPosition(x,y);
		glutInitWindowSize(GAME_WIDTH,GAME_HEIGHT);
		glutCreateWindow("The Legend of Zelda");
		PlaySound("sounds\\02_overworld_theme.wav",NULL,SND_LOOP|SND_FILENAME|SND_ASYNC);
		HWND windowHandle = FindWindow(NULL,"The Legend of Zelda");
		HICON hIcon = (HICON)LoadImage(NULL, "images\\/window_logo.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(windowHandle,WM_SETICON, ICON_BIG, (LPARAM)hIcon );
	}
}

void main(int argc, char** argv)
{
	bool fullScreen=false;
	int res_x,res_y,pos_x,pos_y;

	//GLUT initialization
	glutInit(&argc, argv);


	//RGBA with double buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);

	//Create centered window
	res_x = glutGet(GLUT_SCREEN_WIDTH);
	res_y = glutGet(GLUT_SCREEN_HEIGHT);
	pos_x = (res_x>>1)-(GAME_WIDTH>>1);
	pos_y = (res_y>>1)-(GAME_HEIGHT>>1);

	if(argc > 1){
		if(argv[1]=="TRUE"){
			fullScreen = true;
			createWindow(true,res_x,res_y);
		}
		else{
			createWindow(false,pos_x,pos_y);
		}
	}
	else{
		createWindow(false,pos_x,pos_y);
	}

	//Register callback functions
	glutDisplayFunc(AppRender);			
	glutKeyboardFunc(AppKeyboard);		
	glutKeyboardUpFunc(AppKeyboardUp);	
	glutSpecialFunc(AppSpecialKeys);	
	glutSpecialUpFunc(AppSpecialKeysUp);
	glutMouseFunc(AppMouse);
	glutIdleFunc(AppIdle);

	//Game initializations
	if (!Game.Init()){
		int msgboxID = MessageBox(
			NULL,
			"Error in the init",
			"Error",
			MB_OKCANCEL | MB_ICONERROR
		);
		switch (msgboxID)
		{
		case IDCANCEL:
			Game.Finalize();
			break;
		case IDCONTINUE:
			
			break;
		}
	}

	//Application loop
	glutMainLoop();
}
