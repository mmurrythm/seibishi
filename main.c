#include "raylib.h"

int main(void)
{
    InitWindow(800, 600, "Sokoban");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // 1. UPDATE
        // Read input and change the game state

        // 2. DRAW
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the game here

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
