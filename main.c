#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 700 

int main(void) {
  InitWindow(WIDTH, HEIGHT, "Game");
    const int rows = 10;
    const int columns = 10;
    const int cellWidth = WIDTH / columns;   // 80 pixels
    const int cellHeight = HEIGHT / rows;    // 70 pixels

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    for (int i = 0; i <= columns; i++) 
        {
            int xPosition = i * cellWidth;
            DrawLine(xPosition, 0, xPosition, HEIGHT, LIGHTGRAY);
        }
        for (int i = 0; i <= rows; i++) 
        {
            int yPosition = i * cellHeight;
            DrawLine(0, yPosition, WIDTH, yPosition, LIGHTGRAY);
        }

    EndDrawing(); 
  }

  CloseWindow();

  return 0;
}
