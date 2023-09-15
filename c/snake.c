#include "snake.h"
#include <curses.h>

vec2_t init_ncurses(void) {
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    vec2_t screen_size;
    getmaxyx(stdscr, screen_size.y, screen_size.x);
    return screen_size;
}

snake_t *snake_init(size_t length, vec2_t head, direct4_t direction) {
    snake_t *snake = malloc(sizeof(snake_t));
    snake->length = length;
    snake->head = head;
    snake->direction = direction;
    snake->rots = NULL;
    return snake;
}

void snake_draw(WINDOW *win, snake_t *snake) {
    vec2_t draw_cursor = snake->head;
    direct4_t curr_direct = snake->direction;
    for (size_t i = 0; i < snake->length; i++) {
        mvwprintw(win, draw_cursor.y, draw_cursor.x, "@");
        curr_direct = snake_is_rot(snake, draw_cursor);
        switch (curr_direct) {
            case UP:
                draw_cursor.y++;
                break;
            case DOWN:
                draw_cursor.y--;
                break;
            case LEFT:
                draw_cursor.x++;
                break;
            case RIGHT:
                draw_cursor.x--;
                break;
        }
    }
}

void snake_step(snake_t *snake) {
    switch (snake->direction) {
        case UP:
            snake->head.y--;
            break;
        case DOWN:
            snake->head.y++;
            break;
        case LEFT:
            snake->head.x--;
            break;
        case RIGHT:
            snake->head.x++;
            break;
    }

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
    snake->direction = direction;
    snake_rot_llist_t *rot_ptr = snake->rots;
    while (rot_ptr != NULL)
        rot_ptr = rot_ptr->next;

    rot_ptr = malloc(sizeof(snake_rot_llist_t));
    rot_ptr->position = snake->head;
    rot_ptr->direction = direction;
    rot_ptr->lifetime = snake->length;
    rot_ptr->next = NULL;
}

direct4_t snake_is_rot(snake_t *snake, vec2_t position) {
    snake_rot_llist_t *rot_ptr = snake->rots;
    while (rot_ptr != NULL) {
        if (rot_ptr->position.x == position.x && rot_ptr->position.y == position.y)
            return rot_ptr->direction;
        rot_ptr = rot_ptr->next;
    }
    return snake->direction;
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

int main(void) {
    vec2_t screen_size = init_ncurses();
    WINDOW *game_window = newwin(screen_size.y, screen_size.x, 0, 0);
    wtimeout(game_window, 200);

    vec2_t screen_center = {screen_size.x/2, screen_size.y/2};
    snake_t *snake = snake_init(5, screen_center, RIGHT);

    int ch = 'd';
    do {
        wclear(game_window);
        wattron(game_window, COLOR_PAIR(1));
        snake_step(snake);
        snake_draw(game_window, snake);
        wattroff(game_window, COLOR_PAIR(1));
        box(game_window, 0, 0);
        wrefresh(game_window);

        ch = wgetch(game_window);
        switch (ch) {
            case KEY_UP:
            case 'w':
                snake_rotate(snake, UP);
                break;
            case KEY_DOWN:
            case 's':
                snake_rotate(snake, DOWN);
                break;
            case KEY_LEFT:
            case 'a':
                snake_rotate(snake, LEFT);
                break;
            case KEY_RIGHT:
            case 'd':
                snake_rotate(snake, RIGHT);
                break;
        }
    } while (ch != 'q' && ch != KEY_EXIT);

    delwin(game_window);
    endwin();
    clear_snake(snake);
    return 0;
}
