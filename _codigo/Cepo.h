#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

class Cepo{
	public:
		Cepo();
		void InitCepo(int, int, int, int, ALLEGRO_BITMAP *);
		void DrawCepo();
		
		void setCurFrameX(int);
		void setCurFrameY(int);
		void setY(int);
		
		int getCurFrameX();
		int getCurFrameY();
		int getY();
		
	private:
		int x;
		int y;
		
		int curFrameX;
		int curFrameY;
		int frameWidth;
		int frameHeight;
	
		ALLEGRO_BITMAP *image;
};
