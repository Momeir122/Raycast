#include "list.h"

element_ray_point* list_ray_point_push(element_ray_point* element, Ray_Point ray_point){
    element_ray_point* newElement = malloc(sizeof(element_ray_point));
    newElement->value = ray_point;
    newElement->next = element;

    return newElement;
}

element_ray_point* list_ray_point_destroy(element_ray_point* element){
    while(element != NULL){
        element_ray_point* tmp = element->next;
        free(element);
        element = tmp;
    }

    return NULL;
}