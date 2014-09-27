#ifndef _ADVENTURE_SHIRE_H
#define _ADVENTURE_SHIRE_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

const int WIDTH = 640;
const int HEIGHT = 480;

enum Keys{UP, DOWN, LEFT, RIGHT, SPACE};
enum Actions{ open_act, close_act, push_act, pull_act, walk_act, pick_act, talk_act, give_act, use_act, look_act, turnon_act, turnof_act };

static char *ActionsStr[] = { "Open", "Close", "Push", "Pull", "Walk to", "Pick up", "Talk to", "Give", "Use", "Look at", "Turn on", "Turn off" };

#endif