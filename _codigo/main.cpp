#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

#include <stdlib.h>
#include <time.h>

#include "Fila.cpp"
#include "Background.h"
#include "Gugu.h"
#include "Cepo.h"

#include <iostream>
using namespace std;

//Variaveis globais
const int WIDTH = 480;
const int HEIGHT = 660;

bool jaApertou = false;
float tempoRestante = 308 / 2;
float diminuiTempo = 1;
float aumentaTempo = 120;
bool perdeu = false;
int cepos = 0;
bool acertouCepo = false;

int cepoAnterior = 0;
int nivel = 1;
bool jorge = false;
int contNivel = 0;

enum STATE{TITLE, INSTRUCTIONS, PLAYING, END};
enum KEYS{LEFT, RIGHT};
bool keys[2] = {false, false};

Fila<int> fila;
Cepo cepo[12];

//Funcoes globais
void ChangeState(int &state, int newState);

int main(void){
	
	bool done = false;
	bool redraw = true;
	const int FPS = 30;
	int state = -1;
	int posX = 0;
	int posY = 0;
	
	bool apertouBotao = false;
	
	int contGeral = 0;
	int contAuxiliar = -12;
	int contAnimGugu = 0;
	int contSoltou = 0;
	int contErrado = 0;
	
	int cepoY = 555;
	
	bool deuCerto;
	int oi;
	
	srand(time(NULL));

	//object variables
	Background back;
	Gugu gugu;

	//Allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *fontTimes = NULL;
	ALLEGRO_FONT *fontTimesBig = NULL;
	ALLEGRO_SAMPLE *cepoRemix = NULL;
	ALLEGRO_SAMPLE *bateu = NULL;
	ALLEGRO_SAMPLE *perdeuSom = NULL;
	ALLEGRO_BITMAP *backImage = NULL;
	ALLEGRO_BITMAP *titleImage = NULL;
	ALLEGRO_BITMAP *jogarBotao = NULL;
	ALLEGRO_BITMAP *jogarMouse = NULL;
	ALLEGRO_BITMAP *guguImage = NULL;
	ALLEGRO_BITMAP *baseCepo = NULL;
	ALLEGRO_BITMAP *cepoSprite = NULL;
	ALLEGRO_BITMAP *instru = NULL;
	ALLEGRO_BITMAP *barraImage = NULL;
	ALLEGRO_BITMAP *boomImage = NULL;
	ALLEGRO_BITMAP *endImage = NULL;

	//Initialization Functions
	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if(!display)										//test display object
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	
	al_reserve_samples(10);
	
	cepoRemix = al_load_sample("audio/cepoRemix.ogg");
	bateu = al_load_sample("audio/bateu.ogg");
	perdeuSom = al_load_sample("audio/perdeuSom.ogg");
	backImage = al_load_bitmap("img/back.png");
	titleImage = al_load_bitmap("img/titleImage.png");
	jogarBotao = al_load_bitmap("img/jogarBotao.png");
	jogarMouse = al_load_bitmap("img/jogarMouse.png");
	guguImage = al_load_bitmap("img/guguImage.png");
	baseCepo = al_load_bitmap("img/base.png");
	cepoSprite = al_load_bitmap("img/spriteCepo.png");
	boomImage = al_load_bitmap("img/boomImage.png");
	instru = al_load_bitmap("img/instru.png");
	barraImage = al_load_bitmap("img/barraTempo.png");
	endImage = al_load_bitmap("img/endImage.png");
	
	fontTimes = al_load_font("fontes/timesbd.ttf", 28, 0);
	fontTimesBig = al_load_font("fontes/timesbd.ttf", 36, 0);

	ChangeState(state, TITLE);

	back.InitBackground(0, 0, 480, 660, backImage);
	gugu.InitGugu(guguImage);
	
	
	for(int i = 0; i < 12; i++){
		cepo[i].InitCepo(WIDTH / 2, cepoY, rand() % 2, i % 2, cepoSprite);
		cepoY -= 66;
		fila.Insere(i, deuCerto);
	}
	

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	
	al_play_sample(cepoRemix, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

	al_start_timer(timer);
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER){
			redraw = true;
			
			contGeral++;
			if(jaApertou){
				tempoRestante -= diminuiTempo;
			}
			
			if(tempoRestante <= 0){
				tempoRestante = 0;
			}else if(tempoRestante >= 308){
				tempoRestante = 308;
			}
	
			if(state == PLAYING){
				if(keys[LEFT]){
					gugu.Esquerda();
					contAuxiliar = contGeral;
					contAnimGugu = 0;
					jaApertou = true;
				}
				if(keys[RIGHT]){
					gugu.Direita();
					contAuxiliar = contGeral;
					contAnimGugu = 0;
					jaApertou = true;
				}
				
				
			}
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if(ev.mouse.button & 1){
				if(state == TITLE || state == INSTRUCTIONS || state == END){
					if(posX >= WIDTH / 2 - 65 && posX <= WIDTH / 2 + 65 && posY >= HEIGHT / 2 - 65 && posY <= HEIGHT / 2 + 65){
						switch(state){
							case TITLE:
								ChangeState(state, INSTRUCTIONS);
								break;
							case INSTRUCTIONS:
								ChangeState(state, PLAYING);
								break;
							case END:
								ChangeState(state, PLAYING);
								break;
						}
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}
				}
			}
		}else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			posX = ev.mouse.x;
			posY = ev.mouse.y;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_ESCAPE:
					done = true;
					break;
				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = true;
					break;
				case ALLEGRO_KEY_SPACE:
					if(state == TITLE){
						ChangeState(state, INSTRUCTIONS);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}else if(state == INSTRUCTIONS){
						ChangeState(state, PLAYING);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}if(state == END){
						ChangeState(state, PLAYING);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}
					break;
				case ALLEGRO_KEY_ENTER:
					if(state == TITLE){
						ChangeState(state, INSTRUCTIONS);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}else if(state == INSTRUCTIONS){
						ChangeState(state, PLAYING);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}if(state == END){
						ChangeState(state, PLAYING);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}
					break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP){
			switch(ev.keyboard.keycode){
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				if(state == PLAYING){
					contSoltou = contGeral;
					if(cepo[(fila.getPrimeiro())->info].getCurFrameX() == 0){
						perdeu = false;
						acertouCepo = true;
						fila.Retira(oi, deuCerto);
						for(int i = 0; i < 12; i++){
							cepo[i].setY(cepo[i].getY() + 66);
						}
						fila.Insere(oi, deuCerto);
						cepo[oi].setY(-171);
						cepo[oi].setCurFrameX(rand() % 2);
					}else{
						perdeu = true;
						contErrado = contGeral;
					}
					
					if(!perdeu)
						tempoRestante += aumentaTempo;
					if(acertouCepo)
						cepos++;
					acertouCepo = false;
				}
				
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				if(state == PLAYING){
					contSoltou = contGeral;
					if(cepo[(fila.getPrimeiro())->info].getCurFrameX() == 1){
						perdeu = false;
						acertouCepo = true;
						fila.Retira(oi, deuCerto);
						for(int i = 0; i < 12; i++){
							cepo[i].setY(cepo[i].getY() + 66);
						}
						fila.Insere(oi, deuCerto);
						cepo[oi].setY(-171);
						cepo[oi].setCurFrameX(rand() % 2);
					}else{
						perdeu = true;
						contErrado = contGeral;
					}
					if(!perdeu)
						tempoRestante += aumentaTempo;
					if(acertouCepo)
						cepos++;
					acertouCepo = false;
				}
				break;
			}
		}
		if(redraw && al_is_event_queue_empty(event_queue)){
			redraw = false; 
			back.DrawBackground();
			
			if(state == TITLE || state == INSTRUCTIONS || state == PLAYING || state == END){
				if(contGeral - contAuxiliar < 15){
					if(contGeral - contAuxiliar >= 0 && contGeral - contAuxiliar < 1){
						gugu.setCurFrameX(1);
					}else if(contGeral - contAuxiliar >= 1 && contGeral - contAuxiliar < 6){
						gugu.setCurFrameX(3);
					}else if(contGeral - contAuxiliar >= 6 && contGeral - contAuxiliar < 9){
						gugu.setCurFrameX(2);
					}else if(contGeral - contAuxiliar >= 9 && contGeral - contAuxiliar < 11){
						gugu.setCurFrameX(0);
					}
					contAnimGugu++;
				}else{
					if((contGeral / 15) % 2 == 0){
						gugu.setCurFrameX(0);
					}else{
						gugu.setCurFrameX(1);
					}
				}
		
			gugu.DrawGugu();
			
			al_draw_bitmap(baseCepo, WIDTH / 2 - 146 / 2, 660 - 72, 0);
				for(int i = 0; i < 12; i++){
					cepo[i].DrawCepo();
				}
			}
			
			if(state == TITLE || state == INSTRUCTIONS || state == END){
				if(posX >= WIDTH / 2 - 65 && posX <= WIDTH / 2 + 65 && posY >= HEIGHT / 2 - 65 && posY <= HEIGHT / 2 + 65){
					al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
					al_draw_bitmap(jogarMouse, WIDTH / 2 - 80, HEIGHT / 2 - 80, 0);
				}else{
					al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					al_draw_bitmap(jogarBotao, WIDTH / 2 - 75, HEIGHT / 2 - 75, 0);
				}
			}
			
			if(state == TITLE){
				al_draw_bitmap(titleImage, WIDTH / 2 - 143, 25, 0);
			}
			
			if(state == INSTRUCTIONS){
				al_draw_bitmap(instru, WIDTH / 2 - 110, 15, 0);
			}
			
			if(state == PLAYING){
				if(contGeral - contSoltou == 1){
					if(!perdeu){
						al_play_sample(bateu, 1.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					}else{
						al_play_sample(perdeuSom, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						ChangeState(state, END);
					}	
				}
				
				al_draw_bitmap(barraImage, WIDTH / 2 - 173, 10, 0);
				al_draw_filled_rectangle(19 + WIDTH / 2 - 173, 18 + 10, 19 + WIDTH / 2 - 173 + tempoRestante, 73 + 11, al_map_rgb(236,34,34));
				al_draw_textf(fontTimes, al_map_rgb(236,34,34), 4, 120, ALLEGRO_ALIGN_LEFT, "Cepos: %d", cepos);
				
				if(cepos >= cepoAnterior + 50){
					diminuiTempo += 0.15;
					cepoAnterior = cepos;
					jorge = true;
					nivel++;
					contNivel = contGeral;
				}
				
				if(contGeral - contNivel >= 0 && contGeral - contNivel <= 60 && jorge){
					al_draw_textf(fontTimesBig, al_map_rgb(255, 238, 164), WIDTH / 2, 140, ALLEGRO_ALIGN_CENTRE, "Nivel %d", nivel);
				}else{
					jorge = false;
				}
				
				if(tempoRestante <= 0){
					perdeu = true;
					al_play_sample(perdeuSom, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				}
				
				if(contGeral - contAuxiliar >= 1 && contGeral - contAuxiliar < 3 && !perdeu){
					al_draw_bitmap(boomImage, WIDTH / 2 - 139 / 2, 660 - 6 - 66 - 66, 0);
				}
				
				if(perdeu){
					ChangeState(state, END);
				}
			}
			
			if(state == END){
				al_draw_bitmap(endImage, WIDTH / 2 - 159, 25, 0);
				al_draw_textf(fontTimesBig, al_map_rgb(0, 0, 0), WIDTH / 2, 165, ALLEGRO_ALIGN_CENTRE, "%d", cepos);
				if(contGeral - contErrado == 1)
						al_play_sample(perdeuSom, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			}
			
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}
	
	al_destroy_bitmap(backImage);
	al_destroy_bitmap(titleImage);
	al_destroy_bitmap(guguImage);
	al_destroy_bitmap(jogarBotao);
	al_destroy_bitmap(jogarMouse);
	al_destroy_bitmap(baseCepo);
	al_destroy_bitmap(cepoSprite);
	al_destroy_bitmap(boomImage);
	al_destroy_bitmap(instru);
	al_destroy_bitmap(barraImage);
	al_destroy_bitmap(endImage);
	al_destroy_sample(cepoRemix);
	al_destroy_sample(bateu);
	al_destroy_sample(perdeuSom);
	al_destroy_font(fontTimes);
	al_destroy_font(fontTimesBig);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						//destroy our display object

	return 0;
}

void ChangeState(int &state, int newState){
	if(state == TITLE){
	}
	else if(state == PLAYING){
		jaApertou = false;
	}
	else if(state == INSTRUCTIONS){
	}
	else if(state == END){
	}

	state = newState;

	if(state == TITLE){
	}
	else if(state == PLAYING){
		tempoRestante = 308 / 2;
		diminuiTempo = 0.65;
		aumentaTempo = 4.5;
		perdeu = false;
		cepos = 0;
		acertouCepo = false;
		cepoAnterior = 0;
		nivel = 1;
		jorge = false;
		contNivel = 0;
		
		for(int i = 0; i < 12; i++){
			cepo[i].setCurFrameX(rand() % 2);
		}
	}
	else if(state == INSTRUCTIONS){
	}
	else if(state == END){
	}
}
