#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

class Gugu{
	public:
		Gugu();
		void InitGugu(ALLEGRO_BITMAP *);
		void DrawGugu();
		void Esquerda();
		void Direita();
		
		void setCurFrameX(int);
		
	private:	
		int x;
		int y;
		
		int curFrameX;
		int curFrameY;
		int frameWidth;
		int frameHeight;
	
		ALLEGRO_BITMAP *image;
};
