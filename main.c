#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <string.h>

#define BOARD_SIZE 16
#define TILE_TYPES 4
#define MAX_ROUTE_LENGTH 64
#define MAX_NAME_LENGTH 32
#define MAX_LEADERBOARD_ENTRIES 100
#define SAVE_FILE "assets/logfiles/save.txt"
#define LEADERBOARD_FILE "assets/logfiles/leaderboard.txt"
#define PROFILE_FILE "assets/logfiles/profile.txt"
#define SETTINGS_FILE "assets/logfiles/settings.txt"
#define MAX_ENEMIES 5
#define ENEMY_LEVEL_COUNT 10
#define SAVE_VERSION 1

const char tileTypes[TILE_TYPES] = {' ', '#', '.', '$'}; // Empty, Wall, Goal, Box
char board[BOARD_SIZE][BOARD_SIZE];
char goals[BOARD_SIZE][BOARD_SIZE];

typedef struct
{
    int x, y;
    int lastX, lastY;
    int hearts;
    int moves;
    int score,levelScore;
    char name[MAX_NAME_LENGTH];
} Player;

typedef struct Enemy Enemy;
struct Enemy
{
    int x, y;
    int routeIndex;
    int routeLength;
    int routeX[MAX_ROUTE_LENGTH];
    int routeY[MAX_ROUTE_LENGTH];
    int active, chasing;
};

typedef struct SaveData SaveData;
struct SaveData
{
    int levelNum;
    Player player;
    char board[BOARD_SIZE][BOARD_SIZE];
    int enemyCount;
    Enemy enemies[MAX_ENEMIES];
};

typedef enum
{
    SCREEN_MENU = 0,
    SCREEN_GAME = 1,
    SCREEN_INSTRUCTIONS = 2,
    SCREEN_GAMEOVER = 3,
    SCREEN_WIN = 4,
    SCREEN_SAVE,
    SCREEN_EXIT_CONFIRM,
    SCREEN_PROFILE,
    SCREEN_LEADERBOARD,
    SCREEN_SETTINGS,
    SCREEN_CREDITS
} Screen;

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int score;
} LeaderboardEntry;

// Main menu
Rectangle menuPlayButton  = { 576, 268, 76, 78 };
Rectangle menuRuleButton  = { 576, 360, 76, 78 };
Rectangle menuMenuButton  = { 576, 360, 76, 76 };
Rectangle menuNewgButton  = { 576, 452, 76, 72 };
Rectangle menuExitButton  = { 134, 272, 76, 72 };
Rectangle menuSettingsButton = { 20, 15, 70, 65 };
Rectangle menuProfileButton  = { 20, 82, 70, 70 };
Rectangle menuLeaderboardButton = { 20, 150, 70, 70 };
Rectangle menuAboutButton = { 20, 218, 70, 70 };
Rectangle backButton = { 290, 600, 200, 55 };
Rectangle instructionsMenuButton = { 576, 268, 76, 78 };
Rectangle instructionsExitButton = { 576, 452, 76, 72 };
Rectangle confirmYesButton   = { 576, 268, 76, 78 };
Rectangle confirmNoButton    = { 576, 360, 76, 78 };
Rectangle confirmExitButton  = { 576, 452, 76, 72 };
Rectangle creditsGithubButton = { 0 };
Rectangle creditsAdvisorButton = { 0 };
Rectangle creditsSourceButton = { 0 };
Rectangle creditsBackButton = { 0 };
bool musicOn = true;
bool keysOn = true;
bool actionsOn = true;
float musicVolume = 0.7f;
int difficulty = 1; // 0 = Easy // 1 = Normal // 2 = Hard
float enemyMoveInterval = 0.3f;
float masterVolume = 0.35f;

Rectangle musicButton  = { 500, 190, 100, 45 };
Rectangle keysButton   = { 500, 270, 100, 45 };
Rectangle actionsButton = { 500, 350, 100, 45 };
Rectangle musicSlider  = { 300, 225, 280, 8 };
Rectangle easyButton   = { 250, 520, 90, 45 };
Rectangle normalButton = { 355, 520, 90, 45 };
Rectangle hardButton   = { 460, 520, 90, 45 };
//List of fn declaration
int isValidEnemyPosition(int x, int y);
int scanForPlayer(Enemy *enemy, Player *player, int *moveX, int *moveY);
void moveEnemyRandomly(Enemy *enemy);
void updateEnemies(Enemy enemies[], int enemyCount, Player *player, int levelnum);
void initEnemies(int levelNum, Enemy enemies[], int *enemyCount);
int playerStartsX[10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
int playerStartsY[10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
void init_Board(int levelNum);
float getEnemyInterval(int levelNum);
void SaveSettings(void);
void LoadSettings(void);
void ApplySettings( Music menuMusic, Sound selectSound, Sound moveSound, Sound loadgameSound, Sound hitSound, Sound winSound, Sound gameOverSound, Sound goalSound);

int mouseClicked(Rectangle button)
{
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), button);
}

void init_Board(int levelNum)
{
    //from line 24 to 214, we defined layout
    char level2[BOARD_SIZE][BOARD_SIZE] =
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

    char level1[BOARD_SIZE][BOARD_SIZE] =
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
        "#   $  #   .   #",
        "#      #       #",
        "#      #       #",
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
        "#              #",
        "#    ## #   ## #",
        "#    ## #   ## #",
        "#         $    #",
        "#  ###  #      #",
        "#  .    #  ##  #",
        "#       $   .  #",
        "#              #",
        "#   $          #",
        "#       #  .   #",
        "#  $##  #      #",
        "#       #   ## #",
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

int saveGame(Player *player, int levelNum, Enemy enemies[], int enemyCount)
{
    FILE *file = fopen(SAVE_FILE, "wb");

    if (file == NULL)
    {
        TraceLog(LOG_ERROR, "Could not open save file.");
        return 0;
    }

    SaveData data;

    data.levelNum = levelNum;
    data.player = *player;

    memcpy(data.board, board, sizeof(board));

    data.enemyCount = enemyCount;

    memcpy(data.enemies,
           enemies,
           sizeof(Enemy) * enemyCount);

    size_t written = fwrite(&data,
                            sizeof(SaveData),
                            1,
                            file);

    fclose(file);

    return written == 1;
}

int loadGame(Player *player, int *levelNum, Enemy enemies[], int *enemyCount)
{
    FILE *file = fopen(SAVE_FILE, "rb");

    if (file == NULL)
    {
        TraceLog(LOG_WARNING, "No save file found.");
        return 0;
    }

    SaveData data;

    size_t read = fread(&data,
                        sizeof(SaveData),
                        1,
                        file);

    fclose(file);

    if (read != 1)
    {
        TraceLog(LOG_ERROR, "Save file is invalid.");
        return 0;
    }
    *levelNum = data.levelNum;
    init_Board(*levelNum);
    *player = data.player;
    memcpy(board,
           data.board,
           sizeof(board));

    *enemyCount = data.enemyCount;

    if (*enemyCount < 0 ||
            *enemyCount > MAX_ENEMIES)
    {
        TraceLog(LOG_ERROR, "Invalid enemy count in save.");
        return 0;
    }

    memcpy(enemies,
           data.enemies,
           sizeof(Enemy) * (*enemyCount));

    return 1;
}

int saveFileExists(void)
{
    FILE *file = fopen( SAVE_FILE, "rb");

    if (file == NULL)
        return 0;

    fclose(file);
    return 1;
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
    player->lastX = player->x;   // ← add
    player->lastY = player->y;
    player->moves = 0;

    initEnemies(levelNum, enemies, enemyCount);
}

void initEnemies(int levelNum, Enemy enemies[], int *enemyCount)
{
    *enemyCount = 0;
    if (levelNum < 1 || levelNum > ENEMY_LEVEL_COUNT) return;
    int numberOfEnemies;
    if (levelNum <= 3) numberOfEnemies = 1;
    else if (levelNum <= 6) numberOfEnemies = 2;
    else numberOfEnemies = 3;
    if (numberOfEnemies > MAX_ENEMIES)
        numberOfEnemies = MAX_ENEMIES;
    for (int e = 0; e < numberOfEnemies; e++)
    {
        int found = 0;
        for (int y = BOARD_SIZE - 2; y >= 1 && !found; y--)
        {
            for (int x = BOARD_SIZE - 2; x >= 1 && !found; x--)
            {
                if (x == playerStartsX[levelNum - 1] && y == playerStartsY[levelNum - 1]) continue;
                int distance = abs(x - playerStartsX[levelNum - 1]) + abs(y - playerStartsY[levelNum - 1]);
                if (distance < 5) continue;
                if (isValidEnemyPosition(x, y))
                {
                    int occupied = 0;
                    for (int j = 0; j < *enemyCount; j++)
                    {
                        if (enemies[j].active && enemies[j].x == x && enemies[j].y == y)
                        {
                            occupied = 1;
                            break;
                        }
                    }
                    if (occupied) continue;
                    enemies[*enemyCount].x = x;
                    enemies[*enemyCount].y = y;
                    enemies[*enemyCount].routeIndex = 0;
                    enemies[*enemyCount].routeLength = 0;
                    enemies[*enemyCount].active = 1;
                    enemies[*enemyCount].chasing = 0;
                    (*enemyCount)++;
                    found = 1;
                }
            }
        }
    }
}

int isValidEnemyPosition(int x, int y)
{
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) return 0;
    if (board[y][x] == '#' || board[y][x] == '$') return 0;
    return 1;
}

