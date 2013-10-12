
#include "cPlayer.h"
#include <math.h>

cPlayer::cPlayer() {
	espasa.alive =false;
	life = max_life = 6;
}
cPlayer::~cPlayer(){}

void DrawSword(int tex_id,float tx,float ty,float tw,float th,int x,int y,int w,int h){

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(tx,ty+th);	glVertex2i(x  ,y);
		glTexCoord2f(tx+tw,ty+th);	glVertex2i(x+w,y);
		glTexCoord2f(tx+tw,ty);	glVertex2i(x+w,y+h);
		glTexCoord2f(tx,ty);	glVertex2i(x  ,y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cPlayer::Draw(int tex_id,int obj_id){	
	int posx,posy;
	GetPosition(&posx,&posy);
	float bordeX = (0.+LINK_SIZE)/LINK_TEXTURES_WIDTH;
	float bordeY = (0.+LINK_SIZE)/LINK_TEXTURES_HEIGHT;
	float blockX = (0.+LINK_SIZE)/LINK_TEXTURES_WIDTH;
	float blockY = (0.+LINK_SIZE)/LINK_TEXTURES_HEIGHT;
	float xo,yo;
	float direction = GetDirection();
	float state = GetState();
	xo = direction*(blockX+bordeX);
	yo = state*(blockY+bordeY);
	if(state == STATE_ATTACK_1 || state == STATE_ATTACK_2){
		NextFrame(STATE_IDLE,STATE_ATTACK_2,2*FRAME_DELAY);
	}
	if (state == STATE_ATTACK_2){
		if(direction == DIRECTION_DOWN){
			yo -= (0.+LINK_DESFASE_ATTACK)/LINK_TEXTURES_HEIGHT;
			SetPosition(posx,posy-BLOCK_SIZE);
			yo+=blockY;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			yo-=blockY;
			SetPosition(posx,posy);
		}
		if(direction == DIRECTION_UP){
			yo += (0.+LINK_DESFASE_ATTACK)/LINK_TEXTURES_HEIGHT;
			SetPosition(posx,posy+BLOCK_SIZE);
			yo-=blockY;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			yo+=blockY;
			SetPosition(posx,posy);
		}
		if(direction == DIRECTION_LEFT){
			xo+= (0.+LINK_DESFASE_ATTACK)/LINK_TEXTURES_WIDTH;
			SetPosition(posx-BLOCK_SIZE,posy);
			xo-=blockX;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			xo+=blockX;
			SetPosition(posx,posy);
		}
		if(direction == DIRECTION_RIGHT){
			xo-= (0.+LINK_DESFASE_ATTACK)/LINK_TEXTURES_WIDTH;
			SetPosition(posx+BLOCK_SIZE,posy);
			xo+=blockX;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			xo-=blockX;
			SetPosition(posx,posy);
		}
	}
	DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
	if(espasa.alive){
		DrawSword(obj_id,espasa.direction/15.,espasa.state/4.,16./450.,1./8,espasa.x,espasa.y,BLOCK_SIZE,BLOCK_SIZE);
		espasa.x+= espasa.direction==DIRECTION_RIGHT?STEP_LENGTH:0;
		espasa.x-= espasa.direction==DIRECTION_LEFT?STEP_LENGTH:0;
		espasa.y+= espasa.direction==DIRECTION_UP?STEP_LENGTH:0;
		espasa.y-= espasa.direction==DIRECTION_DOWN?STEP_LENGTH:0;

		//espasa.state = (espasa.state+1)%4; //cambiarla de color
		if (espasa.x-BLOCK_SIZE <= 0 || espasa.y-BLOCK_SIZE<=0 || espasa.x >BLOCK_SIZE*SCENE_WIDTH||espasa.y >BLOCK_SIZE*SCENE_HEIGHT){//comprovar colisions amb enemics
			espasa.alive = false;
			//fer animacio de xoc			
		}
	}

	int pos = 18;
	for (int i = 1; i <= life; i++){
		DrawSword(obj_id,8./15.,0,16./450.,15./120.,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
		pos+=18;
	}
}

bool cPlayer::ataca(){
	int posx,posy;
	int direction = GetDirection();
	GetPosition(&posx,&posy);
	switch(direction){
		case DIRECTION_UP:
			if(posy+BLOCK_SIZE > BLOCK_SIZE*SCENE_HEIGHT)return false;
			if(!espasa.alive){
				espasa.direction = DIRECTION_UP;
				espasa.x=posx;
				espasa.y=posy+2*BLOCK_SIZE;
			}
			break;
		case DIRECTION_DOWN:
			if(posy-BLOCK_SIZE < 0)return false;
			if(!espasa.alive){
				espasa.direction = DIRECTION_DOWN;
				espasa.x=posx;
				espasa.y=posy-2*BLOCK_SIZE;
			}
			break;
		case DIRECTION_RIGHT:
			if(posx+BLOCK_SIZE > BLOCK_SIZE*SCENE_WIDTH)return false;
			if(!espasa.alive){
				espasa.direction = DIRECTION_RIGHT;
				espasa.x=posx+2*BLOCK_SIZE;
				espasa.y=posy;
			}
			break;
		case DIRECTION_LEFT:
			if(posx-BLOCK_SIZE < 0)return false;
			if(!espasa.alive){
				espasa.direction = DIRECTION_LEFT;
				espasa.x=posx-2*BLOCK_SIZE;
				espasa.y=posy;
			}
			break;
	}
	SetState(STATE_ATTACK_1);
	if(!espasa.alive){
		PlaySound("sounds\\sword_shoot.wav",NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP);
		espasa.alive=true;
		espasa.state=0;
	}

}
/* x = column
   y = row
   map = vector of tiles
*/
int checkPos(int x, int y, const int* map) {
	int bx,by;
	bx = x/BLOCK_SIZE;
	by = y/BLOCK_SIZE;
	return map[by*SCENE_WIDTH + bx ];
}

bool isWalkable(int tile) {
	if (tile == 2 || tile == 8 || tile == 14) return true;	//suelo
	
	return false;
}

// return
// true => same map
// false => change map
bool cPlayer::tirapalante(int* map){
	int x,y;
	int case1, case2;
	GetPosition(&x,&y);
	//TODO colisiones con enemigos
	switch (GetDirection()) {
	case DIRECTION_UP:
		if (y == SCENE_HEIGHT*BLOCK_SIZE) {
			SetPosition(x,SCENE_Yo);
			return false;
		}
		case1 = checkPos(x-BLOCK_SIZE, y, map);
		case2 = 2;
		if(x%BLOCK_SIZE != 0) {
			case2 = checkPos(x, y,map);
		}
		if ( !isWalkable(case1) || !isWalkable(case2)) return true;

		y+=GetSpeed();
		if (y > SCENE_HEIGHT*BLOCK_SIZE) return true;
		break;
	case DIRECTION_DOWN:
		if (y == SCENE_Yo) {
			SetPosition(x,BLOCK_SIZE*SCENE_HEIGHT);
			return false;
		}

		case1 = checkPos(x-BLOCK_SIZE, y-BLOCK_SIZE-1, map);
		case2 = 2;
		if(x%BLOCK_SIZE != 0) {
			case2 = checkPos(x, y-BLOCK_SIZE-1,map);
		}
		if ( !isWalkable(case1) || !isWalkable(case2)) return true;

		y-=GetSpeed();
		if (y < SCENE_Yo) return true;
		break;
	case DIRECTION_LEFT:
		if (x == SCENE_Xo) {
			SetPosition(BLOCK_SIZE*SCENE_WIDTH,y);
			return false;
		}

		case1 = checkPos(x-BLOCK_SIZE-1, y-BLOCK_SIZE, map);
		case2 = 2;
		if(y%BLOCK_SIZE != 0) {
			case2 = checkPos(x-BLOCK_SIZE-1, y,map);
		}
		if ( !isWalkable(case1) || !isWalkable(case2)) return true;

		x-=GetSpeed();
		if (x < SCENE_Xo) return true;
		break;
	case DIRECTION_RIGHT:
		if (x == BLOCK_SIZE*SCENE_WIDTH) {
			SetPosition(SCENE_Xo,y);
			return false;
		}

		case1 = checkPos(x, y-BLOCK_SIZE, map);
		case2 = 2;
		if(y%BLOCK_SIZE != 0) {
			case2 = checkPos(x, y,map);
		}
		if ( !isWalkable(case1) || !isWalkable(case2)) return true;

		x+=GetSpeed();
		if (x > SCENE_WIDTH*BLOCK_SIZE) return true;
		break;
	default:
		break;
	}
	SetPosition(x,y);
	NextFrame(STATE_MOVE,2,FRAME_DELAY);
	return true;
}

void cPlayer::heal(int num_hearts){//-1 -> full recovery
	if(num_hearts == -1) life = max_life;
	else life = min(max_life,life+num_hearts);
}
void cPlayer::damage(int num_hearts){ 
	life = max(0,life-num_hearts);
	PlaySound("sounds\\link_hurt.wav",NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP);
}
