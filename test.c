#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_video.h>

#include <stdio.h>

static ALLEGRO_DISPLAY *screen;
char filename[20];

static void video_display(ALLEGRO_VIDEO *video)
{
      ALLEGRO_BITMAP * frame = al_get_video_frame(video);
      int w, h, x, y;

      if (!frame)
            return;

      w = (int) al_get_video_scaled_width(video);
      h = (int) al_get_video_scaled_height(video);

      x = (al_get_display_width(screen) - w) / 2;
      y = (al_get_display_height(screen) - h) / 2;

      /* Display the frame. */
      al_draw_scaled_bitmap(frame, 0, 0,
                            al_get_bitmap_width(frame),
                            al_get_bitmap_height(frame), x, y, w, h, 0);

      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));
}

int main()
{
      ALLEGRO_EVENT_QUEUE *queue;
      ALLEGRO_EVENT event;
      ALLEGRO_TIMER *timer;
      ALLEGRO_VIDEO *video;
      bool redraw = true;

      al_init();
      al_init_video_addon();
      al_install_keyboard();
      al_install_audio();
      al_reserve_samples(1);

      timer = al_create_timer(1.0 / 60);

      al_set_new_display_flags(ALLEGRO_RESIZABLE);
      al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
      
      screen = al_create_display(640, 480);

      //al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

      strcpy(filename, "video.ogv");
      video = al_open_video(filename);

      queue = al_create_event_queue();
      al_register_event_source(queue, al_get_video_event_source(video));
      al_register_event_source(queue, al_get_timer_event_source(timer));

      al_start_video(video, al_get_default_mixer());
      al_start_timer(timer);

      while (true)
      {
            if (redraw && al_event_queue_is_empty(queue))
            {
                  video_display(video);
                  redraw = false;
            }

            al_wait_for_event(queue, &event);
            switch (event.type)
            {
            case ALLEGRO_EVENT_TIMER:
                  if (!false)
                  {
                        redraw = true;
                  }
                  break;
            }
      }

      al_destroy_display(screen);
      return 0;
}