int scanForPlayer(Enemy *enemy, Player *player, int *moveX, int *moveY)
{
    int directionsX[4] = { 0,  0,  1, -1 };
    int directionsY[4] = { 1, -1,  0,  0 };
    for (int direction = 0; direction < 4; direction++)
    {
        int x = enemy->x + directionsX[direction];
        int y = enemy->y + directionsY[direction];
        while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
        {
            if (x == player->x && y == player->y)
            {
                *moveX = directionsX[direction];
                *moveY = directionsY[direction];
                return 1;
            }
            if (board[y][x] == '#' || board[y][x] == '$') break;
            x += directionsX[direction];
            y += directionsY[direction];
        }
    }
    return 0;
}

void moveEnemyRandomly(Enemy *enemy)
{
    int directionsX[4] = { 0,  0,  1, -1 };
    int directionsY[4] = { 1, -1,  0,  0 };

    /*
     * Try all four directions in random order.
     *
     * This prevents the enemy from getting stuck simply
     * because the first randomly selected direction was blocked.
     */
    int startDirection = GetRandomValue(0, 3);

    for (int i = 0; i < 4; i++)
    {
        int direction = (startDirection + i) % 4;

        int nextX = enemy->x + directionsX[direction];
        int nextY = enemy->y + directionsY[direction];

        if (isValidEnemyPosition(nextX, nextY))
        {
            enemy->x = nextX;
            enemy->y = nextY;

            return;
        }
    }

    /*
     * If all four directions are blocked,
     * the enemy simply stays where it is.
     */
}

void moveEnemyToward(Enemy *enemy, int targetX, int targetY)
{
    int dx   = targetX - enemy->x;
    int dy   = targetY - enemy->y;
    int absX = dx < 0 ? -dx : dx;
    int absY = dy < 0 ? -dy : dy;
    int stepX = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    int stepY = dy > 0 ? 1 : (dy < 0 ? -1 : 0);

    // Try dominant axis first, then secondary, then random
    if (absX >= absY)
    {
        if (stepX && isValidEnemyPosition(enemy->x + stepX, enemy->y))
        {
            enemy->x += stepX;
            return;
        }
        if (stepY && isValidEnemyPosition(enemy->x, enemy->y + stepY))
        {
            enemy->y += stepY;
            return;
        }
    }
    else
    {
        if (stepY && isValidEnemyPosition(enemy->x, enemy->y + stepY))
        {
            enemy->y += stepY;
            return;
        }
        if (stepX && isValidEnemyPosition(enemy->x + stepX, enemy->y))
        {
            enemy->x += stepX;
            return;
        }
    }
    moveEnemyRandomly(enemy); // all preferred directions blocked
}

void updateEnemies(Enemy enemies[], int enemyCount, Player *player, int levelNum)
{
    int dirX = player->x - player->lastX;
    int dirY = player->y - player->lastY;
    int predX = player->x + dirX * 3;
    int predY = player->y + dirY * 3;
    if (predX < 1) predX = 1;
    if (predX > BOARD_SIZE - 2) predX = BOARD_SIZE - 2;
    if (predY < 1) predY = 1;
    if (predY > BOARD_SIZE - 2) predY = BOARD_SIZE - 2;
    if (board[predY][predX] == '#' ||
            board[predY][predX] == '$')
    {
        predX = player->x;
        predY = player->y;
    }

    for (int i = 0; i < enemyCount; i++)
    {
        if (!enemies[i].active) continue;
        int moveX = 0;
        int moveY = 0;
        int seesPlayer = scanForPlayer(&enemies[i], player, &moveX, &moveY);
        if (levelNum <= 3)
        {
            if (seesPlayer)
            {
                enemies[i].chasing = 1;
                moveEnemyToward(
                    &enemies[i],
                    player->x,
                    player->y
                );
            }
            else
            {
                enemies[i].chasing = 0;
                moveEnemyRandomly(&enemies[i]);
            }
        }
        else if (levelNum <= 6)
        {
            if (seesPlayer)
            {
                enemies[i].chasing = 1;

                moveEnemyToward(
                    &enemies[i],
                    player->x,
                    player->y
                );
            }
            else
            {
                enemies[i].chasing = 0;

                moveEnemyToward(
                    &enemies[i],
                    player->x,
                    player->y
                );
            }
        }
        else
        {
            if (seesPlayer)
            {
                enemies[i].chasing = 1;

                moveEnemyToward(
                    &enemies[i],
                    player->x,
                    player->y
                );
            }
            else
            {
                enemies[i].chasing = 0;

                if (i % 2 == 0)
                {
                    moveEnemyToward(
                        &enemies[i],
                        predX,
                        predY
                    );
                }
                else
                {
                    moveEnemyToward(
                        &enemies[i],
                        player->x,
                        player->y
                    );
                }
            }
        }
    }
}

int checkEnemyCollision(Player player, Enemy enemies[], int enemyCount)
{
    for (int i = 0; i < enemyCount; i++)
    {
        if (!enemies[i].active)
            continue;

        if (player.x == enemies[i].x &&
                player.y == enemies[i].y)
        {
            return 1;
        }
    }

    return 0;
}

