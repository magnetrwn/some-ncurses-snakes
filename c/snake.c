#include "snake.h"

snake_t *snake_init(size_t length, vec2_t head, direct4_t direction) {
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
        else if (draw_cursor.y < 1)
            draw_cursor.y = max_xy.y;
        else if (draw_cursor.y > max_xy.y)
            draw_cursor.y = 1;

        mvwprintw(win, draw_cursor.y, draw_cursor.x, "@");

        if (snake_get_rot_from(snake, draw_cursor) != NONE)
            curr_rot = snake_get_rot_from(snake, draw_cursor);

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
    else if (snake->head.y < 1)
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

void snake_rotate(snake_t *snake, direct4_t direction) {
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

direct4_t snake_get_rot_from(snake_t *snake, vec2_t position) {
    snake_rot_llist_t *rot_ptr = snake->rots;
    while (rot_ptr != NULL) {
        if (rot_ptr->position.x == position.x && rot_ptr->position.y == position.y)
            return rot_ptr->rot_from;
        rot_ptr = rot_ptr->next;
    }
    return NONE;
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

/* ----------------------------------------------------------------------- */

vec2_t init_ncurses(void) {
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    vec2_t screen_size;
    getmaxyx(stdscr, screen_size.y, screen_size.x);
    return screen_size;
}

/*size_t draw_all_rot_points(WINDOW *win, snake_t *snake) {
    size_t sz = 0;
    snake_rot_llist_t *rot_ptr = snake->rots;
    while (rot_ptr != NULL) {
        mvwprintw(win, rot_ptr->position.y, rot_ptr->position.x, "+");
        rot_ptr = rot_ptr->next;
        sz++;
    }
    return sz;
}*/

/* ----------------------------------------------------------------------- */

#define SNAKE_STARTING_LENGTH 200
#define FRAME_DELAY 133

int main(void) {
    vec2_t screen_size = init_ncurses();
    WINDOW *game_window = newwin(screen_size.y, screen_size.x, 0, 0);
    wtimeout(game_window, FRAME_DELAY);

    vec2_t screen_center = {screen_size.x/2, screen_size.y/2};
    vec2_t screen_max = {screen_size.x - 2, screen_size.y - 2};
    snake_t *snake = snake_init(SNAKE_STARTING_LENGTH, screen_center, RIGHT);

    int ch = 'd';
    do {
        ch = wgetch(game_window);
        switch (ch) {
            case 'w':
                snake_rotate(snake, UP);
                break;
            case 's':
                snake_rotate(snake, DOWN);
                break;
            case 'a':
                snake_rotate(snake, LEFT);
                break;
            case 'd':
                snake_rotate(snake, RIGHT);
                break;
        }

        wclear(game_window);
        wattron(game_window, COLOR_PAIR(1));
        snake_step(screen_max, snake);
        snake_draw(game_window, screen_max, snake);
        wattroff(game_window, COLOR_PAIR(1));
        wattron(game_window, COLOR_PAIR(2));
        box(game_window, 0, 0);
        mvwprintw(game_window, 1, 1, "Score: %lu", (snake->length - SNAKE_STARTING_LENGTH) * 1000 / FRAME_DELAY);
        wattroff(game_window, COLOR_PAIR(2));
        wrefresh(game_window);

    } while (ch != 'q' && ch != KEY_EXIT);

    delwin(game_window);
    endwin();
    clear_snake(snake);
    return 0;
}
