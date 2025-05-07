#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include "map.h"
#include "player.h"
#include "types.h"
#include "renderer.h"
#include <stdlib.h>
#include <stdio.h>
#pragma GCC diagnostic pop

#define WIN_WIDTH 1400
#define WIN_HEIGHT 800
#define WIN_BACKGROUND_COLOR sfBlack
#define FPS 60

sfRenderWindow* create_window(const char* title, unsigned int width, unsigned int height){
    sfVideoMode video_mode = {width, height, 32};
    return sfRenderWindow_create(video_mode, title, sfDefaultStyle, NULL);
}

void handle_windowEvents(sfRenderWindow* window){
    sfEvent event;
    while(sfRenderWindow_pollEvent(window, &event)){
        if(event.type == sfEvtClosed){
            sfRenderWindow_close(window);
        }
    }
}

void handle_inputs(Player* player, Map map){
    if(sfKeyboard_isKeyPressed(sfKeyZ)){
        move_player(player, map, 1);
    }
    if(sfKeyboard_isKeyPressed(sfKeyS)){
        move_player(player, map,-1);
    }
    if(sfKeyboard_isKeyPressed(sfKeyQ)){
        rotate_player(player, -1);
    }
    if(sfKeyboard_isKeyPressed(sfKeyD)){
        rotate_player(player, 1);
    }
    if(sfKeyboard_isKeyPressed(sfKeySpace)){
        player_shoot(player);
    }
}

int main(void){
    sfRenderWindow* window = create_window("Raycast", WIN_WIDTH, WIN_HEIGHT);

    Map map = create_main_map();

    Player* player = create_player();

    while(sfRenderWindow_isOpen(window)){
        sfRenderWindow_setFramerateLimit(window, FPS);
        sfRenderWindow_clear(window, WIN_BACKGROUND_COLOR);

        handle_windowEvents(window);
        handle_inputs(player, map);

        render_raycast(window, (sfVector2f){0,0}, (sfVector2f){WIN_WIDTH, WIN_HEIGHT}, map, player);
        draw_map(window, (sfVector2f){0,0}, (sfVector2f){WIN_WIDTH/4, WIN_HEIGHT/4}, map, player);
        player_update(player, map);

        sfRenderWindow_display(window);
    }

    /* Destroying Everything */
    sfRenderWindow_destroy(window);
    destroy_player(player);
    destroy_map(map);

    return 0;
}