void handleEnemyHit(int levelNum, Player *player, Enemy enemies[], int *enemyCount, int *totalcount, int *gamewon, float *enemyTimer, Sound hitSound)
{
    player->hearts--;
    PlaySound(hitSound);

    if (player->hearts <= 0)
    {
        player->hearts = 0;
        *gamewon = 2;
        return;
    }

    // Save everything we want to keep across the reset
    int savedHearts    = player->hearts;
    int savedScore     = player->score;     // ← save BEFORE any modification
    char savedName[MAX_NAME_LENGTH];
    strcpy(savedName, player->name);

    // Penalty: lose only the points earned this level
    savedScore -= player->levelScore;

    if (savedScore < 0) savedScore = 0;     // floor at zero

    // Reset the level (this overwrites player fields)
    resetLevel(levelNum, player, enemies, enemyCount);
    enemyMoveInterval = getEnemyInterval(levelNum);
    // Restore everything
    player->hearts     = savedHearts;
    player->score      = savedScore;        // ← restored with penalty already applied
    player->levelScore = 0;                 // ← level score starts fresh
    strcpy(player->name, savedName);

    *totalcount = countGoals();
    *enemyTimer = 0.0f;
}

void addEnemy(Enemy enemies[], int *enemyCount, int index, int routeX[], int routeY[], int routeLength)
{
    if (index >= MAX_ENEMIES) return;
    if (routeLength > MAX_ROUTE_LENGTH) routeLength = MAX_ROUTE_LENGTH;
    enemies[index].active = 1;
    enemies[index].routeIndex = 0;
    enemies[index].routeLength = routeLength;
    for (int i = 0; i < routeLength; i++)
    {
        enemies[index].routeX[i] = routeX[i];
        enemies[index].routeY[i] = routeY[i];
    }
    enemies[index].x = routeX[0];
    enemies[index].y = routeY[0];
    (*enemyCount)++;
}

void startNewGame(int *levelNum, Player *player, Enemy enemies[], int *enemyCount, int *totalcount, float *enemyTimer, int *gamewon)
{
    char savedName[MAX_NAME_LENGTH];
    strcpy(savedName, player->name);
    *levelNum = 1;
    player->x = playerStartsX[0];
    player->y = playerStartsY[0];
    player->hearts = 3;
    player->moves = 0;
    player->score = 0;
    player->levelScore = 0;
    strcpy(player->name, savedName);
    resetLevel(*levelNum, player, enemies, enemyCount);
    enemyMoveInterval = getEnemyInterval(*levelNum);
    *totalcount = countGoals();
    *enemyTimer = 0.0f;
    *gamewon = 0;
}

void applyMoveScore(Player *player, int landedOnGoal)
{
    if (player->moves % 2 == 0)
    {
        player->score--;
        player->levelScore--;
        if (player->score < 0) player->score = 0;
        if (player->levelScore < 0) player->levelScore = 0;
    }
    if (landedOnGoal)
    {
        player->score += 100;
        player->levelScore += 100;
    }
}

void saveLeaderboardScore(const char *name, int score)
{
    FILE *file = fopen(LEADERBOARD_FILE, "a");
    if (file == NULL)
    {
        TraceLog(LOG_ERROR, "Could not open leaderboard: %s", LEADERBOARD_FILE);
        return;
    }
    fprintf(file, "%s %d\n", name, score);
    fclose(file);
    TraceLog(LOG_INFO, "Leaderboard saved: %s = %d", name, score);
}

void recordScoreOnce(Player *player, int *scoreRecorded)
{
    if (!*scoreRecorded)
    {
        saveLeaderboardScore(player->name, player->score);
        *scoreRecorded = 1;
    }
}

int loadLeaderboard(LeaderboardEntry entries[])
{
    FILE *file = fopen(LEADERBOARD_FILE, "r");

    if (file == NULL) return 0;
    int count = 0;
    while (count < MAX_LEADERBOARD_ENTRIES && fscanf(file, "%31s %d", entries[count].name, &entries[count].score) == 2)
    {
        count++;
    }
    fclose(file);
    return count;
}

