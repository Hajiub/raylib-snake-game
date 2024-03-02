#include <raylib.h>
#include <time.h>
#include <stdlib.h>

/////////////////////////////////////////

#define BOXSIZE 30
#define MAX_LENGTH 100 
#define COLOR_BACKGROUND        (Color){ 15, 15, 15, 255 }
#define COLOR_SNAKE             (Color){ 0, 255, 0, 255 }
#define COLOR_SNAKE_HEAD        (Color){ 0, 200, 0, 255 }
#define COLOR_FOOD              (Color){ 255, 0, 0, 255 }
#define COLOR_BORDER            (Color){ 200, 200, 200, 255 }

/////////////////////////////////////////

typedef struct {
    Vector2 position;
    Vector2 size;
    Color color;
} Box;

typedef struct {
    Box body;
    Vector2 speed;
} Snake;

typedef struct {
    Box body;
} Food;

typedef enum {
    RIGHT = 0,
    LEFT,
    UP,
    DOWN,
    ELEMENTCOUNT
} Direction;

/////////////////////////////////////////
static const int screenWidth  = 900;
static const int screenHeight = 600;
static const int innerWidth   = screenWidth - (BOXSIZE * 2);
static const int innerHeight  = screenHeight -  (BOXSIZE * 2);
static const char *title = "Snake Game";
static Snake snake[MAX_LENGTH] = { 0 };
static Vector2 snakePosition[MAX_LENGTH] = { 0 };
static int current_len = 0;
static int framesCounter = 0;
static Direction direction;
static bool gameOver = false;
static Food food;
static Box box;
Font font;
Vector2 textPos;
////////////////////////////////////////
void InitGame(void);
void UpdateDrawFrame(void);
void UpdateGame(void);
void DrawGame(void);
void UnloadGame(void);
Vector2 SetFoodPosition(void);
int randint(int a, int b);

int main(void)
{
    InitWindow(screenWidth, screenHeight, title);
    InitGame();
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
    UnloadGame();
    CloseWindow();
    return 0;
}

void InitGame(void)
{
   font = LoadFont("./fonts/OpenSans-ExtraBold.ttf");
   textPos = (Vector2){innerWidth / 3, innerHeight / 3};

   box.position = (Vector2){BOXSIZE, BOXSIZE };
   box.size     = (Vector2){innerWidth, innerHeight};
   box.color    = COLOR_BACKGROUND;
   current_len = 3;
   gameOver = false;
   direction = randint(1, ELEMENTCOUNT);
   for (int i = 0; i < MAX_LENGTH; i++) {
       snake[i].body.size = (Vector2){BOXSIZE, BOXSIZE};
       if (i == 0){
           snake[i].body.position = (Vector2) {10 * BOXSIZE, 10 * BOXSIZE};
           snake[i].body.color = COLOR_SNAKE_HEAD;
        }
        else {
            snake[i].body.position = (Vector2) {snake[0].body.position.x + (BOXSIZE * i), snake[0].body.position.y};
            snake[i].body.color = COLOR_SNAKE;
        }
   }
   food.body.position = SetFoodPosition();
   food.body.size = (Vector2){BOXSIZE, BOXSIZE};
   food.body.color = COLOR_FOOD; 

}

void UpdateDrawFrame(void)
{
    // Update
    UpdateGame();
    // Draw
    DrawGame();
}

void UpdateGame(void)
{
    if (!gameOver) {
        // User input Vim user ::::
        if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_L)) && direction != LEFT)
            direction = RIGHT;
        else if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_H)) && direction != RIGHT)
            direction = LEFT;
        else if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_K)) && direction != DOWN) 
            direction = UP;
        else if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_J) )&& direction != UP)
            direction = DOWN; 

        if (direction == RIGHT) {
            snake[0].speed = (Vector2) {BOXSIZE, 0};
        }

        else if (direction == LEFT) {
            snake[0].speed = (Vector2) {-BOXSIZE, 0};
        }

        else if (direction == UP) {
            snake[0].speed = (Vector2) {0, -BOXSIZE};
        }

        else if (direction == DOWN) {
            snake[0].speed = (Vector2) {0, BOXSIZE};
        }
        // Hanlde Snake movement
        if ((++framesCounter % 5) == 0) {
            for (int i = 0; i < current_len; i++) {
                // head
                if (i == 0) {
                    snake[i].body.position.x += snake[i].speed.x;
                    snake[i].body.position.y += snake[i].speed.y;
                }
                else
                    snake[i].body.position = snakePosition[i - 1];
            }

        }
        for (int i = 0; i < current_len; i++)
            snakePosition[i] = snake[i].body.position;
        
        // wall        
        if (((snake[0].body.position.x) > innerWidth) ||
            ((snake[0].body.position.y) > innerHeight) ||
            (snake[0].body.position.x < BOXSIZE) || (snake[0].body.position.y < BOXSIZE))
        {
             gameOver = true;
        }
        // self
        for (int i = 1; i < current_len; i++)
            if ((snake[0].body.position.x  == snake[i].body.position.x) && (snake[0].body.position.y == snake[i].body.position.y))
                gameOver = true;

        if ((snake[0].body.position.x < (food.body.position.x + BOXSIZE)) && ((snake[0].body.position.x + BOXSIZE) > food.body.position.x) && 
            (snake[0].body.position.y < (food.body.position.y + BOXSIZE)) && ((snake[0].body.position.y + BOXSIZE)) > food.body.position.y) {
            snake[current_len].body.position = snakePosition[current_len - 1];
            food.body.position = SetFoodPosition();
            current_len++;
        }
    }

    else {
        box.color = COLOR_BORDER;
        if (IsKeyPressed(KEY_R))
            InitGame();
    }
    
}

void DrawGame(void)
{
    BeginDrawing();
        ClearBackground(WHITE);
//        DrawRectangleLines(int posX, int posY, int width, int height, Color color);
        DrawRectangle(0, 0, screenWidth, screenWidth, COLOR_BORDER);
        DrawRectangleV(box.position, box.size, box.color);
        DrawFPS(5, 5);
        if(!gameOver) {
            for (int i = current_len - 1; i  >= 0 ; i--) {
                DrawRectangleV(snake[i].body.position, snake[i].body.size, snake[i].body.color);
            }
            DrawRectangleV(food.body.position, food.body.size, food.body.color);
        }
        else {
//            DrawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);
            DrawTextEx(font, "Press 'r' to play again!", textPos, 40, 2.3, COLOR_BACKGROUND);
        }
    EndDrawing();
}

void UnloadGame(void)
{
    //void UnloadFont(Font font);
    UnloadFont(font);
}

Vector2 SetFoodPosition(void)
{
    int maxWidthUnits = innerWidth / BOXSIZE; 
    int maxHeightUnits = innerHeight / BOXSIZE;

    Vector2 pos;

    pos.x = ((rand() % maxWidthUnits) * BOXSIZE) + BOXSIZE;
    pos.y = ((rand() % maxHeightUnits) * BOXSIZE) + BOXSIZE;
    for (int i = 0; i < current_len; i++)
        if ((pos.x == snake[i].body.position.x) && (pos.y == snake[i].body.position.y))
           return  SetFoodPosition();
    return pos;
}



static int seed_set = 0;
int randint(int min, int max)
{
    if (!seed_set) {
        srand(time(NULL));
        seed_set = 1;
    }
    return min + rand() / (RAND_MAX / (max - min) + 1);
}
