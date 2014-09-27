#include <stdio.h>
#include "adventureshire.h"
#include "objects.h"

ALLEGRO_BITMAP *bar = NULL;
ALLEGRO_BITMAP *bar_pirate_conver = NULL;
ALLEGRO_BITMAP *character_image = NULL;
ALLEGRO_BITMAP *current_scene = NULL;
ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_TIMER* timer;
ALLEGRO_FONT* arial18;

ALLEGRO_SAMPLE *bar_song = NULL;
ALLEGRO_SAMPLE_INSTANCE *songInstance = NULL;

Character hero;
Conversacion pirataVentanaConver;

struct Sprite lampara;
struct Sprite pirataVentana;
struct Sprite pirataVentanafrente;
struct Sprite pirataInconsciente;
struct Sprite puerta;
double FPS = 60;

int done = false;
int redraw = true;
int privateScene = false;

int destX, destY;
int currAction;
int option_selected;

char target_action[50];
char full_action[120];

void checkMenu(int mx, int my);
void adjustSprite(struct Sprite &sp, int code);
void adjustSprite(Character &sp, int code);
void checkConver(int mx, int my);

int main(int argc, char **argv)
{
	display = NULL;
	event_queue = NULL;
	timer = NULL;
	option_selected = 0;
	strcpy_s(target_action, "");

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	display = al_create_display(WIDTH, HEIGHT);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

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

	srand(time(NULL));

	bar = al_load_bitmap("assets/MIScummbarSheet.png");
	character_image = al_load_bitmap("assets/GuybrushPart1.png");
	bar_pirate_conver = al_load_bitmap("assets/pirateconver.png");
	al_convert_mask_to_alpha(character_image, al_map_rgb(0, 0, 171));
	al_convert_mask_to_alpha(bar, al_map_rgb(4, 120, 76));


	initCharacter(hero, character_image);

	al_reserve_samples(2);
	bar_song = al_load_sample("assets/03-TheScummBar.ogg");

	songInstance = al_create_sample_instance(bar_song);

	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());

	lampara.tile = bar;
	lampara.imgy = 60;
	lampara.imgx = 419;
	lampara.animationColumns = 6;
	lampara.animationDirection = 1;
	lampara.animationRow = 3;
	lampara.currFrame = 0;
	lampara.frameCount = 0;
	lampara.frameDelay = 8;
	lampara.frameHeight = 50;
	lampara.frameWidth = 64;
	lampara.maxFrame = 6;
	lampara.sizex2 = 140;
	lampara.sizey2 = 136;
	lampara.offsetx = 3;
	lampara.offsety = -6;

	puerta.tile = bar;
	puerta.imgy = 189;
	puerta.imgx = 85;
	puerta.animationColumns = 6;
	puerta.animationDirection = 1;
	puerta.animationRow = 3;
	puerta.currFrame = 0;
	puerta.frameCount = 0;
	puerta.frameDelay = 8;
	puerta.frameHeight = 50;
	puerta.frameWidth = 64;
	puerta.maxFrame = 6;
	puerta.sizex2 = 153;
	puerta.sizey2 = 138;
	puerta.offsetx = 122;
	puerta.offsety = 101;
	puerta.draw = 0;

	pirataVentana.tile = bar;
	pirataVentana.imgy = 199;
	pirataVentana.imgx = 224;
	pirataVentana.animationColumns = 3;
	pirataVentana.animationDirection = 1;
	pirataVentana.animationRow = 3;
	pirataVentana.currFrame = 0;
	pirataVentana.frameCount = 0;
	pirataVentana.frameDelay = 10;
	pirataVentana.frameHeight = 48;
	pirataVentana.frameWidth = 40;
	pirataVentana.maxFrame = 3;
	pirataVentana.sizex2 = 93;
	pirataVentana.sizey2 = 142;
	pirataVentana.offsetx = 390;
	pirataVentana.offsety = 0;

	pirataVentanafrente.tile = bar;
	pirataVentanafrente.imgy = 211;
	pirataVentanafrente.imgx = 319;
	pirataVentanafrente.animationColumns = 3;
	pirataVentanafrente.animationDirection = 1;
	pirataVentanafrente.animationRow = 3;
	pirataVentanafrente.currFrame = 0;
	pirataVentanafrente.frameCount = 0;
	pirataVentanafrente.frameDelay = 10;
	pirataVentanafrente.frameHeight = 48;
	pirataVentanafrente.frameWidth = 48;
	pirataVentanafrente.maxFrame = 3;
	pirataVentanafrente.sizex2 = 116;
	pirataVentanafrente.sizey2 = 132;
	pirataVentanafrente.offsetx = 195;
	pirataVentanafrente.offsety = 50;

	pirataInconsciente.tile = bar;
	pirataInconsciente.imgy = 316;
	pirataInconsciente.imgx = 83;
	pirataInconsciente.animationColumns = 2;
	pirataInconsciente.animationDirection = 1;
	pirataInconsciente.animationRow = 1;
	pirataInconsciente.currFrame = 0;
	pirataInconsciente.frameCount = 0;
	pirataInconsciente.frameDelay = 15;
	pirataInconsciente.frameHeight = 24;
	pirataInconsciente.frameWidth = 41;
	pirataInconsciente.maxFrame = 2;
	pirataInconsciente.sizex2 = 98;
	pirataInconsciente.sizey2 = 64;
	pirataInconsciente.offsetx = 534;
	pirataInconsciente.offsety = 170;
		
	pirataVentanaConver.participantes = 2;
	pirataVentanaConver.frases_shown = (int*)malloc(sizeof(int)*4);
	pirataVentanaConver.frases_shown[0] = 0;
	pirataVentanaConver.frases_shown[1] = 1;
	pirataVentanaConver.frases_shown[2] = 2;
	pirataVentanaConver.frases_shown[3] = 3;

	pirataVentanaConver.frases_list[0][0] = { 0, 1,0, "Me llamo Guybrush Threepwood, Acabo de llegar al pueblo." };
	pirataVentanaConver.frases_list[0][1] = { 1, 1, 0, "Eres un pirata? Puedo formar parte de tu tripulacion?" };
	pirataVentanaConver.frases_list[0][2] = { 2, 1, 0, "Quien esta al mando aqui?" };
	pirataVentanaConver.frases_list[0][3] = { 3, 1, 0, "Ha sido un placer." };

	pirataVentanaConver.frases_list[1][0] = { 0, 1, 0, "Me llamo Guybrush Threepwood, Acabo de llegar al pueblo." };
	pirataVentanaConver.frases_list[1][1] = { 1, 1, 0, "Eres un pirata? Puedo formar parte de tu tripulación?" };
	pirataVentanaConver.frases_list[1][2] = { 2, 1, 0, "Quién está al mando aquí?" };
	pirataVentanaConver.frases_list[1][3] = { 3, 1, 0, "Ha sido un placer." };

	currAction = walk_act;
	current_scene = bar;

	arial18 = al_load_font("assets/ONESIZE_.ttf", 18, 0);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);
	al_play_sample_instance(songInstance);
	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			adjustSprite(puerta, ev.keyboard.keycode);
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
			switch (privateScene){
			case 0:
				if (currAction == talk_act || currAction == look_act){
					if (ev.mouse.x >= 235 && ev.mouse.x <= 275 && ev.mouse.y >= 219 && ev.mouse.y <= 272) {
						strcpy_s(target_action, " Pirate");
					}
					else if (ev.mouse.x > 419 && ev.mouse.x < 537 && ev.mouse.y > 60 && ev.mouse.y < 196) {
						strcpy_s(target_action, " Drunken Pirate");
					}
					else if (strcmp(target_action, "") != 0) {
						strcpy_s(target_action, "");
					}
				}
				else if ((currAction == open_act || currAction == close_act) && ev.mouse.x >= 85 && ev.mouse.x <= 176 && ev.mouse.y >= 206 && ev.mouse.y <= 309) {
					strcpy_s(target_action, " Door");
				}
				else if (strcmp(target_action, "") != 0) {
					strcpy_s(target_action, "");
				}
				break;
			case 1:
				checkConver(ev.mouse.x, ev.mouse.y);
				break;
			}

			
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			printf("%d %d\n", ev.mouse.x, ev.mouse.y);
			if (ev.mouse.y >= HEIGHT - 100 ){
				checkMenu(ev.mouse.x, ev.mouse.y);
				
			}
			else {
				switch (currAction) {
				case walk_act:
					destX = ev.mouse.x;
					destY = ev.mouse.y;
					hero.movement = true;
					break;
				case open_act:
					if (ev.mouse.x >= 85 && ev.mouse.x <= 176 && ev.mouse.y >= 206 && ev.mouse.y <= 309) {
						puerta.draw = 1;
					}
					break;
				case close_act:
					if (ev.mouse.x >= 85 && ev.mouse.x <= 176 && ev.mouse.y >= 206 && ev.mouse.y <= 309) {
						puerta.draw = 0;
					}
					break;
				case talk_act:
					if (ev.mouse.x >=235 && ev.mouse.x <= 275 && ev.mouse.y >= 219 && ev.mouse.y <= 272) {
						printf("Talking to %s\n", target_action);
						current_scene = bar_pirate_conver;
						privateScene = 1;
					}
				default:
					if (ev.mouse.x > 419 && ev.mouse.x < 537 && ev.mouse.y > 60 && ev.mouse.y < 196) {
						strcpy_s(target_action, " Drunken Pirate");
					}
					break;
				}

			}
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			if (privateScene == 0) {
				al_draw_scaled_bitmap(bar, 0, 0,
					240,
					144, 0, 0, WIDTH, HEIGHT - 100, 0);

				animateSprite(lampara);
				animateSprite(pirataVentana);
				animateSprite(pirataVentanafrente);
				if (hero.movement)
					moveCharacter(hero, destX, destY);

				if (puerta.draw == 1)
					drawSprite(puerta);
				drawSprite(lampara);
				drawSprite(pirataVentana);
				drawSprite(pirataVentanafrente);
				drawCharacter(hero);
				drawSprite(pirataInconsciente);

				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 10, HEIGHT - 85, 0, "Open");
				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 10, HEIGHT - 70, 0, "Close");
				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 10, HEIGHT - 55, 0, "Push");
				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 10, HEIGHT - 40, 0, "Pull");

				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 100, HEIGHT - 85, 0, "Walk to");
				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 100, HEIGHT - 70, 0, "Pick up");
				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 100, HEIGHT - 55, 0, "Talk to");
				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 100, HEIGHT - 40, 0, "Give");

				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 200, HEIGHT - 85, 0, "Use");
				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 200, HEIGHT - 70, 0, "Look at");
				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 200, HEIGHT - 55, 0, "Turn on");
				al_draw_textf(arial18, al_map_rgb(0, 125, 0), 200, HEIGHT - 40, 0, "Turn off");

				strcpy_s(full_action, ActionsStr[currAction]);
				strcat_s(full_action, target_action);
				al_draw_textf(arial18, al_map_rgb(0, 200, 200), (WIDTH / 2) - 20, HEIGHT - 100, 0, full_action);
			}
			else {
				al_draw_scaled_bitmap(current_scene, 0, 0, al_get_bitmap_width(current_scene), al_get_bitmap_height(current_scene), 0, 0, 3*WIDTH/4, 3*(HEIGHT - 100)/4, 0);
				
				int i = 0;
				for (i = 0; i < 4; i++) {
					if (pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[i]].selected)
						al_draw_textf(arial18, al_map_rgb(0, 125, 125), 10, HEIGHT - (90 - (20*i)), 0, pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[i]].frases);
					else
						al_draw_textf(arial18, al_map_rgb(0, 125, 0), 10, HEIGHT - (90 - (20 * i)), 0, pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[i]].frases);
				}
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

	al_destroy_sample(bar_song);
	al_destroy_sample_instance(songInstance);
	al_destroy_bitmap(bar);
	al_destroy_bitmap(character_image);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_font(arial18);
	al_destroy_display(display);

	return 0;
}