void sortLeaderboard(LeaderboardEntry entries[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (entries[j].score > entries[i].score)
            {
                LeaderboardEntry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }
}

void saveProfileName(const char *name)
{
    FILE *file = fopen(PROFILE_FILE, "w");
    if (file == NULL) return;
    fprintf(file, "%s\n", name);
    fclose(file);
}

void loadProfileName(char *name)
{
    FILE *file = fopen(PROFILE_FILE, "r");
    if (file == NULL)
    {
        strcpy(name, "PLAYER");
        return;
    }
    if (fscanf(file, "%31s", name) != 1) strcpy(name, "PLAYER");
    fclose(file);
}

float getEnemyInterval(int levelNum)
{
    float base;

    if (difficulty == 0)
        base = 0.50f;
    else if (difficulty == 1)
        base = 0.34f;
    else
        base = 0.24f;

    if (levelNum <= 3)
        return base + 0.10f;

    if (levelNum <= 6)
        return base;

    return base - 0.05f;
}

void SaveSettings(void)
{
    FILE *file = fopen(SETTINGS_FILE, "w");

    if (file == NULL)
    {
        printf("Failed to save settings.\n");
        return;
    }

    fprintf(file, "%d\n", musicOn);
    fprintf(file, "%d\n", keysOn);
    fprintf(file, "%d\n", actionsOn);
    fprintf(file, "%.2f\n", musicVolume);
    fprintf(file, "%d\n", difficulty);

    fclose(file);
}

void LoadSettings(void)
{
    FILE *file = fopen(SETTINGS_FILE, "r");

    if (file == NULL)
    {
        return; // Use default settings
    }

    int loadedMusicOn;
    int loadedKeysOn;
    int loadedActionsOn;
    float loadedMusicVolume;
    int loadedDifficulty;

    if (fscanf(file, "%d", &loadedMusicOn) != 1 ||
            fscanf(file, "%d", &loadedKeysOn) != 1 ||
            fscanf(file, "%d", &loadedActionsOn) != 1 ||
            fscanf(file, "%f", &loadedMusicVolume) != 1 ||
            fscanf(file, "%d", &loadedDifficulty) != 1)
    {
        printf("Invalid settings file. Using defaults.\n");
        fclose(file);
        return;
    }

    musicOn = loadedMusicOn;
    keysOn = loadedKeysOn;
    actionsOn = loadedActionsOn;

    if (loadedMusicVolume < 0.0f)
        loadedMusicVolume = 0.0f;

    if (loadedMusicVolume > 1.0f)
        loadedMusicVolume = 1.0f;

    musicVolume = loadedMusicVolume;

    if (loadedDifficulty < 0)
        loadedDifficulty = 0;

    if (loadedDifficulty > 2)
        loadedDifficulty = 2;

    difficulty = loadedDifficulty;

    fclose(file);
}

void ApplySettings( Music menuMusic, Sound selectSound, Sound moveSound, Sound loadgameSound, Sound hitSound, Sound winSound, Sound gameOverSound, Sound goalSound)
{
    SetMusicVolume(
        menuMusic,
        musicOn ? musicVolume : 0.0f
    );

    SetSoundVolume(
        selectSound,
        keysOn ? masterVolume : 0.0f
    );

    SetSoundVolume(
        moveSound,
        keysOn ? masterVolume : 0.0f
    );

    SetSoundVolume(
        hitSound,
        actionsOn ? masterVolume : 0.0f
    );

    SetSoundVolume(
        loadgameSound,
        actionsOn ? masterVolume : 0.0f
    );

    SetSoundVolume(
        winSound,
        actionsOn ? masterVolume : 0.0f
    );

    SetSoundVolume(
        gameOverSound,
        actionsOn ? masterVolume : 0.0f
    );

    SetSoundVolume(
        goalSound,
        actionsOn ? masterVolume : 0.0f
    );
}

int main(void)
{
    const int screenwidth = 800, screenheight = 800;
    InitWindow(screenwidth, screenheight, "SEIBISHI");
    SetExitKey(KEY_NULL);
    InitAudioDevice();
    LoadSettings();
    //audio/.wavfiles
    Music menuMusic = LoadMusicStream("assets/audio/introwavybgm.mp3");
    menuMusic.looping = true;
    Sound selectSound = LoadSound("assets/audio/select.wav");
    Sound moveSound = LoadSound("assets/audio/move.wav");
    Sound loadgameSound = LoadSound("assets/audio/loadgame.wav");
    Sound hitSound = LoadSound("assets/audio/hit.wav");
    Sound winSound = LoadSound("assets/audio/newlevel.wav");
    Sound gameOverSound = LoadSound("assets/audio/gameover.wav");
    Sound goalSound = LoadSound("assets/audio/boxongoal.wav");
    ApplySettings( menuMusic, selectSound,moveSound,loadgameSound,hitSound, winSound,gameOverSound,goalSound);
    SetTargetFPS(60);
    //texture/.pngfiles
    Texture2D floorTexture = LoadTexture("assets/texture/floor.png");
    Texture2D wallTexture = LoadTexture("assets/texture/wall.png");
    Texture2D goalTexture = LoadTexture("assets/texture/goal.png");
    Texture2D boxTexture = LoadTexture("assets/texture/box.png");
    Texture2D playerTexture = LoadTexture("assets/texture/player.png");
    Texture2D enemyTexture = LoadTexture("assets/texture/enemy.png");
    Texture2D boxOnGoalTexture = LoadTexture("assets/texture/box_goal.png");
    Texture2D menubgTexture = LoadTexture("assets/texture/menubg.png");
    Texture2D instructionbgTexture = LoadTexture("assets/texture/instructions.png");
    Texture2D gameoverTexture = LoadTexture("assets/texture/gameover.png");
    Texture2D escapeConfirm = LoadTexture("assets/texture/escConf.png");
    Texture2D gamewinTexture = LoadTexture("assets/texture/gamewin.png");
    Texture2D heartTexture = LoadTexture("assets/texture/heart.png");
    Screen screen = SCREEN_MENU;
    Screen previousScreen = SCREEN_GAME;
    int exitRequested = 0;

    int levelNum = 1;
    int menu = 1;
    init_Board(levelNum);

    Player player;
    loadProfileName(player.name);
    player.x = playerStartsX[levelNum - 1];
    player.y = playerStartsY[levelNum - 1];
    player.hearts = 3;
    player.moves = 0;
    player.score = 0;
    strcpy(player.name, "PLAYER");

    Enemy enemies[MAX_ENEMIES];
    int enemyCount = 0;
    float enemyTimer = 0.0f;
    initEnemies(levelNum, enemies, &enemyCount);

    LeaderboardEntry entries[MAX_LEADERBOARD_ENTRIES];
    int count = loadLeaderboard(entries);
    sortLeaderboard(entries, count);
    for (int i = 0; i < count && i < 3; i++)
    {
        printf("%d. %s - %d\n", i + 1, entries[i].name, entries[i].score);
    }
    int boardWidth = BOARD_SIZE * 32, boardHeight = BOARD_SIZE * 32;
    int boardoffsetX = (screenwidth - boardWidth) / 2;
    int boardoffsetY = (screenheight - boardHeight) / 2;

    int goalCount = 0;
    int totalcount = countGoals();
    int gamewon = 0;
    int scoreRecorded = 0;
    int profileEditing = 0;

    PlayMusicStream(menuMusic);
    while (!WindowShouldClose() &&(exitRequested==0))
    {
        UpdateMusicStream(menuMusic);
        if (screen == SCREEN_EXIT_CONFIRM)
        {
            BeginDrawing();
            ClearBackground(BLACK);

            DrawTexture(escapeConfirm, 0, 0, WHITE);

            Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointRec(mouse, confirmYesButton))
                DrawRectangleRec(confirmYesButton, Fade(RED, 0.19f));

            if (CheckCollisionPointRec(mouse, confirmNoButton))
                DrawRectangleRec(confirmNoButton, Fade(GREEN, 0.19f));

            EndDrawing();

            // YES: Exit the game while saving
            if (IsKeyPressed(KEY_Y) || mouseClicked(confirmYesButton))
            {
                PlaySound(selectSound);
                if(saveGame(&player, levelNum, enemies, enemyCount)) TraceLog(LOG_INFO, "Game saved successfully.");
                else TraceLog(LOG_ERROR, "Failed to save game.");
                exitRequested = 1;
                continue;
            }

            // NO: Exit the game without saving
            if (IsKeyPressed(KEY_N) || mouseClicked(confirmNoButton) )
            {
                PlaySound(selectSound);
                exitRequested = 2;
                continue;
            }
            // Esc: Return to the previous screen
            if (IsKeyPressed(KEY_ESCAPE) || mouseClicked(confirmExitButton))
            {
                PlaySound(selectSound);
                screen = previousScreen;
            }


            continue;
        }
        if (screen == SCREEN_SAVE)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(escapeConfirm, 0, 0, WHITE);
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, confirmYesButton))
            {
                DrawRectangleRec( confirmYesButton, Fade(RED, 0.19f));
            }
            if (CheckCollisionPointRec(mouse, confirmNoButton))
            {
                DrawRectangleRec( confirmNoButton, Fade(GREEN, 0.19f));
            }
            if (CheckCollisionPointRec(mouse, confirmExitButton))
            {
                DrawRectangleRec( confirmExitButton, Fade(YELLOW, 0.10f));
            }
            EndDrawing();
            if (IsKeyPressed(KEY_Y) ||  mouseClicked(confirmYesButton))
            {
                PlaySound(selectSound);
                if (saveGame(&player, levelNum, enemies, enemyCount))
                {
                    TraceLog(LOG_INFO, "Game saved successfully.");
                }
                else
                {
                    TraceLog(LOG_ERROR, "Failed to save game.");
                }
                screen = SCREEN_MENU;
                continue;
            }
            if (IsKeyPressed(KEY_N) || mouseClicked(confirmNoButton))
            {
                PlaySound(selectSound);
                screen = SCREEN_MENU;
                continue;
            }
            if (IsKeyPressed(KEY_ESCAPE) || mouseClicked(confirmExitButton))
            {
                PlaySound(selectSound);
                screen = previousScreen;
                continue;
            }
            continue;
        }
        if (screen == SCREEN_MENU)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(menubgTexture, 0, 0, WHITE);
            DrawRectangleRec( menuSettingsButton, Fade(BLACK, 0.25f) );
            DrawRectangleRec( menuProfileButton, Fade(BLACK, 0.25f) );
            DrawRectangleRec( menuLeaderboardButton, Fade(BLACK, 0.25f) );
            DrawRectangleRec(menuAboutButton, Fade(BLACK, 0.25f));
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, menuSettingsButton))
            {
                DrawRectangleRec( menuSettingsButton, Fade(WHITE, 0.12f));
            }
            if (CheckCollisionPointRec(mouse, menuProfileButton))
            {
                DrawRectangleRec( menuProfileButton, Fade(WHITE, 0.12f));
            }
            if (CheckCollisionPointRec(mouse, menuLeaderboardButton))
            {
                DrawRectangleRec( menuLeaderboardButton, Fade(WHITE, 0.12f) );
            }
            if (CheckCollisionPointRec(mouse, menuExitButton))
            {
                DrawRectangleRec( menuExitButton, Fade(RED, 0.12f));
            }
            if (CheckCollisionPointRec(mouse, menuNewgButton))
            {
                DrawRectangleRec( menuNewgButton, Fade(YELLOW, 0.12f) );
            }
            if (CheckCollisionPointRec(mouse, menuPlayButton))
            {
                DrawRectangleRec( menuPlayButton, Fade(GREEN, 0.12f) );
            }
            if (CheckCollisionPointRec(mouse, menuAboutButton))
            {
                DrawRectangleRec(menuAboutButton, Fade(WHITE, 0.12f));
            }
            EndDrawing();
            if (IsKeyPressed(KEY_ENTER) || mouseClicked(menuPlayButton))
            {
                PlaySound(selectSound);
                if (loadGame(&player, &levelNum, enemies, &enemyCount))
                {
                    totalcount = countGoals();
                    enemyMoveInterval = getEnemyInterval(levelNum);
                    enemyTimer = 0.0f;
                    gamewon = 0;
                    scoreRecorded = 0;
                    TraceLog(LOG_INFO, "Game loaded successfully.");
                    screen = SCREEN_GAME;
                }
                else
                {
                    TraceLog(LOG_WARNING, "No valid save found.");
                    startNewGame(&levelNum, &player, enemies, &enemyCount, &totalcount, &enemyTimer, &gamewon);
                    screen = SCREEN_GAME;
                }
            }
            if (IsKeyPressed(KEY_BACKSLASH) || mouseClicked(menuRuleButton))
            {
                PlaySound(selectSound);
                screen = SCREEN_INSTRUCTIONS;
            }
            if (IsKeyPressed(KEY_ESCAPE) || mouseClicked(menuExitButton))
            {
                PlaySound(selectSound);
                previousScreen = SCREEN_MENU;
                screen = SCREEN_EXIT_CONFIRM;
            }
            if(IsKeyPressed(KEY_G) || mouseClicked(menuNewgButton))
            {
                PlaySound(loadgameSound);
                TraceLog(LOG_INFO, "DEBUG: NEWG pressed");
                startNewGame(&levelNum, &player, enemies, &enemyCount, &totalcount, &enemyTimer, &gamewon);
                scoreRecorded = 0;
                screen = SCREEN_GAME;
                scoreRecorded = 0;
                screen = SCREEN_GAME;
            }
            if (mouseClicked(menuSettingsButton))
            {
                PlaySound(selectSound);
                previousScreen = SCREEN_MENU;
                screen = SCREEN_SETTINGS;
            }
            if (mouseClicked(menuProfileButton))
            {
                PlaySound(selectSound);
                previousScreen = SCREEN_MENU;
                screen = SCREEN_PROFILE;
            }
            if (mouseClicked(menuLeaderboardButton))
            {
                PlaySound(selectSound);
                previousScreen = SCREEN_MENU;
                screen = SCREEN_LEADERBOARD;
            }
            if (mouseClicked(menuAboutButton))
            {
                PlaySound(selectSound);
                previousScreen = SCREEN_MENU;
                screen = SCREEN_CREDITS;
            }
            continue;
        }
        if (screen == SCREEN_PROFILE)
        {
            BeginDrawing();
            ClearBackground((Color)
            {
                5, 10, 55, 255
            });

            DrawText("PROFILE", 320, 80, 40, RAYWHITE);
            DrawText("PLAYER NAME:", 250, 180, 25, RAYWHITE);

            DrawRectangle(220, 225, 360, 55, DARKGRAY);
            DrawRectangleLines(220, 225, 360, 55, RAYWHITE);
            DrawText(player.name, 235, 240, 25, WHITE);

            DrawText("TYPE NAME", 325, 310, 18, GRAY);

            // SAVE button
            Rectangle saveButton = { 250, 590, 140, 55 };
            DrawRectangleRec(saveButton, Fade(BLACK, 0.4f));
            DrawRectangleLinesEx(saveButton, 2, RAYWHITE);
            DrawText("SAVE", 290, 605, 20, RAYWHITE);

            // BACK button
            Rectangle backButton = { 405, 590, 140, 55 };
            DrawRectangleRec(backButton, Fade(BLACK, 0.4f));
            DrawRectangleLinesEx(backButton, 2, RAYWHITE);
            DrawText("BACK", 450, 605, 20, RAYWHITE);
            if (CheckCollisionPointRec(GetMousePosition(), backButton)) DrawRectangleRec(backButton, Fade(WHITE, 0.15f));
            if (CheckCollisionPointRec(GetMousePosition(), saveButton)) DrawRectangleRec(saveButton, Fade(WHITE, 0.15f));

            EndDrawing();

            // Name input
            int key = GetCharPressed();

            while (key > 0)
            {
                if (key >= 32 && key <= 125 &&
                        strlen(player.name) < MAX_NAME_LENGTH - 1)
                {
                    int len = strlen(player.name);
                    player.name[len] = (char)key;
                    player.name[len + 1] = '\0';
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                int len = strlen(player.name);
                if (len > 0)
                    player.name[len - 1] = '\0';
            }

            // SAVE
            if (mouseClicked(saveButton))
            {
                saveProfileName(player.name);
                PlaySound(selectSound);
                screen = previousScreen;
            }

            // BACK
            if (mouseClicked(backButton))
            {
                PlaySound(selectSound);
                screen = previousScreen;
            }

            continue;
        }
        if (screen == SCREEN_LEADERBOARD)
        {
            LeaderboardEntry e[MAX_LEADERBOARD_ENTRIES];
            int count = loadLeaderboard(e);
            sortLeaderboard(e, count);
            BeginDrawing();
            ClearBackground((Color)
            {
                5, 10, 55, 255
            });
            DrawText("LEADERBOARD", 250, 55, 40, RAYWHITE);
            int shown = count > 7 ? 7 : count;

            if (shown == 0) DrawText("NO SCORES YET", 285, 180, 25, GRAY);
            for (int i = 0; i < shown; i++)
            {
                Color bar = i == 0 ? GOLD : i == 1 ? (Color)
                {
                    192,192,192,255
                } :
                i == 2 ? (Color)
                {
                    205,127,50,255
                } :
                (Color)
                {
                    25,35,80,255
                };
                Rectangle r = {150, 125 + i * 58, 500, 48};
                DrawRectangleRec(r, bar);
                DrawRectangleLinesEx(r, 2, RAYWHITE);
                DrawText(TextFormat("#%d", i + 1), 170, r.y + 12, 20, BLACK);
                DrawText(e[i].name, 245, r.y + 12, 20, i < 3 ? BLACK : RAYWHITE);
                DrawText(TextFormat("%d", e[i].score), 550, r.y + 12, 20, i < 3 ? BLACK : GOLD);
            }
            DrawRectangleRec(backButton, Fade(BLACK, 0.4f));
            if (CheckCollisionPointRec(GetMousePosition(), backButton)) DrawRectangleRec(backButton, Fade(WHITE, 0.15f));
            DrawRectangleLinesEx(backButton, 2, RAYWHITE);
            DrawText("BACK", 360, 620, 25, RAYWHITE);
            EndDrawing();
            if (mouseClicked(backButton))
            {
                PlaySound(selectSound);
                screen = previousScreen;
            }
            continue;
        }
        if (screen == SCREEN_INSTRUCTIONS)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(instructionbgTexture, 0, 0, WHITE);
            DrawRectangleRec( menuMenuButton, Fade(BLACK, 0.25f) );
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, menuMenuButton))
            {
                DrawRectangleRec( menuMenuButton, Fade(WHITE, 0.12f) );
            }
            EndDrawing();
            if (IsKeyPressed(KEY_M) || mouseClicked(menuMenuButton))
            {
                PlaySound(selectSound);
                screen = SCREEN_MENU;
            }
            continue;
        }
        if (screen == SCREEN_SETTINGS)
        {
            BeginDrawing();
            ClearBackground((Color)
            {
                5, 10, 55, 255
            });
            DrawText("SETTINGS", 300, 70, 40, RAYWHITE);
            // MUSIC
            DrawText("MUSIC", 150, 185, 22, RAYWHITE);
            DrawText("Background Music", 150, 215, 18, LIGHTGRAY);

            DrawRectangleRec(musicButton, musicOn ? DARKGREEN : DARKGRAY);
            DrawRectangleLinesEx(musicButton, 2, RAYWHITE);
            DrawText(musicOn ? "ON" : "OFF", 530, 203, 20, RAYWHITE);

            // MUSIC VOLUME SLIDER
            DrawRectangle(300, 245, 280, 6, DARKGRAY);
            DrawRectangle(
                300,
                245,
                (int)(280 * musicVolume),
                6,
                GOLD
            );
            DrawCircle(
                300 + (int)(280 * musicVolume),
                248,
                9,
                RAYWHITE
            );

            // KEYS
            DrawText("KEYS", 150, 300, 22, RAYWHITE);
            DrawText("Select / Move", 150, 330, 18, LIGHTGRAY);

            DrawRectangleRec(keysButton, keysOn ? DARKGREEN : DARKGRAY);
            DrawRectangleLinesEx(keysButton, 2, RAYWHITE);
            DrawText(keysOn ? "ON" : "OFF", 530, 283, 20, RAYWHITE);

            // ACTIONS
            DrawText("ACTIONS", 150, 385, 22, RAYWHITE);
            DrawText("Hit / Load / Win", 150, 415, 18, LIGHTGRAY);

            DrawRectangleRec(actionsButton, actionsOn ? DARKGREEN : DARKGRAY);
            DrawRectangleLinesEx(actionsButton, 2, RAYWHITE);
            DrawText(actionsOn ? "ON" : "OFF", 530, 363, 20, RAYWHITE);

            // BACK
            DrawRectangleRec(backButton, Fade(BLACK, 0.4f));
            DrawRectangleLinesEx(backButton, 2, RAYWHITE);
            DrawText("BACK", 365, 615, 20, RAYWHITE);
            DrawText("DIFFICULTY", 150, 455, 22, RAYWHITE);

            DrawRectangleRec(
                easyButton,
                difficulty == 0 ? PURPLE : DARKGRAY
            );
            DrawRectangleLinesEx(easyButton, 2, RAYWHITE);
            DrawText("EASY", 270, 533, 18, RAYWHITE);

            DrawRectangleRec(
                normalButton,
                difficulty == 1 ? GREEN : DARKGRAY
            );
            DrawRectangleLinesEx(normalButton, 2, RAYWHITE);
            DrawText("NORMAL", 365, 533, 18, RAYWHITE);

            DrawRectangleRec(
                hardButton,
                difficulty == 2 ? RED : DARKGRAY
            );
            DrawRectangleLinesEx(hardButton, 2, RAYWHITE);
            DrawText("HARD", 485, 533, 18, RAYWHITE);
            EndDrawing();

            // MUSIC ON / OFF
            if (mouseClicked(musicButton))
            {
                musicOn = !musicOn;
                ApplySettings( menuMusic, selectSound,moveSound,loadgameSound,hitSound, winSound,gameOverSound,goalSound);
                PlaySound(selectSound);
                SaveSettings();
            }

            // MUSIC VOLUME SLIDER
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 m = GetMousePosition();

                if (CheckCollisionPointRec( m, (Rectangle)
            {
                290, 230, 300, 35
            }))
                {
                    musicVolume = (m.x - 300) / 280.0f;
                    if (musicVolume < 0.0f) musicVolume = 0.0f;
                    if (musicVolume > 1.0f) musicVolume = 1.0f;
                    if (musicOn) SetMusicVolume(menuMusic, musicVolume);
                    SaveSettings();
                }
            }

            // KEYS ON / OFF
            if (mouseClicked(keysButton))
            {
                keysOn = !keysOn;
                ApplySettings( menuMusic, selectSound,moveSound,loadgameSound,hitSound, winSound,gameOverSound,goalSound);
                SaveSettings();
                PlaySound(selectSound);
            }

            // ACTIONS ON / OFF
            if (mouseClicked(actionsButton))
            {
                actionsOn = !actionsOn;
                ApplySettings( menuMusic, selectSound,moveSound,loadgameSound,hitSound, winSound,gameOverSound,goalSound);
                SaveSettings();
            }

            // BACK
            if (mouseClicked(backButton))
            {
                PlaySound(selectSound);
                screen = previousScreen;
            }
            if (mouseClicked(easyButton))
            {
                difficulty = 0;
                enemyMoveInterval = getEnemyInterval(levelNum);
                PlaySound(selectSound);
                SaveSettings();
            }

            if (mouseClicked(normalButton))
            {
                difficulty = 1;
                enemyMoveInterval = getEnemyInterval(levelNum);
                PlaySound(selectSound);
                SaveSettings();
            }

            if (mouseClicked(hardButton))
            {
                difficulty = 2;
                enemyMoveInterval = getEnemyInterval(levelNum);
                PlaySound(selectSound);
                SaveSettings();
            }

            continue;
        }
        if (screen == SCREEN_CREDITS)
        {
            BeginDrawing();
            ClearBackground(BLACK);

            const int cx = 384;
            const Color accent = {255, 180, 205, 255};
            const Color link = {120, 190, 255, 255};
            const Color muted = {170, 170, 180, 255};

            Rectangle panel = {80, 35, 608, 575};
            DrawRectangleRounded(panel, 0.035f, 12, Fade((Color)
            {
                18,18,28,255
            }, 0.96f));
            DrawRectangleRoundedLines(panel, 0.035f, 12, Fade(accent, 0.45f));
            DrawText("CREDITS", cx - MeasureText("CREDITS", 40) / 2, 58, 40, RAYWHITE);
            DrawLine(145, 112, 623, 112, Fade(accent, 0.45f));
            DrawText("SEIBISHI", cx - MeasureText("SEIBISHI", 30) / 2, 132, 30, accent);
            DrawText("STUDENTS", cx - MeasureText("STUDENTS", 20) / 2, 185, 20, RAYWHITE);
            const char *student1 = "2505139";
            const char *student2 = "2505124";
            Rectangle github1 =
            {
                cx -30, 220,
                MeasureText(student1, 18), 22
            };
            Rectangle github2 =
            {
                cx -30, 250,
                MeasureText(student2, 18), 22
            };
            DrawText(student1, github1.x, github1.y, 18, link);
            DrawText(student2, github2.x, github2.y, 18, link);
            DrawText("ADVISOR", cx - MeasureText("ADVISOR", 20) / 2, 290, 20, RAYWHITE);
            DrawText("JYK", cx - MeasureText("JYK", 18) / 2, 325, 18, muted);
             Rectangle advisor =
            {
                cx - 130, 355, 260, 25
            };
            DrawText("junaedyounuskhan.com", cx - MeasureText("junaedyounuskhan.com", 17) / 2, 355, 17, link);
            DrawText("GAME LIBRARY", cx - MeasureText("GAME LIBRARY", 20) / 2, 405, 20, RAYWHITE);
            Rectangle raylib =
            {
                cx - MeasureText("raylib.com", 17) / 2,
                440,
                MeasureText("raylib.com", 17),
                22
            };
            DrawText("raylib.com", raylib.x, raylib.y, 17, link);
            DrawText("EXTERNAL SOURCE", cx - MeasureText("EXTERNAL SOURCE", 20) / 2, 485, 20, RAYWHITE);
            Rectangle source =
            {
                cx - MeasureText("opengameart.org", 18) / 2, 520, MeasureText("opengameart.org", 18), 22
            };
            DrawText("opengameart.org", source.x, source.y, 18, link);
            Rectangle back = {290, 565, 188, 34};
            DrawRectangleRounded(back, 0.25f, 8, Fade(BLACK, 0.65f));
            DrawRectangleRoundedLines(back, 0.25f, 8, Fade(RAYWHITE, 0.35f));
            DrawText("ESC / M  :  BACK", back.x + (back.width - MeasureText("ESC / M  :  BACK", 16)) / 2, back.y + 8, 16, RAYWHITE);
            Vector2 mouse = GetMousePosition();

            Rectangle links[] = {github1, github2, advisor, raylib, source};

            for (int i = 0; i < 5; i++)
            {
                if (CheckCollisionPointRec(mouse, links[i]))
                    DrawRectangleRounded(
                        links[i], 0.2f, 6, Fade(link, 0.10f)
                    );
            }

            if (CheckCollisionPointRec(mouse, back))
                DrawRectangleRounded(
                    back, 0.25f, 8, Fade(WHITE, 0.10f)
                );

            EndDrawing();

            /* -------------------------------------------------
               HYPERLINK ACTIONS
               ------------------------------------------------- */
            if (mouseClicked(github1))
                OpenURL("https://github.com/mmurrythm");

            if (mouseClicked(github2))
                OpenURL("https://github.com/mahimhasan9130-beep");

            if (mouseClicked(advisor))
                OpenURL("https://www.junaedyounuskhan.com/");

            if (mouseClicked(raylib))
                OpenURL("https://www.raylib.com/");

            if (mouseClicked(source))
                OpenURL("https://opengameart.org/");

            if (IsKeyPressed(KEY_ESCAPE) ||
                    IsKeyPressed(KEY_M) ||
                    mouseClicked(back))
            {
                PlaySound(selectSound);
                screen = previousScreen;
            }

            continue;
        }

        if (screen == SCREEN_GAME)
        {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M))
            {
                PlaySound(selectSound);
                previousScreen = SCREEN_GAME;
                screen = SCREEN_SAVE;
                continue;
            }
            if (IsKeyPressed(KEY_R))
            {
                PlaySound(selectSound);
                int savedHearts    = player.hearts;
                int baseScore  = player.score - player.levelScore;   // strip current level earnings
                int savedScore = (baseScore * 85) / 100;   // penalise level score
                char savedName[MAX_NAME_LENGTH];
                strcpy(savedName, player.name);
                if (savedScore < 0) savedScore = 0;
                resetLevel(levelNum, &player, enemies, &enemyCount);
                enemyMoveInterval = getEnemyInterval(levelNum);
                player.hearts     = savedHearts;
                player.score      = savedScore;
                player.levelScore = 0;
                strcpy(player.name, savedName);
                totalcount = countGoals();
                enemyTimer = 0.0f;
            }
            int moveX = 0;
            int moveY = 0;
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) moveX = 1;
            else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) moveX = -1;
            else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) moveY = -1;
            else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))  moveY = 1;
            player.lastX = player.x;   // ← add
            player.lastY = player.y;   // ← add
            if ((moveX != 0 || moveY != 0) && gamewon == 0)
            {
                int nextX = player.x + moveX;
                int nextY = player.y + moveY;
                if (nextX >= 0 && nextX < BOARD_SIZE && nextY >= 0 && nextY < BOARD_SIZE)
                {
                    if (board[nextY][nextX] == '$')
                    {
                        int boxNextX = nextX + moveX;
                        int boxNextY = nextY + moveY;
                        if (boxNextX >= 0 && boxNextX < BOARD_SIZE && boxNextY >= 0 && boxNextY < BOARD_SIZE)
                        {
                            if (board[boxNextY][boxNextX] != '#' && board[boxNextY][boxNextX] != '$')
                            {
                                int landedOnGoal =(goals[boxNextY][boxNextX] == '.');
                                board[boxNextY][boxNextX] = '$';
                                if (goals[nextY][nextX] == '.')
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
                                applyMoveScore(&player, landedOnGoal);
                                if (landedOnGoal)
                                {
                                    PlaySound(goalSound);
                                }
                                else
                                {
                                    PlaySound(moveSound);
                                }
                            }
                        }
                    }
                    else if (board[nextY][nextX] != '#')
                    {
                        player.x = nextX;
                        player.y = nextY;
                        player.moves++;
                        applyMoveScore(&player, 0);
                        PlaySound(moveSound);
                    }
                }
            }
            int playerHit = 0;
            if (gamewon == 0 && checkEnemyCollision(player, enemies, enemyCount))
            {
                handleEnemyHit(levelNum, &player, enemies, &enemyCount, &totalcount, &gamewon, &enemyTimer, hitSound);
                playerHit = 1;
            }
            if (gamewon == 2)
            {
                recordScoreOnce(&player, &scoreRecorded);
                PlaySound(gameOverSound);
                screen = SCREEN_GAMEOVER;
                continue;
            }
            enemyTimer += GetFrameTime();
            if (enemyTimer >= enemyMoveInterval && gamewon == 0)
            {
                updateEnemies(enemies, enemyCount,&player,levelNum);
                enemyTimer = 0.0f;
                if (!playerHit && checkEnemyCollision(player, enemies, enemyCount))
                {
                    handleEnemyHit(levelNum, &player,enemies, &enemyCount, &totalcount, &gamewon, &enemyTimer, hitSound);
                }
                if (gamewon == 2)
                {
                    recordScoreOnce(&player, &scoreRecorded);
                    PlaySound(gameOverSound);
                    screen = SCREEN_GAMEOVER;
                    continue;
                }
            }
            goalCount = 0;

            for (int y = 0; y < BOARD_SIZE; y++)
            {
                for (int x = 0; x < BOARD_SIZE; x++)
                {
                    if (goals[y][x] == '.' && board[y][x] == '$') goalCount++;
                }
            }
            if (gamewon == 0 && goalCount == totalcount)
            {
                gamewon = 1;
                PlaySound(winSound);
            }
            if (gamewon == 1 && IsKeyPressed(KEY_ENTER))
            {
                levelNum++;
                if (levelNum > 10)
                {
                    levelNum = 10;
                    if (!scoreRecorded)
                    {
                        saveLeaderboardScore(player.name, player.score);
                        scoreRecorded = 1;
                    }
                    screen = SCREEN_WIN;
                }
                else
                {
                    resetLevel(levelNum, &player, enemies, &enemyCount);
                    enemyMoveInterval = getEnemyInterval(levelNum);
                    player.levelScore = 0;
                    totalcount = countGoals();
                    enemyTimer = 0.0f;
                    gamewon = 0;
                }
            }
            if (gamewon == 2)
            {
                PlaySound(gameOverSound);
                screen = SCREEN_GAMEOVER;
                continue;
            }
            BeginDrawing();
            ClearBackground(BLACK);

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

                    if (board[y][x] == '#')
                    {
                        DrawTexture(wallTexture, rect.x, rect.y, WHITE);
                    }
                    else if (board[y][x] == '.')
                    {
                        DrawTexture(goalTexture,
                                    rect.x,
                                    rect.y,
                                    WHITE);
                    }
                    else if (board[y][x] == '$')
                    {
                        if (goals[y][x] == '.')
                        {
                            DrawTexture(boxOnGoalTexture,
                                        rect.x,
                                        rect.y,
                                        WHITE);
                        }
                        else
                        {
                            DrawTexture(boxTexture, rect.x, rect.y, WHITE);
                        }
                    }
                    else
                    {
                        DrawTexture(floorTexture, rect.x, rect.y,  WHITE);
                    }
                    DrawRectangleLinesEx(rect, 1, DARKGRAY);
                }
            }
            for (int i = 0; i < enemyCount; i++)
            {
                if (enemies[i].active)
                {
                    DrawTexture(enemyTexture, boardoffsetX + enemies->x * 32, boardoffsetY + enemies->y* 32, WHITE );
                }
            }
            DrawTexture( playerTexture, boardoffsetX + player.x * 32, boardoffsetY + player.y * 32, WHITE );
            int hudX = 15;
            int hudY = 15;
            int boxW = 180;
            int boxH = 55;
            Color hudBG = Fade(BLACK, 0.75f);
            Color hudBorder = Fade(RAYWHITE, 0.45f);
            Rectangle scoreBox =
            {
                hudX,
                hudY,
                boxW,
                boxH
            };
            DrawRectangleRec(scoreBox, hudBG);
            DrawRectangleLinesEx(scoreBox, 2, hudBorder);
            DrawText( "SCORE", hudX + 12, hudY + 7, 16, GRAY );
            DrawText( TextFormat("%d", player.score), hudX + 12, hudY + 25,  25,  GOLD );
            Rectangle levelBox =
            {
                hudX + boxW + 10,
                hudY,
                120,
                boxH
            };
            DrawRectangleRec(levelBox, hudBG);
            DrawRectangleLinesEx(levelBox, 2, hudBorder);
            DrawText("LEVEL", levelBox.x + 12, levelBox.y + 7, 16, GRAY);
            DrawText(TextFormat("%02d", levelNum), levelBox.x + 12, levelBox.y + 25, 25, BLUE);
            Rectangle heartBox =
            {
                hudX + boxW + 10 + 120 + 10,
                hudY,
                180,
                boxH
            };
            DrawRectangleRec(heartBox, hudBG);
            DrawRectangleLinesEx(heartBox, 2, hudBorder);
            DrawText("HEALTH", heartBox.x + 12, heartBox.y + 7, 16, GRAY);
            for (int i = 0; i < 3; i++)
            {
                Rectangle heartBar =
                {
                    heartBox.x + 12 + i * 52,
                    heartBox.y + 25,
                    42,
                    24
                };
                Color heartColor;
                if (i < player.hearts) heartColor = WHITE;
                else heartColor = Fade(DARKGRAY, 0.5f);
                Rectangle source =
                {
                    0,
                    0,
                    (float)heartTexture.width,
                    (float)heartTexture.height
                };
                DrawTexturePro(
                    heartTexture,
                    source,
                    heartBar,
                    (Vector2)
                {
                    0, 0
                },
                0.0f,
                heartColor
                );
            }
            EndDrawing();
        }
        if (screen == SCREEN_GAMEOVER)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(gameoverTexture, 0, 0, WHITE);
            // Back to Menu button
            Rectangle menuButton =
            {
                260, 720,
                280, 55
            };
            Vector2 mouse = GetMousePosition();
            bool mouseOver = CheckCollisionPointRec(mouse, menuButton);
            DrawRectangleRec(menuButton, GRAY);
            Rectangle innerButton =
            {
                menuButton.x + 5,
                menuButton.y + 5,
                menuButton.width - 10,
                menuButton.height - 10
            };
            if (mouseOver) DrawRectangleRec(innerButton, DARKGRAY);
            else
            {
                DrawRectangleRec(innerButton, BLACK);
            }
            DrawRectangleLinesEx(menuButton, 2, LIGHTGRAY);
            const char *menuText = "BACK TO MENU";
            int textWidth = MeasureText(menuText, 20);
            DrawText( menuText, (int)(menuButton.x + (menuButton.width - textWidth) / 2), (int)(menuButton.y + 17), 20, mouseOver ? LIME : GREEN);
            EndDrawing();
            if (mouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_ENTER))
            {
                screen = SCREEN_MENU;
                PlaySound(selectSound);
            }
            continue;
        }
        if(screen == SCREEN_WIN)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(gamewinTexture,0,0,WHITE);
            Rectangle menuButton =
            {
                260, 720, 280, 55
            };
            Vector2 mouse = GetMousePosition();
            bool mouseOver = CheckCollisionPointRec(mouse, menuButton);
            DrawRectangleRec(menuButton, GRAY);
            Rectangle innerButton =
            {
                menuButton.x + 5,
                menuButton.y + 5,
                menuButton.width - 10,
                menuButton.height - 10
            };
            if (mouseOver) DrawRectangleRec(innerButton, DARKGRAY);
            else
            {
                DrawRectangleRec(innerButton, BLACK);
            }
            DrawRectangleLinesEx(menuButton, 2, LIGHTGRAY);
            const char *menuText = "BACK TO MENU";
            int textWidth = MeasureText(menuText, 20);
            DrawText( menuText, (int)(menuButton.x + (menuButton.width - textWidth) / 2), (int)(menuButton.y + 17), 20, mouseOver ? LIME : GREEN);
            EndDrawing();
            if ((mouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ENTER))
            {
                screen = SCREEN_MENU;
                PlaySound(selectSound);
            }
            continue;
        }
    }
    SaveSettings();
    UnloadMusicStream(menuMusic);
    UnloadSound(moveSound);
    UnloadSound(hitSound);
    UnloadSound(winSound);
    UnloadSound(gameOverSound);
    UnloadSound(goalSound);
    UnloadSound(selectSound);
    UnloadSound(loadgameSound);
    UnloadTexture(floorTexture);
    UnloadTexture(wallTexture);
    UnloadTexture(goalTexture);
    UnloadTexture(boxTexture);
    UnloadTexture(playerTexture);
    UnloadTexture(enemyTexture);
    UnloadTexture(boxOnGoalTexture);
    UnloadTexture(menubgTexture);
    UnloadTexture(instructionbgTexture);
    UnloadTexture(gameoverTexture);
    UnloadTexture(gamewinTexture);
    UnloadTexture(escapeConfirm);
    UnloadTexture(heartTexture);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
