
#include "cPlayer.h"
#include <math.h>

cPlayer::cPlayer() {
	espasa.alive =false;
	life = 3;
	max_life = 6;
	points = 0;
	keys = 3;
	triforce = 0;
	max_triforces = 3;
}
cPlayer::~cPlayer(){}

void DrawObject(int tex_id,int x,int y,int posx,int posy){
	float th = 1/5.;
	float tw = 1/5.;
	float tx = x/5.;
	float ty = y/5.;

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(tx,ty+th);	glVertex2i(posx  ,posy);
		glTexCoord2f(tx+tw,ty+th);	glVertex2i(posx+BLOCK_SIZE,posy);
		glTexCoord2f(tx+tw,ty);	glVertex2i(posx+BLOCK_SIZE,posy+BLOCK_SIZE);
		glTexCoord2f(tx,ty);	glVertex2i(posx  ,posy+BLOCK_SIZE);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void print( int x, int y, char *st){
	int i,len;
	len = strlen(st);
	//glColor3f(1.,0.,0.);
	glRasterPos2i( x, y);
	for( i=0; i < len; i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
	}
}

void cPlayer::printInfo(int obj_id){
	int i;
	int pos = BLOCK_SIZE;
	for (i = 0; i < max_life; i+=2){
		if (i+1 == life)DrawObject(obj_id,1,1,pos,(SCENE_HEIGHT)*BLOCK_SIZE + 21);
		else if(life > i) DrawObject(obj_id,0,1,pos,(SCENE_HEIGHT)*BLOCK_SIZE + 21); 
		else DrawObject(obj_id,2,1,pos,(SCENE_HEIGHT)*BLOCK_SIZE + 21);
		pos+=18;
	}
	DrawObject(obj_id,3,0,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE);
	pos+=BLOCK_SIZE;
	char *buffer = (char*)malloc(42);
	sprintf(buffer,"X%d",points);
	print(pos,(SCENE_HEIGHT+1)*BLOCK_SIZE+(BLOCK_SIZE/4),buffer);
	pos += strlen(buffer)*10;
	DrawObject(obj_id,2,0,pos,(SCENE_HEIGHT+1)*BLOCK_SIZE);
	pos+=BLOCK_SIZE-3;
	sprintf(buffer,"X%d",keys);
	print(pos,(SCENE_HEIGHT+1)*BLOCK_SIZE+(BLOCK_SIZE/4),buffer);
	pos += strlen(buffer)*15;
	free(buffer);
	for(i=0;i<max_triforces;i++){
		if(triforce>i)DrawObject(obj_id,0,2,pos,(SCENE_HEIGHT)*BLOCK_SIZE+27);
		else
		DrawObject(obj_id,0,3,pos,(SCENE_HEIGHT)*BLOCK_SIZE+27);
		pos += 28;
	}
}

void cPlayer::Draw(int tex_id,int obj_id){	
	int posx,posy;
	GetPosition(&posx,&posy);
	float bordeX = (0.+LINK_SIZE)/LINK_TEXTURES_WIDTH;
	float bordeY = (0.+LINK_SIZE)/LINK_TEXTURES_HEIGHT;
	float blockX = (0.+LINK_SIZE)/LINK_TEXTURES_WIDTH;
	float blockY = (0.+LINK_SIZE)/LINK_TEXTURES_HEIGHT;
	float xo,yo;
	int direction = GetDirection();
	float state = GetState();
	xo = direction*(blockX+bordeX);
	yo = state*(blockY+bordeY);
	if(state == STATE_ATTACK_1 || state == STATE_ATTACK_2){
		NextFrame(STATE_IDLE,STATE_ATTACK_2,2*FRAME_DELAY);
	}
	if (state == STATE_ATTACK_2){
		switch (direction){
		case DIRECTION_DOWN:
			yo -= (0.+LINK_DESFASE_ATTACK)/LINK_TEXTURES_HEIGHT;
			SetPosition(posx,posy-BLOCK_SIZE);
			yo+=blockY;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			yo-=blockY;
			break;
		case DIRECTION_UP:
			yo += (0.+LINK_DESFASE_ATTACK)/LINK_TEXTURES_HEIGHT;
			SetPosition(posx,posy+BLOCK_SIZE);
			yo-=blockY;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			yo+=blockY;
			break;
		case DIRECTION_LEFT:
			xo+= (0.+LINK_DESFASE_ATTACK)/LINK_TEXTURES_WIDTH;
			SetPosition(posx-BLOCK_SIZE,posy);
			xo-=blockX;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			xo+=blockX;
			break;
		case DIRECTION_RIGHT:
			xo-= (0.+LINK_DESFASE_ATTACK)/LINK_TEXTURES_WIDTH;
			SetPosition(posx+BLOCK_SIZE,posy);
			xo+=blockX;
			DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
			xo-=blockX;
			break;
		}
		SetPosition(posx,posy);
		if(!espasa.alive){
			//TODO: cambiar por FMOD
			
			PlaySound("sounds\\LOZ_Sword_shoot.wav",NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP);
			espasa.alive=true;
			espasa.state=0;
		}
	}
	DrawRect(tex_id,xo,yo + blockY,xo + blockX,yo);
	if(espasa.alive){
		DrawObject(obj_id,espasa.direction,4,espasa.x,espasa.y);
		espasa.x+= espasa.direction==DIRECTION_RIGHT?STEP_LENGTH:0;
		espasa.x-= espasa.direction==DIRECTION_LEFT?STEP_LENGTH:0;
		espasa.y+= espasa.direction==DIRECTION_UP?STEP_LENGTH:0;
		espasa.y-= espasa.direction==DIRECTION_DOWN?STEP_LENGTH:0;

		//espasa.state = (espasa.state+1)%4; //cambiarla de color
		if (espasa.x-BLOCK_SIZE <= 0 || espasa.y-BLOCK_SIZE<=0 || espasa.x >BLOCK_SIZE*SCENE_WIDTH-BLOCK_SIZE/2||espasa.y >BLOCK_SIZE*SCENE_HEIGHT-BLOCK_SIZE/2){//comprovar colisions amb enemics
			espasa.alive = false;
			//fer animacio de xoc			
		}
	}
	printInfo(obj_id);
}

bool cPlayer::ataca(){
	int posx,posy;
	int state = GetState();
	int direction = GetDirection();
	if (!(state == STATE_IDLE || state==STATE_MOVE))return false;
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
	return true;
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

int cPlayer::heal(int num_hearts){//-1 -> full recovery
	if(num_hearts == -1) life = max_life;
	else life = min(max_life,life+num_hearts);
	return life;
}
int cPlayer::damage(int num_hearts){ 
	life = max(0,life-num_hearts);
	return life;
}

void cPlayer::givePoints(int num_points){
	points+=num_points;
}

bool cPlayer::useKey(){
	if(keys > 0){
		keys--;
		return true;
	}
	return false;
}
void cPlayer::getKey(){
	keys++;
}

