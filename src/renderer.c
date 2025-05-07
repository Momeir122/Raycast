#include "renderer.h"

#define WALL_SCALE 30
#define GUN_SCALE_X 1000
#define GUN_SCALE_Y 600
#define FLOOR_COLOR sfColor_fromRGB(28, 87, 22)
#define ZOOM_WALL_DISTANCE 40

void draw_player_gun(sfRenderWindow* window, sfVector2f pos, sfVector2f size, Player* player){
    sfSprite* gun_sprite = sfSprite_create();
    sfSprite_setTexture(gun_sprite, player->gun.texture, sfFalse);

    float gun_scale_x =  size.x/GUN_SCALE_X;
    float gun_scale_y = size.y / GUN_SCALE_Y;
    float head_bob_dy = player->gun.head_bob_amplitude+ player->gun.head_bob_amplitude*sinf(player->gun.head_bob);

    sfSprite_setPosition(gun_sprite, (sfVector2f){pos.x + size.x - gun_scale_x*sfTexture_getSize(player->gun.texture).x, pos.y + size.y - gun_scale_y*sfTexture_getSize(player->gun.texture).y + head_bob_dy});
    sfSprite_setScale(gun_sprite, (sfVector2f){gun_scale_x,gun_scale_y});

    sfRenderWindow_drawSprite(window, gun_sprite, NULL);

    sfSprite_destroy(gun_sprite);
}

void draw_floor_ceiling(sfRenderWindow* window, sfVector2f pos, sfVector2f size){
    sfRectangleShape* rect_floor_ceiling = sfRectangleShape_create();
    sfRectangleShape_setSize(rect_floor_ceiling, (sfVector2f){size.x, size.y/2});

    sfRectangleShape_setPosition(rect_floor_ceiling, (sfVector2f){pos.x,pos.y + size.y/2});
    sfRectangleShape_setFillColor(rect_floor_ceiling, FLOOR_COLOR);
    sfRenderWindow_drawRectangleShape(window, rect_floor_ceiling, NULL);

    sfSprite* sky_sprite = sfSprite_create();
    sfTexture* sky_texture = sfTexture_createFromFile("images/sky.jpg", NULL);
    sfSprite_setTexture(sky_sprite, sky_texture, sfFalse);
    sfSprite_setPosition(sky_sprite, pos);
    sfSprite_setScale(sky_sprite, (sfVector2f){size.x/sfTexture_getSize(sky_texture).x, size.y/(sfTexture_getSize(sky_texture).y*2)});

    sfRenderWindow_drawSprite(window, sky_sprite, NULL);

    sfSprite_destroy(sky_sprite);
    sfTexture_destroy(sky_texture);
    sfRectangleShape_destroy(rect_floor_ceiling);
}

void render_raycast(sfRenderWindow* window, sfVector2f pos, sfVector2f size, Map map, Player* player){
    draw_floor_ceiling(window, pos, size);

    element_ray_point* player_rays = player->rays;

    float dx = size.x / player->settings.n_rays;

    unsigned int i_ray = 0;
    int ray_value = -1;
    int ray_x = -1;
    int ray_y = -1;
    while(player_rays != NULL){
        Ray_Point ray_point = player_rays->value;

        if(ray_point.value == ray_value && (abs((int)(ray_point.pos.x / map.cell_real_size) - ray_x) > 1 || abs((int)(ray_point.pos.y / map.cell_real_size) - ray_y) > 1)){
            i_ray = 0;
        }
        
        ray_value = ray_point.value;
        ray_x = (int)(ray_point.pos.x / map.cell_real_size);
        ray_y = (int)(ray_point.pos.y / map.cell_real_size);

        float ray_dist = dist(player->pos, ray_point.pos);

        float size_x = size.x/player->settings.n_rays;
        float size_y = (WALL_SCALE*size.y/ray_dist);
        float pos_x = pos.x + ray_point.index*dx;
        float pos_y = (size.y - size_y)/2;

        sfSprite* ray_sprite = sfSprite_create();
        sfSprite_setTexture(ray_sprite,map.cells_infos[ray_point.value].render_texture, sfFalse);
        sfSprite_setPosition(ray_sprite, (sfVector2f){pos_x, pos_y});
        sfSprite_setScale(ray_sprite, (sfVector2f){1.0,size_y/ sfTexture_getSize(sfSprite_getTexture(ray_sprite)).y});

        int texture_dx = (int) (sfTexture_getSize(sfSprite_getTexture(ray_sprite)).x/map.cell_real_size)*size_x/size.x*1400;
        int texture_x = (texture_dx*i_ray)%sfTexture_getSize(sfSprite_getTexture(ray_sprite)).x;

        sfSprite_setTextureRect(ray_sprite, (sfIntRect){texture_x,0, texture_dx,sfTexture_getSize(sfSprite_getTexture(ray_sprite)).y});

        sfRenderWindow_drawSprite(window, ray_sprite, NULL);
        sfSprite_destroy(ray_sprite);

        i_ray++;

        player_rays = player_rays->next;
    }

    draw_player_gun(window, pos, size, player);
}