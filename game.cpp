#include "game.h"

// Global Variables
Color green = {95, 179, 95, 200};
Color darkGreen = {42, 50, 25, 255};
int cellSize = 30;
int cellCount = 25;
int offset = 75;
double lastUpdateTime = 0;

// Utility Functions
bool elementInDeque(Vector2 element, deque<Vector2> deque) {

    for (unsigned int i = 0; i < deque.size(); i++) {

        if (Vector2Equals(deque[i], element)) {
            return true;

        }
    }
    return false;
}

bool eventTriggered(double interval, int score, double speedMultiplier) {
    double currentTime = GetTime();

    if (currentTime - lastUpdateTime >= interval * speedMultiplier - score / 100.0) {
        lastUpdateTime = currentTime;
        return true;

    }
    return false;
}

// Snake Class 
Snake::Snake() {
    body = {Vector2{6, 9}, Vector2{7, 9}, Vector2{8, 9}};
    direction = {1, 0};
    addSegment = false;

    Image headImage = LoadImage("Graphics/snakeHead.png");
    headTexture = LoadTextureFromImage(headImage);
    UnloadImage(headImage);

    Image bodyImage = LoadImage("Graphics/snakeBody.png");
    bodyTexture = LoadTextureFromImage(bodyImage);
    UnloadImage(bodyImage);
}

Snake::~Snake() {
    UnloadTexture(headTexture);
    UnloadTexture(bodyTexture);
}

void Snake::draw() {
    for(unsigned int i = 0; i < body.size(); i++){
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment = Rectangle{offset + x*cellSize, offset + y*cellSize, (float)cellSize, (float)cellSize};

        if (i == 0) {
            DrawTexturePro(
            headTexture,
            Rectangle{0, 0, (float)headTexture.width, (float)headTexture.height},
            segment,
            Vector2{0, 0}, 
            0.0f, 
            WHITE);

        } else {
            DrawTexturePro(
            bodyTexture,
            Rectangle{0, 0, (float)bodyTexture.width, (float)bodyTexture.height},
            segment,
            Vector2{0, 0},
            0.0f,
            WHITE);

        }
    }
}

void Snake::update() {
    body.push_front(Vector2Add(body[0], direction));

    if (addSegment) {
        addSegment = false;

    } else {
        body.pop_back();

    }
}

void Snake::reset() {
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    direction = {1, 0};
}

// Food Class 
Food::Food(deque<Vector2> snakeBody) {
    Image image = LoadImage("Graphics/banana.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position = generateRandomPos(snakeBody);
}

Food::~Food() {
    UnloadTexture(texture);
}

void Food::draw() {
    DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
}

Vector2 Food::generateRandomCell() {
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    return Vector2{x, y};
}

Vector2 Food::generateRandomPos(deque<Vector2> snakeBody) {
  Vector2 position = generateRandomCell();

    while(elementInDeque(position, snakeBody)){
        position = generateRandomCell();

    }
    return position;
}

// Game Class Definitions
Game::Game() : snake(), food(snake.body) {
    InitAudioDevice();
    hitWallSound = LoadSound("Sounds/impact.mp3");
    eatSound = LoadSound("Sounds/bite.wav");
    hitBodySound = LoadSound("Sounds/hitBody.mp3");
}

Game::~Game() {
    UnloadSound(eatSound);
    UnloadSound(hitWallSound);
    UnloadSound(hitBodySound);
    CloseAudioDevice();
}

void Game::draw() {
    food.draw();
    snake.draw();
}

void Game::update() {

    if(running){
        snake.update();
        checkCollisionWithFood();
        checkCollisionWithEdges();
        checkCollisionWithTail();
    }
}

void Game::checkCollisionWithFood() {

   if(Vector2Equals(snake.body[0], food.position)){
        food.position = food.generateRandomPos(snake.body);
        snake.addSegment = true;
        score++;
        PlaySound(eatSound);
    }
}

void Game::checkCollisionWithEdges() {

    if(snake.body[0].x == cellCount || snake.body[0].x == -1){
        PlaySound(hitWallSound);
        gameOver();
    }

    if(snake.body[0].y == cellCount || snake.body[0].y == -1){
        PlaySound(hitWallSound);
        gameOver();
    }
}

void Game::checkCollisionWithTail() {
    deque<Vector2> headlessBody = snake.body;
    headlessBody.pop_front();

    if(elementInDeque(snake.body[0],headlessBody)){
        gameOver();
    }
}

void Game::gameOver() {
    snake.reset();
    food.position = food.generateRandomPos(snake.body);
    running = false;
    score = 0;
}