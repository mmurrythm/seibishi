#include "raylib.h"

#define BOARD_SIZE 16
#define TILE_TYPES 4

const char tileTypes[TILE_TYPES] = {' ', '#', '.', '$'}; // Empty, Wall, Goal, Box

char board[BOARD_SIZE][BOARD_SIZE];
char goals[BOARD_SIZE][BOARD_SIZE];

#define MAX_ENEMIES 5

typedef struct
{
    int x;
    int y;
    int hearts;
    int moves;
    int score;
} Player;

typedef struct
{
    int x;
    int y;
    int dx;
    int dy;
    int active;
} Enemy;
void initEnemies(int levelNum, Enemy enemies[], int *enemyCount);
int playerStartsX[10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
int playerStartsY[10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

void init_Board(int levelNum)
{
    //from line 24 to 214, we defined layout
    char level1[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "# $   #    .   #",
        "#     #        #",
        "#     ####     #",
        "#              #",
        "#   .      $   #",
        "#              #",
        "#     ####     #",
        "#              #",
        "#   $      .   #",
        "#              #",
        "#              #",
        "#              #",
        "#              #",
        "################"
    };

    char level2[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "# $    ###     #",
        "#      #       #",
        "#  #### #  .   #",
        "#       #      #",
        "#   .          #",
        "#       ####   #",
        "#              #",
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
        "#  $       .   #",
        "#  ###         #",
        "#      ####    #",
        "#              #",
        "#   .      $   #",
        "#       ###    #",
        "#              #",
        "#   ###        #",
        "#   $      .   #",
        "#              #",
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
        "#       .    # #",
        "#   #       #  #",
        "#   ##### ###  #",
        "#       $      #",
        "#              #",
        "#  ########    #",
        "#    $         #",
        "#              #",
        "#           .  #",
        "################"
    };

    char level6[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "# $       # .  #",
        "#         #    #",
        "#   #######    #",
        "#              #",
        "#  .       $   #",
        "#      ###     #",
        "#              #",
        "#   ####       #",
        "#   $      .   #",
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
        "# $  ###     . #",
        "#    #         #",
        "#    #   ####  #",
        "#              #",
        "#  .      $    #",
        "#       ###    #",
        "#              #",
        "#   ####       #",
        "#   $      .   #",
        "#              #",
        "#              #",
        "#              #",
        "#              #",
        "################"
    };

    char level8[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#              #",
        "# $      ### . #",
        "#        #     #",
        "#   ######     #",
        "#              #",
        "#  .      $    #",
        "#      ###     #",
        "#              #",
        "#    ####      #",
        "#   $      .   #",
        "#              #",
        "#              #",
        "#              #",
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
        "#   $      $   #",
        "#              #",
        "#   #######    #",
        "#    .         #",
        "#              #",
        "################"
    };

    char level10[BOARD_SIZE][BOARD_SIZE] =
    {
        "################",
        "#       #      #",
        "#    ## #   ## #",
        "#  $ ## #   ## #",
        "#       # $    #",
        "#       #      #",
        "#       #      #",
        "#       $   .  #",
        "#       #      #",
        "#       #      #",
        "#       #  .   #",
        "#  $##  #      #",
        "#  .    #   ## #",
        "#     . #   ## #",
        "#       #      #",
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

void resetLevel(int levelNum, Player *player, Enemy enemies[], int *enemyCount)
{
    init_Board(levelNum);

    player->x = playerStartsX[levelNum - 1];
    player->y = playerStartsY[levelNum - 1];
    player->moves = 0;

    initEnemies(levelNum, enemies, enemyCount);
}

void initEnemies(int levelNum, Enemy enemies[], int *enemyCount)
{
    *enemyCount = 0;

    if (levelNum == 1)
    {
        enemies[0] = (Enemy)
        {
            10, 2, 1, 1, 1
        };
        *enemyCount = 1;
    }
    else if (levelNum == 2)
    {
        enemies[0] = (Enemy)
        {
            10, 6, 0, 1, 1
        };
        *enemyCount = 1;
    }
    else if (levelNum == 3)
    {
        enemies[0] = (Enemy)
        {
            10, 5, 1, 0, 1
        };
        *enemyCount = 1;
    }
    else if (levelNum == 4)
    {
        enemies[0] = (Enemy)
        {
            10, 5, 0, 1, 1
        };
        *enemyCount = 1;
    }
    else if (levelNum == 5)
    {
        enemies[0] = (Enemy)
        {
            10, 6, 1, 0, 1
        };
        enemies[1] = (Enemy)
        {
            3, 10, 0, -1, 1
        };
        *enemyCount = 2;
    }
    else if (levelNum == 6)
    {
        enemies[0] = (Enemy)
        {
            10, 5, 1, 0, 1
        };
        *enemyCount = 1;
    }
    else if (levelNum == 7)
    {
        enemies[0] = (Enemy)
        {
            10, 5, 0, 1, 1
        };
        *enemyCount = 1;
    }
    else if (levelNum == 8)
    {
        enemies[0] = (Enemy)
        {
            10, 5, 1, 0, 1
        };
        *enemyCount = 1;
    }
    else if (levelNum == 9)
    {
        enemies[0] = (Enemy)
        {
            10, 6, 1, 0, 1
        };
        enemies[1] = (Enemy)
        {
            3, 9, 0, -1, 1
        };
        *enemyCount = 2;
    }
    else if (levelNum == 10)
    {
        enemies[0] = (Enemy)
        {
            10, 5, 0, 1, 1
        };
        enemies[1] = (Enemy)
        {
            3, 10, 1, 0, 1
        };
        enemies[2] = (Enemy)
        {
            2, 10, 5, 1, 0
        };
        *enemyCount = 3;
    }
}

void updateEnemies(Enemy enemies[], int enemyCount)
{
    for (int i = 0; i < enemyCount; i++)
    {
        if (!enemies[i].active)
            continue;

        int nextX = enemies[i].x + enemies[i].dx;
        int nextY = enemies[i].y + enemies[i].dy;

        if (nextX >= 0 && nextX < BOARD_SIZE &&
            nextY >= 0 && nextY < BOARD_SIZE &&
            board[nextY][nextX] != '#' &&
            board[nextY][nextX] != '$')
        {
            enemies[i].x = nextX;
            enemies[i].y = nextY;
        }
        else
        {
            // Reverse direction
            enemies[i].dx = -enemies[i].dx;
            enemies[i].dy = -enemies[i].dy;
        }
    }
}

int main(void)
{
    const int screenwidth = 800, screenheight = 800;

    InitWindow(screenwidth, screenheight, "SEIBISHI");
    SetTargetFPS(60);

    //texture/.pngfiles
    Texture2D floorTexture = LoadTexture("texture/floor.png");
    Texture2D wallTexture = LoadTexture("texture/wall.png");
    Texture2D goalTexture = LoadTexture("texture/goal.png");
    Texture2D boxTexture = LoadTexture("texture/box.png");
    Texture2D playerTexture = LoadTexture("texture/player.png");
    Texture2D boxOnGoalTexture = LoadTexture("texture/box_goal.png");
    Texture2D winTexture = LoadTexture("texture/win.png");
    Texture2D menubgTexture = LoadTexture("texture/menubg.png");
    Texture2D instructionbgTexture = LoadTexture("texture/instructions.png");
    Texture2D gameoverTexture = LoadTexture("texture/gameover.png");
    int levelNum = 1;
    int menu = 1;

    init_Board(levelNum);

    Player player;

    player.x = playerStartsX[levelNum - 1];
    player.y = playerStartsY[levelNum - 1];
    player.hearts = 3;
    player.moves = 0;
    player.score = 0;

    Enemy enemies[MAX_ENEMIES];
    int enemyCount = 0;
    float enemyTimer = 0.0f;

    initEnemies(levelNum, enemies, &enemyCount);

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

            DrawTexture(instructionbgTexture, 0, 0, WHITE);

            EndDrawing();
            if(IsKeyPressed(KEY_M))
            {
                menu = 1;
            }
            continue;
        }
        // if movement then if inside board and not wall then move player, (if box then check if box can be moved,( if yes then move box and player, (if no then do nothing)))

        if (IsKeyPressed(KEY_R))
        {
            resetLevel(levelNum, &player, enemies, &enemyCount);
            totalcount = countGoals();
            enemyTimer = 0.0f;
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
            int nextX = player.x + moveX, nextY = player.y + moveY;
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
                            player.x = nextX;
                            player.y = nextY;
                            player.moves++;
                        }
                    }
                }
                else if (board[nextY][nextX] != '#')
                {
                    player.x = nextX;
                    player.y = nextY;
                    player.moves++;
                }
            }
        }

        enemyTimer += GetFrameTime();

        if (enemyTimer >= 0.3f && gamewon == 0)
        {
            updateEnemies(enemies, enemyCount);
            enemyTimer = 0.0f;
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
        if(IsKeyPressed(KEY_M))
        {
            menu = 1;

        }


        if(gamewon ==1 && IsKeyPressed(KEY_ENTER))
        {
            levelNum++;
            if(levelNum == 11)
            {
                levelNum = 10;
                gamewon = 2;
            }
            else
            {
                resetLevel(levelNum, &player, enemies, &enemyCount);
                totalcount = countGoals();
                gamewon = 0;
            }
        }
        if(gamewon == 2)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(gameoverTexture, 0, 0, WHITE);
            if(IsKeyPressed(KEY_ENTER) && IsKeyPressed(KEY_M))
            {
                menu = 1;
                gamewon = 0;
                levelNum = 1;
            }
            EndDrawing();
        }
        else
        {
            BeginDrawing();
            ClearBackground(BLACK);

            //nested loops go through each tile of board
            // y = row, x = column

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
                }
            }

            for (int i = 0; i < enemyCount; i++)
            {
                if (enemies[i].active)
                {
                    DrawRectangle(
                        boardoffsetX + enemies[i].x * 32,
                        boardoffsetY + enemies[i].y * 32,
                        32,
                        32,
                        RED
                    );
                }
            }
            DrawTexture(playerTexture, boardoffsetX + player.x * 32, boardoffsetY + player.y * 32, WHITE);

            int margin = 10;
            int fontSize = 20;
            DrawText("MOVE: WASD / ARROWS", margin, margin, 20, GREEN);
            DrawText("RESET: R", margin, margin + 25, 20, RAYWHITE);

            DrawText(TextFormat("MOVES: %d", player.moves), margin, screenheight - fontSize - margin, fontSize, RAYWHITE);
            DrawText(TextFormat("GOALS: %d / %d", goalCount, totalcount), margin, screenheight - fontSize - margin - 25, fontSize, RAYWHITE);
            DrawText(TextFormat("LEVEL: %d", levelNum), margin, screenheight - fontSize - margin - 50, fontSize, BLUE);
            if(gamewon == 1)
            {
                DrawTexture(winTexture, 170, 40, WHITE);
                //DrawText("MISSION COMPLETE!", 245, 30, 20, RAYWHITE); // center of screen (400,400)
            }
            EndDrawing();
        }
    }
    CloseWindow();
    return 0;
}
