#ifndef TYPES_H
#define TYPES_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#pragma GCC diagnostic pop

typedef struct Map_Cell_Info{
    int isWall;
    sfColor color;
    sfTexture* render_texture;
}Map_Cell_Info;

typedef struct Map{
    int** cells;
    unsigned int width; unsigned int height;
    float cell_real_size;
    Map_Cell_Info* cells_infos;
}Map;

typedef struct Player_Stats{
    float speed;
}Player_Stats;

typedef struct Player_Settings{
    float sensibility;
    float FOV;
    unsigned int n_rays;
}Player_Settings;

typedef struct Ray_Point{
    unsigned int index;
    sfVector2f pos;
    int value;
}Ray_Point;

typedef struct element_ray_point{
    Ray_Point value;
    struct element_ray_point* next;
}element_ray_point;

typedef struct Gun{
    sfTexture* texture;
    sfTexture* idle_texture;
    sfTexture* shoot_texture;
    float shoot_time;   // Time of the shoot animation
    sfClock* shoot_clock;
    float shoot_delay_time; // Delay between every shots
    sfClock* shoot_delay_clock;
    float head_bob;
    float head_bob_amplitude;
    float head_bob_intensity;
}Gun;

typedef struct Player{
    sfVector2f pos;
    float angle;
    Gun gun;
    element_ray_point* rays;
    Player_Stats stats;
    Player_Settings settings;
}Player;

typedef struct Entity{
    sfVector2f pos;
    sfTexture* idle_texture;
}Entity;

#endif