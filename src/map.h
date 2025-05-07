#ifndef MAP_H
#define MAP_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include "types.h"
#include <stdlib.h>
#pragma GCC diagnostic pop

#define PLAYER_COLOR sfRed

void draw_map(sfRenderWindow* window, sfVector2f pos, sfVector2f size, Map map, Player* player);
void draw_player_inMap(sfRenderWindow* window, sfVector2f pos, sfVector2f size, Map map, Player* player);
void draw_player_rays_inMap(sfRenderWindow* window, sfVector2f pos, sfVector2f size, Map map, Player* player);
sfVector2f get_map_relative_pos(sfVector2f pos, sfVector2f display_size, Map map);
Map create_main_map();
void destroy_map(Map);

#endif 