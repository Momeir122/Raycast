#ifndef PLAYER_H
#define PLAYER_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include "types.h"
#include "utils.h"
#include "list.h"
#include <math.h>
#include <stdio.h>
#pragma GCC diagnostic pop

Player* create_player();
void destroy_player(Player* player);
void move_player(Player* player,Map map, int direction);
void rotate_player(Player* player, int direction);
void shoot_rays(Player* player, Map map);
void player_update(Player* player, Map map);
void player_gun_update(Gun* gun);
void player_shoot(Player* player);
int player_wall_collision(Player* player, Map map, float angle);

#endif