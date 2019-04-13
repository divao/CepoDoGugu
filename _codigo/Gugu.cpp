#include "Gugu.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

Gugu::Gugu(){
	this->x = 0;
	this->y = 390;
	this->curFrameX = 0;
	this->curFrameY = 0;
	this->frameWidth = 168;
	this->frameHeight = 252;
	this->image = NULL;
}

void Gugu::InitGugu(ALLEGRO_BITMAP *image){
	this->x = 0; //posicao x na esquerda
	this->y = 390; //posicao y na esquerda
	this->curFrameX = 0;
	this->curFrameY = 0;
	this->frameWidth = 168;
	this->frameHeight = 252;
	this->image = image;
}

void Gugu::DrawGugu(){
	al_draw_bitmap_region(image, curFrameX * frameWidth, curFrameY * frameHeight, frameWidth, frameHeight, x, y, 0);
}

void Gugu::Esquerda(){
	this->x = 0; //posicao x na esquerda
	this->y = 390; //posicao y na esquerda
	this->curFrameY = 0;
}

void Gugu::Direita(){
	this->x = 312; //posicao x na direita
	this->y = 390; //posicao y na direita
	this->curFrameY = 1;
}

void Gugu::setCurFrameX(int curFrameX){
	this->curFrameX = curFrameX;
}
