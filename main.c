#include"raylib.h"
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10
#define TILE_SIZE 32
#define TILE_TYPES 4
const char tile_chars[TILE_TYPES] = {' ', '#', '.', '$'}; // Empty, Wall, Goal, Box

char board[BOARD_SIZE][BOARD_SIZE] ;

char random_tile()
{
    int r = GetRandomValue(0, TILE_TYPES - 1);
    return tile_chars[r];
}

void init_board(void)
{
    char level[BOARD_SIZE][BOARD_SIZE] =
    {
        "##########",
        "#        #",
        "#   $ .  #",
        "#        #",
        "#        #",
        "#        #",
        "#        #",
        "#        #",
        "#        #",
        "##########"
    };

    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            board[y][x] = level[y][x];
        }
    }
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Sokoban Game");
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL)); // Seed the random number generator
    init_board();

    int playerX = 2;
    int playerY = 2;

    while (!WindowShouldClose()) // game loop
    {

        // Update game logic here
        if (IsKeyPressed(KEY_RIGHT) && board[playerY][playerX + 1] != '#')
        {
            playerX++;
        }
        if (IsKeyPressed(KEY_LEFT) && board[playerY][playerX - 1] != '#')
        {
            playerX--;
        }
        if (IsKeyPressed(KEY_UP) && board[playerY - 1][playerX] != '#')
        {
            playerY--;
        }
        if (IsKeyPressed(KEY_DOWN) && board[playerY + 1][playerX] != '#')
        {
            playerY++;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < BOARD_SIZE; y++) //nested drawing loops
        {
            for (int x = 0; x < BOARD_SIZE; x++)
            {
                Rectangle rect =
                {
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                };

                //tile content drawing
                if (board[y][x] == '#')
                {
                    DrawRectangleRec(rect, GRAY);
                }
                else if (board[y][x] == '.')
                {
                    DrawRectangleRec(rect, YELLOW);
                }
                else if (board[y][x] == '$')
                {
                    DrawRectangleRec(rect, BROWN);
                }
                else
                {
                    DrawRectangleRec(rect, RAYWHITE);
                }

                DrawRectangleLinesEx(rect, 1, BLACK);

                DrawTextEx(
                    GetFontDefault(),
                    TextFormat("%c", board[y][x]),
                    (Vector2)
                {
                    rect.x + 12, rect.y + 6
                },
                24,
                1,
                BLACK
                );
                // DrawText(&board[y][x], x * TILE_SIZE, y * TILE_SIZE, 20, BLACK);
            }
        }
        DrawCircle
        (
            playerX * TILE_SIZE + TILE_SIZE / 2,
            playerY * TILE_SIZE + TILE_SIZE / 2,
            10,
            BLUE
        );

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
