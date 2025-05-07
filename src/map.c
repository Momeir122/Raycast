#include "map.h"

sfVector2f get_map_relative_pos(sfVector2f pos, sfVector2f display_size, Map map){
    sfVector2f newPos;
    newPos.x =  (display_size.x * pos.x)/(map.width * map.cell_real_size);
    newPos.y =  (display_size.y * pos.y)/(map.height * map.cell_real_size);

    return newPos;
}

void draw_player_inMap(sfRenderWindow* window, sfVector2f pos, sfVector2f size, Map map, Player* player){
    sfCircleShape* player_circle = sfCircleShape_create();
    sfCircleShape_setFillColor(player_circle, PLAYER_COLOR);

    sfVector2f player_map_pos = {pos.x + get_map_relative_pos(player->pos, size, map).x, pos.y + get_map_relative_pos(player->pos, size, map).y};
    sfCircleShape_setPosition(player_circle, player_map_pos);

    float player_circle_radius = (size.x * 10)/(map.width * map.cell_real_size);
    sfCircleShape_setRadius(player_circle, player_circle_radius);

    sfRenderWindow_drawCircleShape(window, player_circle, NULL);

    sfCircleShape_destroy(player_circle);
}

void draw_player_rays_inMap(sfRenderWindow* window, sfVector2f pos, sfVector2f size, Map map, Player* player){
    element_ray_point* player_rays = player->rays;

    while(player_rays != NULL){
        sfVector2f ray_point_pos = player_rays->value.pos;

        sfCircleShape* ray_point_circle = sfCircleShape_create();
        sfCircleShape_setPosition(ray_point_circle ,get_map_relative_pos(ray_point_pos, size, map));

        float ray_point_circle_radius = (size.x * 5)/(map.width * map.cell_real_size);
        sfCircleShape_setRadius(ray_point_circle, ray_point_circle_radius);

        sfRenderWindow_drawCircleShape(window, ray_point_circle, NULL);

        sfCircleShape_destroy(ray_point_circle);

        player_rays = player_rays->next;
    }
}

void draw_map(sfRenderWindow* window, sfVector2f pos, sfVector2f size, Map map, Player* player){
    /* Drawing the cells */
    float dx = size.x/map.width;
    float dy = size.y/map.height;

    int xi = 0; int yi = 0;
    for(float x = pos.x; x < pos.x + size.x; x += dx){
        for(float y = pos.y; y < pos.y + size.y; y += dy){
            sfRectangleShape* cell_rect = sfRectangleShape_create();

            sfRectangleShape_setPosition(cell_rect, (sfVector2f){x,y});
            sfRectangleShape_setSize(cell_rect, (sfVector2f){dx,dy});
            sfRectangleShape_setFillColor(cell_rect, map.cells_infos[map.cells[xi][yi]].color);

            sfRenderWindow_drawRectangleShape(window, cell_rect, NULL);

            sfRectangleShape_destroy(cell_rect);

            yi++;
        }
        xi++;
        yi = 0;
    }

    draw_player_inMap(window, pos, size, map, player);
    draw_player_rays_inMap(window, pos, size, map, player);
}

Map create_main_map() {
    Map newMap;
    newMap.width = 10;
    newMap.height = 10;
    newMap.cell_real_size = 100;

    // Carte 10x10 manuelle (1 = mur, 0 = vide)
    int manual_map[10][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 1, 0, 1},
        {1, 0, 1, 1, 0, 1, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 1, 1, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    // Copie dans la structure Map
    newMap.cells = malloc(newMap.width * sizeof(int*));
    for (int x = 0; x < newMap.width; x++) {
        newMap.cells[x] = malloc(newMap.height * sizeof(int));
        for (int y = 0; y < newMap.height; y++) {
            newMap.cells[x][y] = manual_map[x][y];
        }
    }

    // Configuration des types de cellules (seulement 0 et 1)
    newMap.cells_infos = malloc(sizeof(Map_Cell_Info) * 2);
    
    // 0 : Vide
    newMap.cells_infos[0].isWall = 0;
    newMap.cells_infos[0].color = sfBlack;
    
    // 1 : Mur rouge (unique texture)
    newMap.cells_infos[1].isWall = 1;
    newMap.cells_infos[1].color = sfRed;
    newMap.cells_infos[1].render_texture = sfTexture_createFromFile("images/walls/brick_red.png", NULL);

    return newMap;
}

void destroy_map(Map map){
    sfTexture_destroy(map.cells_infos[1].render_texture);
    sfTexture_destroy(map.cells_infos[2].render_texture);
    sfTexture_destroy(map.cells_infos[3].render_texture);
}