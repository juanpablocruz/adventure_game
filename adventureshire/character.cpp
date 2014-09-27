#include "adventureshire.h"
#include "objects.h"

void initCharacter(Character &ch, ALLEGRO_BITMAP *tile) {
	ch.x = 185;
	ch.y = 218;
	ch.imgx = 0;
	ch.imgy = 0;
	ch.sizex = 32;
	ch.sizey = 64;
	ch.sizex2 = 70;
	ch.sizey2 = 152;
	ch.npc_id = 0;
	ch.speed = 2;

	ch.maxFrame = 7;
	ch.currFrame = 0;
	ch.frameCount = 0;
	ch.frameDelay = 12;
	ch.frameWidth = 31;
	ch.frameHeight = 64;
	ch.animationColumns = 7;
	ch.animationDirection = 1;
	ch.offsetx = -4;
	ch.offsety = 0;

	ch.animationRow = 1;
	ch.tile = tile;
}

void ResetCharacterAnimation(Character &ch, int position) {
	if (position == 1)
		ch.animationRow = 1;
	else
		ch.currFrame = 0;
}

void drawCharacter(Character &ch) {
	int fx = (ch.currFrame % ch.animationColumns) * ch.frameWidth;
	int fy = ch.animationRow * ch.frameHeight;

	al_draw_scaled_bitmap(ch.tile, fx + ch.offsetx, fy + ch.offsety,
		ch.sizex - 5, ch.sizey,
		ch.x, ch.y, ch.sizex2, ch.sizey2, 0);

	if (ch.currFrame == 0 && ch.animationRow == 1)
		al_draw_scaled_bitmap(ch.tile, 250, 64,
		10, 18,
		ch.x + 37, ch.y - 8, 25, 47, 0);
}

void drawSprite(struct Sprite &ch) {
	int fx = (ch.currFrame % ch.animationColumns) * ch.frameWidth;
	int fy = ch.animationRow * ch.frameHeight;

	al_draw_scaled_bitmap(ch.tile, fx + ch.offsetx, fy + ch.offsety,
		ch.frameWidth - 5, ch.frameHeight,
		ch.imgx, ch.imgy, ch.sizex2, ch.sizey2, 0);

}
void animateSprite(struct Sprite &ch){
	ch.frameCount++;
	if (ch.frameCount == ch.frameDelay) {
		ch.frameCount = 0;
		ch.currFrame++;
		if (ch.currFrame == ch.maxFrame)
			ch.currFrame = 0;
	}
}
void animateSprite(Character &ch){
	ch.frameCount++;
	if (ch.frameCount == ch.frameDelay) {
		ch.frameCount = 0;
		ch.currFrame++;
		if (ch.currFrame == ch.maxFrame)
			ch.currFrame = 0;
	}
}

void moveCharacter(Character &ch, int destX, int destY) {
	ch.animationRow = 1;

	ch.frameCount++;
	if (ch.frameCount == ch.frameDelay) {
		ch.frameCount = 0;
		ch.currFrame++;
		if (ch.currFrame == ch.maxFrame)
			ch.currFrame = 1;
	}

	if ((destX - ch.x) > ch.speed){
		ch.x += ch.speed;
	}
	else if ((ch.x - destX) > ch.speed){
		ch.x -= ch.speed;
	}
	else {
		ch.movement = false;
		ch.currFrame = 0;
	}
	if (ch.x > WIDTH - ch.sizex2)
		ch.x = WIDTH - ch.sizex2;
}