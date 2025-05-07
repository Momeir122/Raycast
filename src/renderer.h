#ifndef RENDERER_H
#define RENDERER_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include "types.h"
#include "utils.h"
#pragma GCC diagnostic pop

void render_raycast(sfRenderWindow* window, sfVector2f pos, sfVector2f size, Map map, Player* player);
void draw_player_gun(sfRenderWindow* window, sfVector2f pos, sfVector2f size, Player* player);
void draw_floor_ceiling(sfRenderWindow* window, sfVector2f pos, sfVector2f size);

#endif