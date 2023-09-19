#include "snake.h"

snake_t *snake_init(const size_t length, const vec2_t head, const direct4_t direction) {
    snake_t *snake = malloc(sizeof(snake_t));
    snake->length = length;
    snake->head = head;
    snake->direction = direction;
    snake->rots = NULL;
    return snake;
}

void snake_draw(WINDOW *win, const vec2_t max_xy, snake_t *snake) {
    vec2_t draw_cursor = snake->head;
    direct4_t curr_rot = snake->direction;

    for (size_t i = 0; i < snake->length; i++) {
        // These ifs below are only for when the wrap gets the snake tail out of bounds
        if (draw_cursor.x < 1)
            draw_cursor.x = max_xy.x;
        else if (draw_cursor.x > max_xy.x)
            draw_cursor.x = 1;

        if (draw_cursor.y < 1)
            draw_cursor.y = max_xy.y;
        else if (draw_cursor.y > max_xy.y)
            draw_cursor.y = 1;

        // TODO: use mvwaddch() instead of mvwprintw()
        mvwprintw(win, draw_cursor.y, draw_cursor.x, "@");

        snake_rot_llist_t *rotation = snake_get_first_rot(snake, draw_cursor);

        if (rotation != NULL)
            if (rotation->rot_from != NONE)
                curr_rot = rotation->rot_from;

        switch (curr_rot) {
            case UP: draw_cursor.y++; break;
            case DOWN: draw_cursor.y--; break;
            case LEFT: draw_cursor.x++; break;
            case RIGHT: draw_cursor.x--; break;
            default: break;
        }
    }
}

void snake_step(const vec2_t max_xy, snake_t *snake) {
    switch (snake->direction) {
        case UP: snake->head.y--; break;
        case DOWN: snake->head.y++; break;
        case LEFT: snake->head.x--; break;
        case RIGHT: snake->head.x++; break;
        default: break;
    }

    if (snake->head.x < 1)
        snake->head.x = max_xy.x;
    else if (snake->head.x > max_xy.x)
        snake->head.x = 1;

    if (snake->head.y < 1)
        snake->head.y = max_xy.y;
    else if (snake->head.y > max_xy.y)
        snake->head.y = 1;

    snake_rot_llist_t *rot_ptr = snake->rots;
    while (rot_ptr != NULL) {
        rot_ptr->lifetime--;
        if (rot_ptr->lifetime == 0) {
            snake_rot_llist_t *tmp = rot_ptr;
            rot_ptr = rot_ptr->next;
            snake->rots = rot_ptr;
            free(tmp);
        } else
            rot_ptr = rot_ptr->next;
    }
}

void snake_rotate(snake_t *snake, const direct4_t direction) {
    snake_rot_llist_t *rot_ptr = snake->rots;
    snake_rot_llist_t *new_rot = malloc(sizeof(snake_rot_llist_t));
    new_rot->position = snake->head;
    new_rot->rot_to = direction;
    new_rot->rot_from = snake->direction;
    new_rot->lifetime = snake->length;
    new_rot->next = NULL;

    snake->direction = direction;

    if (rot_ptr == NULL)
        snake->rots = new_rot;
    else {
        while (rot_ptr->next != NULL)
            rot_ptr = rot_ptr->next;
        rot_ptr->next = new_rot;
    }
}

void snake_lengthen(snake_t *snake, const size_t amount) {
    snake->length += amount;
    snake_rot_llist_t *rot_ptr = snake->rots;
    while (rot_ptr != NULL) {
        rot_ptr->lifetime += amount;
        rot_ptr = rot_ptr->next;
    }
}

snake_rot_llist_t *snake_get_first_rot(snake_t *snake, const vec2_t position) {
    snake_rot_llist_t *rot_ptr = snake->rots;
    while (rot_ptr != NULL) {
        if (rot_ptr->position.x == position.x && rot_ptr->position.y == position.y)
            return rot_ptr;
        rot_ptr = rot_ptr->next;
    }
    return NULL;
}

short snake_is_stuck(const vec2_t max_xy, snake_t *snake) {
    vec2_t checking = snake->head;
    direct4_t curr_rot = snake->direction;

    for (size_t i = 0; i < snake->length; i++) {
        if (checking.x < 1)
            checking.x = max_xy.x;
        else if (checking.x > max_xy.x)
            checking.x = 1;

        if (checking.y < 1)
            checking.y = max_xy.y;
        else if (checking.y > max_xy.y)
            checking.y = 1;

        snake_rot_llist_t *rotation = snake_get_first_rot(snake, checking);

        if (rotation != NULL)
            if (rotation->rot_from != NONE) {
                if (rotation->lifetime != snake->length - i)
                    return TRUE;
                curr_rot = rotation->rot_from;
            }

        switch (curr_rot) {
            case UP: checking.y++; break;
            case DOWN: checking.y--; break;
            case LEFT: checking.x++; break;
            case RIGHT: checking.x--; break;
            default: break;
        }

        if (snake->head.y == checking.y && snake->head.x == checking.x)
            return TRUE;
    }

    return FALSE;
}

void clear_snake(snake_t *snake) {
    snake_rot_llist_t *rot_ptr = snake->rots;
    while (rot_ptr != NULL) {
        snake_rot_llist_t *tmp = rot_ptr;
        rot_ptr = rot_ptr->next;
        free(tmp);
    }
    free(snake);
}