void checkMenu(int mx, int my) {
	if (mx > 0 && mx < 60) {
		if (my > HEIGHT - 90 && my < HEIGHT - 70){
			currAction = open_act;
			strcpy_s(target_action, "");
		}
		else if (my > HEIGHT - 75 && my < HEIGHT - 55){
			currAction = close_act;
			strcpy_s(target_action, "");
		}
		else if (my > HEIGHT - 60 && my < HEIGHT - 40){
			currAction = push_act;
			strcpy_s(target_action, "");
		}
		else if (my > HEIGHT - 45 && my < HEIGHT - 25){
			currAction = pull_act;
			strcpy_s(target_action, "");
		}
	}
	else if (mx > 60 && mx < 180) {
		if (my > HEIGHT - 90 && my < HEIGHT - 70){
			currAction = walk_act;
			strcpy_s(target_action, "");
		}
		else if (my > HEIGHT - 75 && my < HEIGHT - 55){
			currAction = pick_act;
			strcpy_s(target_action, "");
		}
		else if (my > HEIGHT - 60 && my < HEIGHT - 40){
			currAction = talk_act;
			strcpy_s(target_action, "");
		}
		else if (my > HEIGHT - 45 && my < HEIGHT - 25){
			currAction = give_act;
			strcpy_s(target_action, "");
		}
	}
	else if (mx > 200 && mx < 280) {
		if (my > HEIGHT - 90 && my < HEIGHT - 70){
			currAction = use_act;
			strcpy_s(target_action, "");
		}
		else if (my > HEIGHT - 75 && my < HEIGHT - 55){
			currAction = look_act;
			strcpy_s(target_action, "");
		}
		else if (my > HEIGHT - 60 && my < HEIGHT - 40){
			currAction = turnon_act;
			strcpy_s(target_action, "");
		}
		else if (my > HEIGHT - 45 && my < HEIGHT - 25){
			currAction = turnof_act;
			strcpy_s(target_action, "");
		}
	}
}


