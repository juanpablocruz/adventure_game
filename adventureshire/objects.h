#ifndef _OBJECTS_H
#define _OBJECTS_H

#include "adventureshire.h"



struct Character{
	ALLEGRO_BITMAP *tile;
	int imgx, imgy, x, y, sizex, sizey, sizex2, sizey2, npc_id;
	int maxFrame, currFrame, frameCount, frameDelay, frameWidth, frameHeight, animationColumns, animationDirection;
	int animationRow;
	int offsetx, offsety;
	int speed;
	unsigned char movement;
};

struct Sprite{
	ALLEGRO_BITMAP *tile;
	int imgx, imgy, sizex2, sizey2;
	int maxFrame, currFrame, frameCount, frameDelay, frameWidth, frameHeight, animationColumns, animationDirection;
	int animationRow;
	int offsetx, offsety;
	int draw;
};

struct Frase {
	int ID_COMP;
	int ID_RESPUESTA;
	int selected;
	char *frases;
};
struct Conversacion {
	int participantes;
	int* frases_shown;
	Frase frases_list[2][4];
};



void loadTileset(Character &ch);
void initCharacter(Character &ch, ALLEGRO_BITMAP *tile);
void drawCharacter(Character &ch);
void ResetCharacterAnimation(Character &ch, int position);
void moveCharacter(Character &ch, int destx, int desty);
void animateSprite(Character &ch);
void animateSprite(struct Sprite &ch);
void drawSprite(struct Sprite &ch);
#endif