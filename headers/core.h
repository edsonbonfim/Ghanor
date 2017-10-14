#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "utils.c"

#include "allegro5/define.h"
#include "allegro5/typedef.h"
#include "allegro5/init.h"
#include "allegro5/install.h"
#include "allegro5/load.h"
#include "allegro5/create.h"
#include "allegro5/set.h"

//#include "../algif5/algif.h"

DISPLAY *display;

int DISPLAY_W;
int DISPLAY_H;

const int FPS = 60;

#define SHOW_VIDEO 0

#if SHOW_VIDEO

#include <allegro5/allegro_video.h>

typedef ALLEGRO_VIDEO VIDEO;

int redraw;
EVENT event_video;
VIDEO *video;
TIMER *timer;
QUEUE *queue_video;

void load_video()
{
    redraw = 1;

    video = al_open_video("videos/video.ogv");
    timer = al_create_timer(1.0 / FPS);

    queue_video = create_event_queue();
    al_register_event_source(queue_video, al_get_video_event_source(video));
    al_register_event_source(queue_video, al_get_timer_event_source(timer));

    al_start_video(video, al_get_default_mixer());
    al_start_timer(timer);
}

void close_video()
{
    al_close_video(video);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue_video);
}

void g_event_video()
{
    al_wait_for_event(queue_video, &event_video);

    if (event_video.type == ALLEGRO_EVENT_TIMER)
        redraw = true;
}

static void video_display(ALLEGRO_VIDEO *video)
{
    int w, h, x, y;

    ALLEGRO_BITMAP *frame = al_get_video_frame(video);

    if (!frame)
        return;

    w = DISPLAY_W + 300;
    h = DISPLAY_H + 300;

    x = (DISPLAY_W - w) / 2;
    y = (DISPLAY_H - h) / 2;

    al_draw_scaled_bitmap(frame, 0, 0, al_get_bitmap_width(frame), al_get_bitmap_height(frame), x, y, w, h, 0);
}

#else

ALLEGRO_BITMAP * bg;

void load_background()
{
    bg = load_bitmap_at_size("images/background.png", DISPLAY_W, DISPLAY_H);
}

#endif

void install_allegro_components()
{
    install_mouse();
    install_keyboard();
    al_install_audio();
}

void init_allegro_addons()
{
#if SHOW_VIDEO
    al_init_video_addon();
#endif

    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
}
