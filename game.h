#ifndef GAME_H
#define GAME_H

#include <deque>
#include <raylib.h>
#include <raymath.h>

using namespace std;

extern Color green;
extern Color darkGreen;
extern int cellSize;
extern int cellCount;
extern int offset;
extern double lastUpdateTime;

// Utility Functions
bool elementInDeque(Vector2 element, deque<Vector2> deque);
bool eventTriggered(double interval, int score, double speedMultiplier);

// Snake Class
class Snake {
public:
    deque<Vector2> body;
    Vector2 direction;
    bool addSegment;
    Texture2D bodyTexture;
    Texture2D headTexture;

    Snake();
    ~Snake();
    void draw();
    void update();
    void reset();
};

// Food Class
class Food {
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody);
    ~Food();
    void draw();
    Vector2 generateRandomCell();
    Vector2 generateRandomPos(deque<Vector2> snakeBody);
};

// Game Class
class Game {
public:
    Snake snake;
    Food food;
    bool running;
    int score = 0;
    Sound eatSound;
    Sound hitWallSound;
    Sound hitBodySound;

    Game();
    ~Game();
    void draw();
    void update();
    void checkCollisionWithFood();
    void checkCollisionWithEdges();
    void checkCollisionWithTail();
    void gameOver();
};

#endif // GAME_H