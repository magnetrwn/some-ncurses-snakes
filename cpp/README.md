# C++ Version Documentation

## Implementation

The C++ version of Snake uses the following data structures:

```cpp
enum class Direction;
struct Vec2;
struct SnakeRotation;
class Snake;
class SnakeGame;
// Check out the implementation in snake.hpp
```

Same as the C version, but with better encapsulation and OOP principles. `Snake` handles an instance of a snake, while `SnakeGame` also manages the ncurses window internally.

Instead of using a linked list of rotations, the C++ makes use of the STL `std::deque` for storing rotation history:
```cpp
struct SnakeRotation {
    Vec2 position;
    Direction rot_from;
    Direction rot_to;
    size_t lifetime;
};

class Snake {
protected:
    // Deque of rotations
    std::deque<SnakeRotation> rotations;
    // ...
public:
    // Append another rotation
    void rotate(const Direction direction);
    // ...
}
```

The C++ version also makes use of `std::unique_ptr` for storing Snake and an array of pellet coordinates and allows RAII to work. On the other hand, the `WINDOW *` type is still used for storing the ncurses window.

```cpp
WINDOW *window;
std::unique_ptr<Snake> snake;
std::unique_ptr<Vec2[]> pellets;
```

Some popular C++ idioms provided by `<algorithm>` are also used here, for example:

```cpp
if (!rotations.empty())
    rotations.erase(
        std::remove_if(rotations.begin(), rotations.end(), [] (SnakeRotation& rotation) {
            if (rotation.lifetime == 1)
                return true;
            else {
                rotation.lifetime--;
                return false;
            }
        }),
        rotations.end()
    );
```

This is based on the erase-remove idiom of C++, and is used in the source to decrement all lifetimes by 1 and delete completed rotations.

I haven't really created a good scoring system for either the C/C++ version, but it's not really that necessary for this project: the C++ version just counts the length increments.