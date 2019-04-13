#include "Cepo.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

#define WIDTH 480

Cepo::Cepo(){
	this->x = WIDTH / 2;
	this->y = 66 - 66 - 6;
	this->curFrameX = 0;
	this->curFrameY = 0;
	this->frameWidth = 146;
	this->frameHeight = 66;
	this->image = NULL;
}

void Cepo::InitCepo(int x, int y, int curFrameX, int curFrameY, ALLEGRO_BITMAP *image){
	this->x = x;
	this->y = y;
	this->curFrameX = curFrameX;
	this->curFrameY = curFrameY;
	this->frameWidth = 146;
	this->frameHeight = 66;
	this->image = image;
}

void Cepo::DrawCepo(){
	al_draw_bitmap_region(image, curFrameX * frameWidth, curFrameY * frameHeight, frameWidth, frameHeight, x - frameWidth / 2, y - frameHeight / 2, 0);
}

void Cepo::setCurFrameX(int curFrameX){
	this->curFrameX = curFrameX;
}

void Cepo::setCurFrameY(int curFrameY){
	this->curFrameY = curFrameY;
}

int Cepo::getCurFrameX(){
	return this->curFrameX;
}

int Cepo::getCurFrameY(){
	return this->curFrameY;
}

void Cepo::setY(int y){
	this->y = y;
}

int Cepo::getY(){
	return this->y;
}
