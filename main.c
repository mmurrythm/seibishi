// refrerence https://youtu.be/P7PMA3X1tf8?si=5vKgEPFMyzi640u6

#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10
#define TILE_SIZE 32
#define TILE_TYPES 4
const char tileTypes[TILE_TYPES] = {' ', '#', '.', '$'}; // Empty, Wall, Goal, Box

char board[BOARD_SIZE][BOARD_SIZE];
char goals[BOARD_SIZE][BOARD_SIZE];

char randomtile()
{
    int r = GetRandomValue(0, TILE_TYPES - 1);
    return tileTypes[r];
}

void init_Board(void)
{
    char level[BOARD_SIZE][BOARD_SIZE] =
        {
            "##########",
            "#        #",
            "#   $ .  #",
            "#        #",
            "#   .    #",
            "#  #     #",
            "#        #",
            "#        #",
            "#      # #",
            "##########"};
    // tile screen position = board offset + grid position × tile size
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            board[y][x] = level[y][x];
            goals[y][x] = level[y][x] ;
        }
    }
}

int main(void)
{
    const int screenwidth = 800, screenheight = 800;

    InitWindow(screenwidth, screenheight, "Sokoban Game");
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL)); // Seed the random number generator
    init_Board();

    int playerX = 2, playerY = 2;

    int movescount = 0;
    int boardWidth = BOARD_SIZE * TILE_SIZE, boardHeight = BOARD_SIZE * TILE_SIZE;

    int boardoffsetX = (screenwidth - boardWidth) / 2;
    int boardoffsetY = (screenheight - boardHeight) / 2;

    int goalCount = 0, totalcount = 0;

    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (goals[y][x] == '.')
            {
                totalcount++;
            }
        }
    }


    while (!WindowShouldClose()) // game loop
    {

        // Update game logic here

        int moveX = 0, moveY = 0;

        if (IsKeyPressed(KEY_RIGHT))
            moveX = 1;
        else if (IsKeyPressed(KEY_LEFT))
            moveX = -1;
        else if (IsKeyPressed(KEY_UP))
            moveY = -1;
        else if (IsKeyPressed(KEY_DOWN))
            moveY = 1;

        if (moveX != 0 || moveY != 0) // ensure pressing for movement
        {
            int nextX = playerX + moveX, nextY = playerY + moveY;
            if (nextX >= 0 && nextX < BOARD_SIZE && nextY >= 0 && nextY < BOARD_SIZE)
            {
                if (board[nextY][nextX] == '$')
                {
                    // detect box
                    int boxNextX = nextX + moveX, boxNextY = nextY + moveY;
                    if (boxNextX >= 0 && boxNextX < BOARD_SIZE && boxNextY >= 0 && boxNextY < BOARD_SIZE)
                    {
                        // check before boxes
                        if (board[boxNextY][boxNextX] != '#' && board[boxNextY][boxNextX] != '$')
                        {
                            // push box
                            board[boxNextY][boxNextX] = '$';
                            if(goals[nextY][nextX] == '.')
                            {
                                board[nextY][nextX] = '.';
                            }
                            else
                            {
                                board[nextY][nextX] = ' ';
                            }
                            playerX = nextX;
                            playerY = nextY;
                            movescount++;
                        }
                    }
                }
                else if (board[nextY][nextX] != '#')
                {
                    playerX = nextX;
                    playerY = nextY;
                    movescount++;
                }
            }
        }

        goalCount = 0;
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            for (int x = 0; x < BOARD_SIZE; x++)
            {
                if (goals[y][x] == '.' && board[y][x] == '$')
                {
                    goalCount++;
                }
            }
        }

        if(goalCount == totalcount)
        {
            DrawText("YOU WIN!", screenwidth / 2 - 100, screenheight / 2 - 20, 40, GREEN);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < BOARD_SIZE; y++) // nested drawing loops
        {
            for (int x = 0; x < BOARD_SIZE; x++)
            {
                Rectangle rect =
                    {
                        boardoffsetX + x * TILE_SIZE,
                        boardoffsetY + y * TILE_SIZE,
                        TILE_SIZE,
                        TILE_SIZE
                    };

                // tile content drawing
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
                    if(goals[y][x] == '.')
                    {
                        DrawRectangleRec(rect, GREEN);
                    }
                    else
                    {
                        DrawRectangleRec(rect, BROWN);
                    }
                }
                else
                {
                    DrawRectangleRec(rect, RAYWHITE);
                }

                DrawRectangleLinesEx(rect, 1, BLACK);

                DrawTextEx(GetFontDefault(), TextFormat("%c", board[y][x]), (Vector2){rect.x + 12, rect.y + 6}, 24, 1, BLACK);
                // DrawText(&board[y][x], x * TILE_SIZE, y * TILE_SIZE, 20, BLACK);
            }
        }
        DrawCircle(boardoffsetX + playerX * TILE_SIZE + TILE_SIZE / 2, boardoffsetY + playerY * TILE_SIZE + TILE_SIZE / 2, 10, BLUE);

        int margin = 10;
        int fontSize = 20;
        DrawText(TextFormat("MOVES: %d", movescount), margin, screenheight - fontSize - margin, fontSize, BLACK);
        DrawText(TextFormat("GOALS: %d", goalCount), margin, screenheight - fontSize - margin - 25, fontSize, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
