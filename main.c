// refrerence https://youtu.be/P7PMA3X1tf8?si=5vKgEPFMyzi640u6

#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 16
#define TILE_SIZE 32
#define TILE_TYPES 4
const char tileTypes[TILE_TYPES] = {' ', '#', '.', '$'}; // Empty, Wall, Goal, Box

char board[BOARD_SIZE][BOARD_SIZE];
char goals[BOARD_SIZE][BOARD_SIZE];

void init_Board(int levelNum)
{
    //from line 24 to 214, we defined layout
    char level1[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "#   $      .   #",
        "#      ##      #",
        "#              #",
        "#   ###        #",
        "#       .      #",
        "#        $     #",
        "#              #",
        "#      ###     #",
        "#              #",
        "#   #          #",
        "#              #",
        "#              #",
        "#              #",
        "################"
    };

    char level2[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "#  $           #",
        "#  ###   ###   #",
        "#      .       #",
        "#      ###     #",
        "#              #",
        "#   ##         #",
        "#   ##    .    #",
        "#        $     #",
        "#              #",
        "#      ###     #",
        "#              #",
        "#              #",
        "#              #",
        "################"
    };
    char level3[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#       #      #",
        "#  $    #  .   #",
        "#       #      #",
        "###   #######  #",
        "#              #",
        "#   .          #",
        "#       ###    #",
        "#       ###    #",
        "#          $   #",
        "#              #",
        "#  #######     #",
        "#              #",
        "#       #      #",
        "#              #",
        "################"
    };
    char level4[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "#   ####       #",
        "#   #  .#      #",
        "#   #   #      #",
        "#   #   ####   #",
        "#   #      .   #",
        "#   ######     #",
        "#              #",
        "#      $       #",
        "#              #",
        "#       $      #",
        "#              #",
        "#              #",
        "#              #",
        "################"
    };
    char level5[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#     #        #",
        "#  $  #   .    #",
        "#     #        #",
        "# ### #####    #",
        "#   #         #",
        "#   #   .    #",
        "#   #       #",
        "#   ##### ###  #",
        "#       $      #",
        "#              #",
        "#  ########    #",
        "#              #",
        "#              #",
        "#              #",
        "################"
    };
    char level6[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "#   #######    #",
        "#   #     #    #",
        "# $ #  .  #    #",
        "#   #     #### #",
        "#   ###        #",
        "#       .      #",
        "#       ####   #",
        "#           $  #",
        "#   ######     #",
        "#              #",
        "#              #",
        "#              #",
        "#              #",
        "################"
    };
    char level7[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "#  $      # .  #",
        "#          #   #",
        "#  ####    #   #",
        "#     #    ### #",
        "#  .  #        #",
        "#     ####     #",
        "#              #",
        "#     $        #",
        "#   ########   #",
        "#              #",
        "#              #",
        "#              #",
        "#              #",
        "################"
    };
    char level8[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#      #       #",
        "#  $   #   .   #",
        "#      #       #",
        "#  ##########  #",
        "#              #",
        "#    .         #",
        "#      ####    #",
        "#          $   #",
        "#      ####    #",
        "#              #",
        "#  ##########  #",
        "#              #",
        "#      #       #",
        "#              #",
        "################"
    };
    char level9[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "#   ###    .   #",
        "#   #          #",
        "# $ #   ####   #",
        "#   #          #",
        "#   #######    #",
        "#       .      #",
        "#              #",
        "#       ####   #",
        "#          $   #",
        "#              #",
        "#   #######    #",
        "#              #",
        "#              #",
        "################"
    };
    char level10[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "#  $    #      #",
        "#       #  .   #",
        "#   ### #      #",
        "#       ####   #",
        "#              #",
        "#   .      $   #",
        "#              #",
        "#    #####     #",
        "#              #",
        "#  #######     #",
        "#              #",
        "#       #      #",
        "#              #",
        "################"
    };
    // tile screen position = board offset + grid position × tile size

    char (*level)[BOARD_SIZE] =
    (levelNum == 1) ? level1 :
    (levelNum == 2) ? level2 :
    (levelNum == 3) ? level3 :
    (levelNum == 4) ? level4 :
    (levelNum == 5) ? level5 :
    (levelNum == 6) ? level6 :
    (levelNum == 7) ? level7 :
    (levelNum == 8) ? level8 :
    (levelNum == 9) ? level9 :level10;

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
    int levelNum = 1;

    init_Board(levelNum);

    //game variables
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

    // while game loop
    int gamewon = 0;
    while (!WindowShouldClose())
    {

        // Update game logic here
        if (IsKeyPressed(KEY_R))
        {
            init_Board(levelNum);
            playerX = 2;
            playerY = 2;
            movescount = 0;
            gamewon = 0;
        }

        int moveX = 0, moveY = 0;

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
            moveX = 1;
        else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
            moveX = -1;
        else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            moveY = -1;
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            moveY = 1;

        if ((moveX != 0 || moveY != 0) && gamewon == 0 && !IsKeyPressed(KEY_R)) // ensure pressing for movement
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
            gamewon = 1;
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

                DrawTextEx(GetFontDefault(), TextFormat("%c", board[y][x]), (Vector2)
                {
                    rect.x + 12, rect.y + 6
                }, 24, 1, BLACK);
                // DrawText(&board[y][x], x * TILE_SIZE, y * TILE_SIZE, 20, BLACK);
            }
        }
        DrawCircle(boardoffsetX + playerX * TILE_SIZE + TILE_SIZE / 2, boardoffsetY + playerY * TILE_SIZE + TILE_SIZE / 2, 10, BLUE);

        int margin = 10;
        int fontSize = 20;
        DrawText("MOVE: WASD / ARROWS", margin, margin, 20, BLACK);
        DrawText("RESET: R", margin, margin + 25, 20, BLACK);

        DrawText(TextFormat("MOVES: %d", movescount), margin, screenheight - fontSize - margin, fontSize, BLACK);
        DrawText(TextFormat("GOALS: %d / %d", goalCount, totalcount), margin, screenheight - fontSize - margin - 25, fontSize, BLACK);

        if(gamewon == 1)
        {
            DrawRectangle(250, 05, 295, 45, RED);
            DrawText("YOU WIN!", 300, 10, 40, GREEN); // center of the screen (400,400)
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
