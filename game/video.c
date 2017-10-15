#include <allegro5/allegro_video.h>
#include "video.h"

int redraw;

ALLEGRO_EVENT event_video;
ALLEGRO_VIDEO *video;
ALLEGRO_TIMER *timer;
ALLEGRO_EVENT_QUEUE *queue_video;

void load_video()
{
    redraw = 1;

    video = al_open_video("data/videos/video.ogv");
    timer = al_create_timer(1.0 / FPS);

    queue_video = al_create_event_queue();
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