void checkConver(int mx, int my) {
	if (my > HEIGHT - 90 && my < HEIGHT - 70){
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[0]].selected = 1;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[1]].selected = 0;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[2]].selected = 0;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[3]].selected = 0;
	}
	else if (my > HEIGHT - 70 && my < HEIGHT - 50){
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[0]].selected = 0;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[1]].selected = 1;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[2]].selected = 0;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[3]].selected = 0;
	}
	else if (my > HEIGHT - 60 && my < HEIGHT - 40){
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[0]].selected = 0;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[1]].selected = 0;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[2]].selected = 1;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[3]].selected = 0;
	}
	else if (my > HEIGHT - 40 && my < HEIGHT - 20){
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[0]].selected = 0;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[1]].selected = 0;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[2]].selected = 0;
		pirataVentanaConver.frases_list[0][pirataVentanaConver.frases_shown[3]].selected = 1;
	}
}


void adjustSprite(struct Sprite &sp, int code) {
	switch (code)
	{
	case ALLEGRO_KEY_A:
		sp.sizex2--;
		break;
	case ALLEGRO_KEY_W:
		sp.sizey2++;
		break;
	case ALLEGRO_KEY_S:
		sp.sizey2--;
		break;
	case ALLEGRO_KEY_D:
		sp.sizex2++;
		break;
	case ALLEGRO_KEY_J:
		sp.frameWidth--;
		break;
	case ALLEGRO_KEY_I:
		sp.frameHeight++;
		break;
	case ALLEGRO_KEY_K:
		sp.frameHeight--;
		break;
	case ALLEGRO_KEY_L:
		sp.frameWidth++;
		break;
	case ALLEGRO_KEY_F:
		sp.offsetx++;
		break;
	case ALLEGRO_KEY_T:
		sp.offsety--;
		break;
	case ALLEGRO_KEY_G:
		sp.offsety++;
		break;
	case ALLEGRO_KEY_H:
		sp.offsetx--;
		break;

	case ALLEGRO_KEY_LEFT:
		sp.imgx--;
		break;
	case ALLEGRO_KEY_UP:
		sp.imgy++;
		break;
	case ALLEGRO_KEY_DOWN:
		sp.imgy--;
		break;
	case ALLEGRO_KEY_RIGHT:
		sp.imgx++;
		break;
	default:
		break;
	}
	printf("imgx:%d imgy:%d frameWidth:%d frameHeight:%d sizex:%d sizey:%d offsetx:%d offsety:%d\n",
		sp.imgx, sp.imgy,
		sp.frameWidth, sp.frameHeight,
		sp.sizex2, sp.sizey2,
		sp.offsetx, sp.offsety);
}

