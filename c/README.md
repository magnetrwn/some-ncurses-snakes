# C Version Documentation

## Bugs

+ On already existing rotations, increasing snake length by eating food makes the tail turn prematurely, causing multiple nodes to warp at the same time. Mostly noticeable when near to borders or when quickly eating pellets.
+ When holding W, A, S or D, there is no frame delay, as it is determined by `wtimeout()` for the main loop call to `wgetch()`. There is a TODO about that as well.

## Implementation

The C version of Snake uses the following data structures:

```c
typedef enum _direct4 direct4_t;
typedef struct _vec2 vec2_t;
typedef struct _snake_rot_llist snake_rot_llist_t;
typedef struct _snake snake_t;
// Check out the implementation in snake.h
```

There are two basic types used throughout the source: `direct4_t` and `vec2_t`. The former is an enum of 4 directions and `NONE`, while the latter is just a pair of coordinates.

The snake itself is of type `snake_t` and stores the following data:

+ Coordinates of the head
+ Direction of the head
+ Length
+ A linked list storing all rotation points

This implementation uses a list of rotations instead of saving states for each node of the snake, basically storing it like a series of lines delimited by the turning positions.

Elements of the linked list of rotation coordinates have the following data:

+ Coordinates of the change of direction
+ Old direction
+ New direction
+ The next element of the list

The main function for the snake movement is:

```c
void snake_step(const vec2_t max_xy, snake_t *snake);
```

It's important to note that `snake_step` only manages the head of the snake! The tail of the snake is drawn by `snake_draw` but is calculated from the position of the head and the linked list of turns, since the head is the only part to touch food.

Most of the other functions are pretty straight-forward, so here's a short description:

```c
// Initialize the snake
snake_t *snake_init(size_t length, vec2_t head, direct4_t direction);

// Draw the snake
void snake_draw(WINDOW *win, const vec2_t max_xy, snake_t *snake);

// Step forward
void snake_step(const vec2_t max_xy, snake_t *snake);

// Add a rotation point to the linked list of the snake,
// on the head's coordinates
void snake_rotate(snake_t *snake, direct4_t direction);

// Check if the linked list of rotations has a match for the
// position coordinates, and find the starting direction;
// this is used to draw the snake starting from the head backwards
direct4_t snake_get_rot_from(snake_t *snake, vec2_t position);

// Checks if the head has collided with the snake with the same
// algorithm as the snake_draw() function
short snake_is_stuck(const vec2_t max_xy, snake_t *snake);

// free() everything
void clear_snake(snake_t *snake);
```

## Demo

<a href="https://asciinema.org/a/608243" target="_blank"><img src="https://asciinema.org/a/608243.svg" /></a>