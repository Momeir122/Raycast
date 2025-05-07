#ifndef LIST_H
#define LIST_H

#include "types.h"
#include <stdlib.h>

element_ray_point* list_ray_point_push(element_ray_point* element, Ray_Point ray_point);
element_ray_point* list_ray_point_destroy(element_ray_point* element);

#endif