void adjustSprite(Character &sp, int code) {
	switch (code)
	{
	case ALLEGRO_KEY_A:
		sp.sizex2--;
		break;
	case ALLEGRO_KEY_W:
		sp.sizey2++;
		break;
	case ALLEGRO_KEY_S:
		sp.sizey2--;
		break;
	case ALLEGRO_KEY_D:
		sp.sizex2++;
		break;
	case ALLEGRO_KEY_J:
		sp.frameWidth--;
		break;
	case ALLEGRO_KEY_I:
		sp.frameHeight++;
		break;
	case ALLEGRO_KEY_K:
		sp.frameHeight--;
		break;
	case ALLEGRO_KEY_L:
		sp.frameWidth++;
		break;
	case ALLEGRO_KEY_F:
		sp.offsetx++;
		break;
	case ALLEGRO_KEY_T:
		sp.offsety--;
		break;
	case ALLEGRO_KEY_G:
		sp.offsety++;
		break;
	case ALLEGRO_KEY_H:
		sp.offsetx--;
		break;
	case ALLEGRO_KEY_LEFT:
		sp.x--;
		break;
	case ALLEGRO_KEY_UP:
		sp.y++;
		break;
	case ALLEGRO_KEY_DOWN:
		sp.y--;
		break;
	case ALLEGRO_KEY_RIGHT:
		sp.x++;
		break;
	default:
		break;
	}
	printf("x:%d y:%d frameWidth:%d frameHeight:%d sizex:%d sizey:%d offsetx:%d offsety:%d\n",
		sp.x, sp.y,
		sp.frameWidth, sp.frameHeight,
		sp.sizex2, sp.sizey2,
		sp.offsetx, sp.offsety);
}
