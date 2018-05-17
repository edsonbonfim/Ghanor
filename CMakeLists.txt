cmake_minimum_required(VERSION 3.10)
project(Ghanor C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)

add_executable(Ghanor ${SOURCE_FILES})

INCLUDE_DIRECTORIES( usr/include/allegro5 )

LINK_DIRECTORIES( usr/lib )


TARGET_LINK_LIBRARIES(Ghanor  allegro_acodec
        allegro_audio
        allegro_color
        allegro_dialog
        allegro_image
        allegro_main
        allegro_memfile
        allegro_physfs
        allegro_primitives
        allegro_ttf
        allegro_video
        allegro_font
        allegro)