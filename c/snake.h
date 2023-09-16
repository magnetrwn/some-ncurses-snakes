#ifndef SNAKE_H_
#define SNAKE_H_

#include <stdlib.h>
#include <ncurses.h>

typedef enum _direct4 {
    NONE, RIGHT, UP, LEFT, DOWN
} direct4_t;

typedef struct _vec2 {
    long x, y;
} vec2_t;

typedef struct _snake_rot_llist {
    vec2_t position;
    direct4_t rot_from;
    direct4_t rot_to;
    size_t lifetime;
    struct _snake_rot_llist *next;
} snake_rot_llist_t;

typedef struct _snake {
    vec2_t head;
    direct4_t direction;
    size_t length;
    snake_rot_llist_t *rots;
} snake_t;

snake_t *snake_init(const size_t length, const vec2_t head, const direct4_t direction);
void snake_draw(WINDOW *win, const vec2_t max_xy, snake_t *snake);
void snake_step(const vec2_t max_xy, snake_t *snake);
void snake_rotate(snake_t *snake, const direct4_t direction);
void snake_lengthen(snake_t *snake, const size_t amount);
direct4_t snake_get_rot_from(snake_t *snake, const vec2_t position);
short snake_is_stuck(const vec2_t max_xy, snake_t *snake);
void clear_snake(snake_t *snake);

#endif // SNAKE_H_
