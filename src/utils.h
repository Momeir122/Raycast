#ifndef UTILS_H
#define UTILS_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include <math.h>
#pragma GCC diagnostic pop

#define PI 3.141592653589793

float deg_to_rad(float angle);
float dist(sfVector2f p1, sfVector2f p2);

#endif