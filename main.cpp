#include <iostream>
#include "game.h"

int main() {
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake Game");
    SetTargetFPS(60);

    Game game;
    double normalSpeed = 1.0;     
    double acceleratedSpeed = 0.5;

    while (!WindowShouldClose()) {
        BeginDrawing();

        double speedMultiplier = IsKeyDown(KEY_SPACE) ? acceleratedSpeed : normalSpeed;

        if (eventTriggered(0.2, game.score / 50, speedMultiplier)) {
            game.update();
        }

        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0, -1};
            game.running = true;
        }

        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0, 1};
            game.running = true;
        }

        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1, 0};
            game.running = true;
        }

        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1, 0};
            game.running = true;
        }

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset-5,
        (float)offset-5, (float)cellSize*cellCount+10,
        (float)cellSize*cellCount+10}, 5, darkGreen);

        DrawText("Snake Game", offset + 250, 22, 50, BLACK);

        DrawText(TextFormat("Score: %i", game.score),offset -5, offset + cellSize * cellCount+10, 40, BLACK);

        DrawText("Hold Space To Accelerate", offset + 450, offset + cellSize * cellCount+10, 20, BLACK );

        DrawText("Use Arrow Keys To Move", offset + 450, offset + cellSize * cellCount+40, 20, BLACK );

        game.draw();
        
        EndDrawing();
   }

   CloseWindow();
}
