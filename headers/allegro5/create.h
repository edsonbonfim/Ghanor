DISPLAY * create_display(int w, int h)
{
    DISPLAY * display;
    
    if (!(display = al_create_display(w, h)))
    {
        fprintf(stderr, "Falha ao criar display!\n");
        exit(-1);
    }
    
    return display;
}

TIMER * create_timer(float fps)
{
    TIMER * timer;
    if (!(timer = al_create_timer(fps)))
    {
        fprintf(stderr, "Falha ao criar timer!\n");
        exit(-1);
    }

    return timer;
}

BITMAP * create_bitmap(int w, int h)
{
    BITMAP * bitmap;

    if (!(bitmap = al_create_bitmap(w, h)))
    {
        fprintf(stderr, "Falha ao criar bitmap!\n");
        exit(-1);
    }

    return bitmap;
}

EVENT_QUEUE * create_event_queue()
{
    EVENT_QUEUE * event_queue;

    if (!(event_queue = al_create_event_queue()))
    {
        fprintf(stderr, "Falha ao inicializar a fila de eventos!\n");
        exit(-1);
    }

    return event_queue;
}