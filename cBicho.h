#pragma once

#include "cTexture.h"
#include "Globals.h"

#define FRAME_DELAY		4
#define STEP_LENGTH		BLOCK_SIZE/FRAME_DELAY
#define JUMP_HEIGHT		96
#define JUMP_STEP		4

#define STATE_IDLE			0
#define STATE_MOVE			1
#define STATE_ATTACK_1		2
#define STATE_ATTACK_2		3
#define STATE_SWORD			4

#define DIRECTION_DOWN			0
#define DIRECTION_UP			1
#define DIRECTION_LEFT		2
#define DIRECTION_RIGHT		3

#define	MONSTER_DOWN	0
#define	MONSTER_RIGHT	1
#define	MONSTER_UP		2
#define	MONSTER_LEFT	3

#define	MONSTER_STATE_IDLE	0
#define	MONSTER_STATE_MOVE	1

class cRect
{
public:
	int left,top,
		right,bottom;
};

class cBicho
{
public:
	cBicho(void);
	cBicho(int x,int y,int w,int h);
	~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y);
	void SetBlock(int tx,int ty);
	void GetBlock(int *tx,int *ty);

	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);

	bool Collides(cRect *rc);
	bool CollidesMapWall(int *map,bool right);
	void GetArea(cRect *rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void Jump(int *map);
	void Stop();
	void Logic(int *map);

	int  GetState();
	void SetState(int s);

	int  GetDirection();
	void SetDirection(int s);

	void NextFrame(int max);
	int  GetFrame();

	int GetSpeed();
	void SetSpeed(int s);

	bool isAlive();
	void setAlive(bool alive);
	
private:
	int x,y;
	int w,h;
	int direction;
	int state;
	int speed;

	int seq,delay;
	bool alive;
};
