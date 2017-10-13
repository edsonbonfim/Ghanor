int fadein(ALLEGRO_BITMAP *imagem, int time)
{
    static int fade = 0;

    if (fade == 1)
        return 0;

    if (time < 0)
        time = 1;

    else if (time > 15)
        time = 15;

    int alfa;

    for (alfa = 0; alfa <= 255; alfa += time)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(imagem, al_map_rgba(alfa, alfa, alfa, alfa), 0, 0, 0);
        //al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 200));
        al_flip_display();
        al_rest(0.005); // Não é necessário caso haja controle de FPS
    }

    fade++;

    return 1;
}

static void choose_shader_source(ALLEGRO_SHADER *shader, char const **vsource, char const **psource)
{
      ALLEGRO_SHADER_PLATFORM platform = al_get_shader_platform(shader);
      
      if (platform == ALLEGRO_SHADER_HLSL)
      {
            *vsource = "data/ex_shader_vertex.hlsl";
            *psource = "data/ex_shader_pixel.hlsl";
      }
      
      else if (platform == ALLEGRO_SHADER_GLSL)
      {
            *vsource = "data/ex_shader_vertex.glsl";
            *psource = "data/ex_shader_pixel.glsl";
      }
      
      else
      {
            *vsource = NULL;
            *psource = NULL;
      }
}

ALLEGRO_BITMAP *load_bitmap_at_size(const char *filename, int w, int h)
{
    ALLEGRO_BITMAP *resized_bmp, *loaded_bmp, *prev_target;

    resized_bmp = al_create_bitmap(w, h);
    if (!resized_bmp)
        return NULL;

    loaded_bmp = al_load_bitmap(filename);
    if (!loaded_bmp)
    {
        al_destroy_bitmap(resized_bmp);
        return NULL;
    }

    prev_target = al_get_target_bitmap();
    al_set_target_bitmap(resized_bmp);

    al_draw_scaled_bitmap(loaded_bmp,
                          0, 0,                             // source origin
                          al_get_bitmap_width(loaded_bmp),  // source width
                          al_get_bitmap_height(loaded_bmp), // source height
                          0, 0,                             // target origin
                          w, h,                             // target dimensions
                          0                                 // flags
                          );

    al_set_target_bitmap(prev_target);

    return resized_bmp;
}