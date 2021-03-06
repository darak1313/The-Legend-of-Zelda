#pragma once
#include "cbicho.h"
#include "utils.h"

#define	ENEMY_DOWN	0
#define	ENEMY_LEFT	1
#define	ENEMY_UP	2
#define	ENEMY_RIGHT	3

#define	ENEMY_STATE_IDLE	0
#define	ENEMY_STATE_MOVE	1

#define FILE_PROP_OCTOROK_B	"enemies/octorok-b.txt"
#define FILE_PROP_FAT_DOG_O "enemies/fat_dog-o.txt"

#define OCTOROK_B	"octorok-b"
#define FAT_DOG_O	"fat_dog-o"

#define RAND	0

#define ROCK_ID	0

#define ENEMY_SIZE	16
#define ENEMY_TEXTURE_WIDTH 64
#define ENEMY_TEXTURE_HEIGHT 32

class cEnemy :
	public cBicho
{
public:
	cEnemy(void);
	cEnemy(int x, int y, char *type, int tex_id,int movementDelay);
	~cEnemy(void);

	void draw();
	void process();
	int getIA();
	void setIA(int IA);
	int SetNewDirection();
	int Damage();
	int getDrop();
	int shoot();
	int getDamage();

	void setMaxLife(int lifes);
	void setDamage(int damage);
	void setTex_ID(int tex_id);
	void setMovementDelay(int movDelay);
	void setMovementSeq(int moveSeq);
	int getTex_ID();
	int getMovementDelay();
	int getMovementSeq();
	void setType(char *type);
	int getLife();

private:
	void readEnemyProps(FILE *propFile);
	int maxLife;
	char *type;
	int damage;
	int typeProyectile;
	float probShoot;
	float probDropHeart;
	float probDropRuppe;
	float probDropKey;
	int tex_id;
	int IA;
	int ratio;
	int movementDelay;
	int movementSeq;
	
	int changeDirectionDelay;
protected:
	bool imBoss;
	int life;
};

