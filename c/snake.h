#ifndef SNAKE_H_
#define SNAKE_H_

#include <stdlib.h>
#include <curses.h>
#include <ncurses.h>

typedef enum _direct4 {
    NONE, RIGHT, UP, LEFT, DOWN
} direct4_t;

typedef struct _vec2 {
    size_t x, y;
} vec2_t;

typedef struct _snake_rot_llist {
    vec2_t position;
    direct4_t rot_from, rot_to;
    size_t lifetime;
    struct _snake_rot_llist *next;
} snake_rot_llist_t;

typedef struct _snake {
    vec2_t head;
    direct4_t direction;
    size_t length;
    snake_rot_llist_t *rots;
} snake_t;

// TODO: remember to check which functions to inline
snake_t *snake_init(size_t length, vec2_t head, direct4_t direction);
void snake_draw(WINDOW *win, snake_t *snake);
void snake_step(snake_t *snake);
void snake_rotate(snake_t *snake, direct4_t direction);
direct4_t snake_get_rot_from(snake_t *snake, vec2_t position);
void clear_snake(snake_t *snake);

#endif // SNAKE_H_
