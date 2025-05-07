#include "player.h"

#define RAY_SPEED 1
#define RAY_MAX_DISTANCE 100
#define WALL_COLLISION_DISTANCE 20

Player* create_player(){
    Player_Stats player_stats = {1.0};
    Player_Settings player_settings = {3, 60, 200};

    Gun player_gun;
    player_gun.idle_texture = sfTexture_createFromFile("images/guns/gun.png", NULL);
    player_gun.shoot_texture = sfTexture_createFromFile("images/guns/gun_shoot.png", NULL);
    player_gun.texture = player_gun.idle_texture;
    player_gun.shoot_time = 100;
    player_gun.shoot_clock = NULL;
    player_gun.shoot_delay_time = 200;
    player_gun.shoot_delay_clock = NULL;
    player_gun.head_bob = 0;
    player_gun.head_bob_amplitude = 20;
    player_gun.head_bob_intensity = 0.1;

    Player* player = malloc(sizeof(Player));
    player->pos = (sfVector2f){150,150};
    player->angle = 70;
    player->rays = NULL;
    player->settings = player_settings;
    player->stats = player_stats;
    player->gun = player_gun;

    return player;
}

void destroy_player(Player* player){
    sfTexture_destroy(player->gun.idle_texture);
    sfTexture_destroy(player->gun.shoot_texture);
    free(player);
}

void move_player(Player* player,Map map, int direction){
    float dx = cosf(deg_to_rad(player->angle))*player->stats.speed;
    float dy = sinf(deg_to_rad(player->angle))*player->stats.speed;

    if(!player_wall_collision(player,map, player->angle + (direction==-1)*180)){
        player->pos.x += dx*direction;
        player->pos.y += dy*direction;

        player->gun.head_bob += player->gun.head_bob_intensity;
        if(player->gun.head_bob > 2*PI){
            player->gun.head_bob = 0;
        }
    }
}

void rotate_player(Player* player, int direction){
    player->angle += player->settings.sensibility * direction;
}

int player_wall_collision(Player* player , Map map, float angle){
    float dx = cosf(deg_to_rad(angle)) * RAY_SPEED;
    float dy = sinf(deg_to_rad(angle)) * RAY_SPEED;

    sfVector2f ray_pos = player->pos;

    int ray_value = map.cells[(int)(ray_pos.x/map.cell_real_size)][(int)(ray_pos.y/map.cell_real_size)];

    float ray_distance = 0;

    while(ray_distance < WALL_COLLISION_DISTANCE){
        if(map.cells_infos[ray_value].isWall){
            break;
        }

        ray_pos.x += dx;
        ray_pos.y += dy;

        if((int)(ray_pos.x/map.cell_real_size) < map.width && (int)(ray_pos.y/map.cell_real_size) < map.height){
            ray_value = map.cells[(int)(ray_pos.x/map.cell_real_size)][(int)(ray_pos.y/map.cell_real_size)];
        }
        else{
            break;
        }
        ray_distance = dist(player->pos, ray_pos);
    }

    return !(ray_distance >= WALL_COLLISION_DISTANCE);
}

void player_gun_update(Gun* gun){
    if(gun->shoot_clock == NULL){
        gun->texture = gun->idle_texture;
    }
    else{
        gun->texture = gun->shoot_texture;
    }

    if(gun->shoot_clock != NULL){
        if(sfTime_asMilliseconds(sfClock_getElapsedTime(gun->shoot_clock)) > gun->shoot_time){
            sfClock_destroy(gun->shoot_clock);

            gun->shoot_clock = NULL;

            gun->shoot_delay_clock = sfClock_create();
        }
    }

    if(gun->shoot_delay_clock != NULL){
        if(sfTime_asMilliseconds(sfClock_getElapsedTime(gun->shoot_delay_clock)) > gun->shoot_delay_time){
            sfClock_destroy(gun->shoot_delay_clock);

            gun->shoot_delay_clock = NULL;
        }
    }
}

void player_shoot(Player* player){
    if(player->gun.shoot_clock == NULL && player->gun.shoot_delay_clock == NULL){
        player->gun.shoot_clock = sfClock_create();
    }
}

void player_update(Player* player, Map map){
    shoot_rays(player,map);

    player_gun_update(&player->gun);
}

void shoot_rays(Player* player, Map map){
    if(player->rays != NULL){
        player->rays = list_ray_point_destroy(player->rays);
    }

    float ray_angle = player->angle - player->settings.FOV/2;
    float dAngle = player->settings.FOV / player->settings.n_rays;

    for(unsigned int i_ray = 0; i_ray < player->settings.n_rays; i_ray ++){
        float dx = cosf(deg_to_rad(ray_angle)) * RAY_SPEED;
        float dy = sinf(deg_to_rad(ray_angle)) * RAY_SPEED;

        sfVector2f ray_pos = player->pos;

        int ray_value = map.cells[(int)(ray_pos.x/map.cell_real_size)][(int)(ray_pos.y/map.cell_real_size)];

        float ray_distance = 0;

        while(ray_distance < RAY_MAX_DISTANCE){
            if(map.cells_infos[ray_value].isWall){
                break;
            }

            ray_pos.x += dx;
            ray_pos.y += dy;

            if((int)(ray_pos.x/map.cell_real_size) < map.width && (int)(ray_pos.y/map.cell_real_size) < map.height){
                ray_value = map.cells[(int)(ray_pos.x/map.cell_real_size)][(int)(ray_pos.y/map.cell_real_size)];
            }
            else{
                break;
            }
            float ray_distance = dist(player->pos, ray_pos);
        }

        if(map.cells_infos[ray_value].isWall){
            Ray_Point ray_point = {i_ray, ray_pos, ray_value};

            player->rays = list_ray_point_push(player->rays, ray_point);

            ray_angle += dAngle;
        }
    }
}