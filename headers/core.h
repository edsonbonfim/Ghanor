#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "allegro5/define.h"
#include "allegro5/typedef.h"
#include "allegro5/init.h"
#include "allegro5/install.h"
#include "allegro5/load.h"
#include "allegro5/create.h"
#include "allegro5/set.h"

void install_allegro_components()
{
    install_mouse();
    install_keyboard();
}

void init_allegro_addons()
{
    al_init_video_addon();
    al_install_audio();
    al_init_font_addon();
    al_init_ttf_addon();
}
