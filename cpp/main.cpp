#include <ncurses.h>
#include <deque>

enum class Direction {
    NONE, RIGHT, UP, LEFT, DOWN
};

struct Vec2 {
    long x, y;
};

struct SnakeRotation {
    Vec2 position;
    Direction rot_from;
    Direction rot_to;
    size_t lifetime;
};

class Snake {
protected:
    Vec2 head;
    Direction direction;
    size_t length;
    std::deque<SnakeRotation> rotations;

public:
    Snake(const size_t length, const Vec2 head, const Direction direction);

    void step(const Vec2 max_xy);
    void rotate(const Direction direction);
    void lengthen(const size_t amount);
};

int main() {

    return 0;
}