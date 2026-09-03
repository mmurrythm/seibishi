// refrerence https://youtu.be/P7PMA3X1tf8?si=5vKgEPFMyzi640u6

#include "raylib.h"

#define BOARD_SIZE 16
#define TILE_TYPES 4
const char tileTypes[TILE_TYPES] = {' ', '#', '.', '$'}; // Empty, Wall, Goal, Box

char board[BOARD_SIZE][BOARD_SIZE];
char goals[BOARD_SIZE][BOARD_SIZE];

int playerStartsX[10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
int playerStartsY[10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

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
        "#   #         ##",
        "#   #   .    # #",
        "#   #       #  #",
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

int countGoals()
{
    int count = 0;
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (goals[y][x] == '.')
            {
                count++;
            }
        }
    }
    return count;
}

int main(void)
{
    const int screenwidth = 800, screenheight = 800;

    InitWindow(screenwidth, screenheight, "SEIBISHI");
    SetTargetFPS(60);

    //texture/pngfiles
    Texture2D floorTexture = LoadTexture("texture/floor.png");
    Texture2D wallTexture = LoadTexture("texture/wall.png");
    Texture2D goalTexture = LoadTexture("texture/goal.png");
    Texture2D boxTexture = LoadTexture("texture/box.png");
    Texture2D playerTexture = LoadTexture("texture/player.png");
    Texture2D boxOnGoalTexture = LoadTexture("texture/box_goal.png");
    Texture2D winTexture = LoadTexture("texture/win.png");
    Texture2D menubgTexture = LoadTexture("texture/menubg.png");
    int levelNum = 1;
    int menu = 1;

    init_Board(levelNum);

    int playerX = playerStartsX[levelNum - 1], playerY = playerStartsY[levelNum - 1];
    int movescount = 0;
    
    int boardWidth = BOARD_SIZE * 32, boardHeight = BOARD_SIZE * 32;

    int boardoffsetX = (screenwidth - boardWidth) / 2;
    int boardoffsetY = (screenheight - boardHeight) / 2;

    int goalCount = 0;
    int totalcount = countGoals();

    int gamewon = 0;
    while (!WindowShouldClose())
    {
        if(menu == 1)
        {
            BeginDrawing();
            ClearBackground(BLACK);

            DrawTexture(menubgTexture, 0, 0, WHITE);

            DrawText("THE SEIBISHI", 245, 200, 40, BLACK);
            DrawRectangle(245, 260, 280, 3, RAYWHITE);
            DrawText("10 LEVELS", 330, 240, 20, DARKGRAY);

            //DrawRectangle(280, 270, 240, 3, RAYWHITE);

            DrawText("Press ENTER to Start", 255, 350, 17, BLACK);
            DrawText("Press ESC TO QUIT", 255, 400, 17, BLACK);
            DrawText("Press I to Instructions", 255, 450, 17, BLACK); 
            EndDrawing();

            if(IsKeyPressed(KEY_ENTER))
            {
                menu = 0;
            }
            if(IsKeyPressed(KEY_ESCAPE))
            {
                break;
            }
            if(IsKeyPressed(KEY_I))
            {
                menu = 2;
            }
            continue;
        }

        if(menu == 2)
        {
            BeginDrawing();
            ClearBackground(BLACK);

            DrawTexture(menubgTexture, 0, 0, WHITE);

            DrawText("INSTRUCTIONS", 245, 200, 30, BLACK);
            DrawRectangle(245, 260, 280, 3, RAYWHITE);
            //DrawRectangle(280, 270, 240, 3, RAYWHITE);

            DrawText("Move: WASD / ARROWS", 255, 350, 17, BLACK);
            DrawText("Reset: R", 255, 400, 17, BLACK);
            DrawText("Press M TO GO BACK", 255, 450, 17, BLACK); 
            EndDrawing();

            if(IsKeyPressed(KEY_M))
            {
                menu = 1;
            }
            continue;
        }
        // if movement then if inside board and not wall then move player, if box then check if box can be moved, if yes then move box and player, if no then do nothing

        if (IsKeyPressed(KEY_R))
        {
            init_Board(levelNum);
            totalcount = countGoals();
            playerX = playerStartsX[levelNum - 1];
            playerY = playerStartsY[levelNum - 1];
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
                    int boxNextX = nextX + moveX, boxNextY = nextY + moveY;
                    if (boxNextX >= 0 && boxNextX < BOARD_SIZE && boxNextY >= 0 && boxNextY < BOARD_SIZE)
                    {
                        // check before boxes
                        if (board[boxNextY][boxNextX] != '#' && board[boxNextY][boxNextX] != '$')
                        {
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

        if(gamewon ==1 && IsKeyPressed(KEY_ENTER))
        {
            levelNum++;
            if(levelNum > 10)
            {
                levelNum = 1;
            }
            init_Board(levelNum);
            totalcount = countGoals();
            playerX = playerStartsX[levelNum - 1];
            playerY = playerStartsY[levelNum - 1];
            movescount = 0;
            gamewon = 0;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        //nested loops go through each tile of board
        // y = row, x = column
        //then create a Rectangle at that tile’s screen position so its contents can be drawn.

        for (int y = 0; y < BOARD_SIZE; y++) 
        {
            for (int x = 0; x < BOARD_SIZE; x++)
            {
                Rectangle rect =
                {
                    boardoffsetX + x * 32,
                    boardoffsetY + y * 32,
                    32,
                    32
                };

                // tile content drawing
                if (board[y][x] == '#')
                {
                    DrawTexture(wallTexture, rect.x, rect.y, WHITE);
                }
                else if (board[y][x] == '.')
                {
                    DrawTexture(goalTexture, rect.x, rect.y, WHITE);
                }
                else if (board[y][x] == '$')
                {
                    if(goals[y][x] == '.')
                    {
                        DrawTexture(boxOnGoalTexture, rect.x, rect.y, WHITE);
                    }
                    else
                    {
                        DrawTexture(boxTexture, rect.x, rect.y, WHITE);
                    }
                }
                else
                {
                    DrawTexture(floorTexture, rect.x, rect.y, WHITE);
                }

                DrawRectangleLinesEx(rect, 1, DARKGRAY);

                /*DrawTextEx(GetFontDefault(), TextFormat("%c", board[y][x]), (Vector2)
                {
                    rect.x + 12, rect.y + 6
                }, 24, 1, RAYWHITE);*/
            }
        }
        DrawTexture(playerTexture, boardoffsetX + playerX * 32, boardoffsetY + playerY * 32, WHITE);

        int margin = 10;
        int fontSize = 20;
        DrawText("MOVE: WASD / ARROWS", margin, margin, 20, GREEN);
        DrawText("RESET: R", margin, margin + 25, 20, RAYWHITE);

        DrawText(TextFormat("MOVES: %d", movescount), margin, screenheight - fontSize - margin, fontSize, RAYWHITE);
        DrawText(TextFormat("GOALS: %d / %d", goalCount, totalcount), margin, screenheight - fontSize - margin - 25, fontSize, RAYWHITE);
        DrawText(TextFormat("LEVEL: %d", levelNum), margin, screenheight - fontSize - margin - 50, fontSize, BLUE);
        if(gamewon == 1)
        {
            DrawTexture(winTexture, 179, 40, WHITE);
            //DrawText("MISSION COMPLETE!", 245, 30, 20, RAYWHITE); // center of screen (400,400)
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
