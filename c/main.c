#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include "snake.h"

#define SNAKE_STARTING_LENGTH 5
#define FOOD_COUNT 20
#define FRAME_DELAY 133

vec2_t init_ncurses(void) {
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_WHITE);

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

int main(void) {
    vec2_t screen_size = init_ncurses();
    WINDOW *game_window = newwin(screen_size.y, screen_size.x, 0, 0);
    wtimeout(game_window, FRAME_DELAY);

    vec2_t screen_center = {screen_size.x/2, screen_size.y/2};
    vec2_t screen_max = {screen_size.x - 2, screen_size.y - 2};
    snake_t *snake = snake_init(SNAKE_STARTING_LENGTH, screen_center, RIGHT);

    size_t score = 0;

    srand(time(NULL));
    vec2_t food[FOOD_COUNT];
    for (size_t i = 0; i < FOOD_COUNT; i++) {
        food[i].x = rand() % screen_max.x + 1;
        food[i].y = rand() % screen_max.y + 1;
    }

    int ch = 'd';
    do {
        // TODO: add dynamic frame delay, maybe using napms() with a timer?
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
        score = (snake->length - SNAKE_STARTING_LENGTH) * 1000 / FRAME_DELAY;
        mvwprintw(game_window, 1, 1, "Score: %lu", score);
        wattron(game_window, COLOR_PAIR(1));
        snake_step(screen_max, snake);
        snake_draw(game_window, screen_max, snake);
        wattroff(game_window, COLOR_PAIR(1));
        wattron(game_window, COLOR_PAIR(2));
        box(game_window, 0, 0);

        for (size_t i = 0; i < FOOD_COUNT; i++) {
            if (snake->head.x == food[i].x && snake->head.y == food[i].y) {
                snake_lengthen(snake, 1);
                food[i].x = rand() % screen_max.x + 1;
                food[i].y = rand() % screen_max.y + 1;
            }
            mvwprintw(game_window, food[i].y, food[i].x, "%c", rand() % 10 + '0');
        }
        wattroff(game_window, COLOR_PAIR(2));
        wrefresh(game_window);

        if (snake_is_stuck(screen_max, snake)) {
            wattron(game_window, COLOR_PAIR(3));
            // TODO: make this a little nicer
            mvwprintw(game_window, screen_center.y, screen_center.x - 6, "  GAME OVER  ");
            mvwprintw(game_window, screen_center.y + 1, screen_center.x - 6, "  %lu points   ", score);
            wrefresh(game_window);
            napms(2000);
            wattroff(game_window, COLOR_PAIR(2));
            break;
        }

    } while (ch != 'q' && ch != KEY_EXIT);

    delwin(game_window);
    endwin();
    clear_snake(snake);
    return 0;
}
