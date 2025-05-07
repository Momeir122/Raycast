#include "utils.h"

float deg_to_rad(float angle){
    return (2*PI * angle)/360;
}

float dist(sfVector2f p1, sfVector2f p2){
    return sqrtf((p2.x - p1.x)*(p2.x-p1.x) + (p2.y - p1.y)*(p2.y-p1.y));
}