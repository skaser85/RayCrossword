#define ARENA_IMPLEMENTATION
#include "./libs/arena.h"

#define NOB_IMPLEMENTATION
#include "./libs/nob.h"

#define SV_IMPLEMENTATION
#include "./libs/sv.h"

#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 960
#define BOARD_WIDTH 600
#define CELL_COUNT 5
#define CELL_DIM (BOARD_WIDTH / CELL_COUNT)
#define FONT_SIZE 80
#define NUM_FONT_SIZE (FONT_SIZE/4)

const char* lemonMilkFP = "./fonts/LEMONMILK-Regular.otf";
const char* orbitronSBFP = "./fonts/Orbitron/Orbitron-SemiBold.ttf";

typedef struct
{
    int row;
    int col;
} RowCol;

typedef enum 
{
    CELL_NORMAL,
    CELL_HOVERED,
    CELL_ACTIVE,
    CELL_BLOCKED
} CELL_STATE;

typedef struct
{
	int w;
	int h;
	int startX;
	int startY;
	Color lineColor;
	int lineThickness;
	Rectangle bounds;
} Board;

typedef struct
{
	Vector2 pos;
    int id;
	char* value;
    char* num;
    CELL_STATE cellState;
    Rectangle bounds;
    RowCol rowCol;
} Cell;

typedef struct
{
	Cell *items;
	size_t count;
	size_t capacity;
} Cells;

Color GetCellColor(CELL_STATE cs)
{
    switch (cs)
    {
        case CELL_BLOCKED: return BLACK;
        case CELL_NORMAL: return RAYWHITE;
        case CELL_HOVERED: return PINK;
        case CELL_ACTIVE: return YELLOW;
    }
}

void DrawBoard(Board b)
{
	for (int y = 0; y < CELL_COUNT + 1; ++y)
	{
		Vector2 p1 = { b.bounds.x, b.bounds.y + (y * CELL_DIM) };
		Vector2 p2 = { b.bounds.x + b.w, b.bounds.y + (y * CELL_DIM) };
		DrawLineEx(p1, p2, 3, b.lineColor);
		for (int x = 0; x < CELL_COUNT + 1; ++x)
		{
			Vector2 p1 = { b.bounds.x + (x * CELL_DIM), b.bounds.y };
			Vector2 p2 = { b.bounds.x + (x * CELL_DIM), b.bounds.y + b.h };
			DrawLineEx(p1, p2, 3, b.lineColor);
		}
	}
}

Board InitBoard(int w, int h, int startX, int startY, Color lineColor, int lineThickness)
{
	Rectangle bounds = { startX, startY, w, h };
	Board b = { w, h, startX, startY, lineColor, lineThickness, bounds };
	return b;
}

void InitCells(Cells *cells, Board b)
{
    int cellCount = 0;
	for (int y = 0; y < CELL_COUNT; ++y)
	{
        int cy = b.startY + (y * CELL_DIM);
		for (int x = 0; x < CELL_COUNT; ++x)
		{
            int cx = b.startX + (x * CELL_DIM);
			Cell c = 
            {
                .pos = { cx, cy },
                .id = cellCount++, 
                .value = "A",
                .num = "25",
                .cellState = CELL_NORMAL,
                .bounds = { cx, cy, CELL_DIM, CELL_DIM },
                .rowCol = (RowCol){ y, x }
            };
            nob_da_append(cells, c);
		}
	}
}

int main(void)
{
	Board b = InitBoard(BOARD_WIDTH, BOARD_WIDTH, 50, 20, BLACK, 3);

	Cells cells = {0};
    InitCells(&cells, b); 

    Cell* activeCell = 0;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ray Crossword");

    Font lemonMilk = LoadFontEx(lemonMilkFP, FONT_SIZE, NULL, 0);
    Font orbitronSBFullSize = LoadFontEx(orbitronSBFP, FONT_SIZE, NULL, 0);
    Font orbitronSBNumSize = LoadFontEx(orbitronSBFP, NUM_FONT_SIZE, NULL, 0);
	
    while (!WindowShouldClose())
	{
		BeginDrawing();
		
		ClearBackground(LIGHTGRAY);
		
		Vector2 m = GetMousePosition();
        bool leftMouseReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
        bool rightMouseReleased = IsMouseButtonReleased(MOUSE_BUTTON_RIGHT);        

        for (size_t i = 0; i < cells.count; i++)
        {   
            Cell* c = &cells.items[i];
            if(CheckCollisionPointRec(m, c->bounds))
            {
                if (c->cellState== CELL_BLOCKED)
                {
                    if (rightMouseReleased)
                        c->cellState = CELL_HOVERED;
                }
                else
                {
                    if (rightMouseReleased)
                    {
                        c->cellState = CELL_BLOCKED;
                        if (activeCell == c)
                            activeCell = NULL;
                    }
                    else if (leftMouseReleased)
                    {
                        if (c->cellState == CELL_ACTIVE)
                        {
                            c->cellState = CELL_HOVERED;
                            activeCell = NULL;
                        }
                        else
                        {
                            c->cellState = CELL_ACTIVE;
                            activeCell = c;
                        }
                    }
                    else
                    {
                        if (c->cellState != CELL_ACTIVE)
                            c->cellState = CELL_HOVERED;
                    }
                }
            }
            else if (c->cellState != CELL_BLOCKED)
            {
                if (c->cellState != CELL_ACTIVE)
                    c->cellState = CELL_NORMAL;
            }
        }

        int charPressed = GetCharPressed();
        char theChar = (char)charPressed;
        if (activeCell != NULL)
        {
            if (charPressed > 0)
            {
                if (charPressed >= 32 && charPressed <= 125)
                {
                    activeCell->value = (char*)malloc(sizeof(char*));
                    strcpy(activeCell->value, TextToUpper(&theChar));
                }
            }
            
            int id = activeCell->id;
            if(IsKeyPressed(KEY_UP))
            {
                id -= CELL_COUNT;
                if(id < 0)
                    id += CELL_COUNT*CELL_COUNT;
            }
            else if(IsKeyPressed(KEY_DOWN))
            {
                id += CELL_COUNT;
                if(id > CELL_COUNT*CELL_COUNT-1)
                    id -= CELL_COUNT*CELL_COUNT;    
            }
            else if(IsKeyPressed(KEY_LEFT))
            {
                id -= 1;
                if (id < 0)
                    id = CELL_COUNT*CELL_COUNT-1;
                
            }
            else if(IsKeyPressed(KEY_RIGHT))
            {
                id += 1;
                if(id == CELL_COUNT*CELL_COUNT)
                    id = 0;
            }
            if(id != activeCell->id)
            {
                activeCell->cellState = CELL_NORMAL;
                activeCell = &cells.items[id];
                activeCell->cellState = CELL_ACTIVE;
            }
            
        }

		for (size_t i = 0; i < cells.count; i++)
		{
			Cell c = cells.items[i];
            DrawRectangle(c.pos.x, c.pos.y, CELL_DIM, CELL_DIM, GetCellColor(c.cellState));
            
            if (c.cellState != CELL_BLOCKED)
            {
                int w = MeasureText(c.value, FONT_SIZE);
                Vector2 pos = { .x = c.pos.x + CELL_DIM/2 - w/2, .y = c.pos.y + CELL_DIM/2 - FONT_SIZE/2 };
                DrawTextEx(orbitronSBFullSize, c.value, pos, FONT_SIZE, 2, BLACK);
            
                Rectangle r = { .x = c.pos.x, .y = c.pos.y, .width = CELL_DIM*0.25, .height = CELL_DIM*0.25 };
                DrawRectangleLinesEx(r, 1, BLACK);
                w = MeasureText(c.num, NUM_FONT_SIZE);
                pos = (Vector2){ .x = c.pos.x + r.width/2 - w/1.5, y: c.pos.y + r.height/2 - NUM_FONT_SIZE/2 };
                DrawTextEx(orbitronSBNumSize, c.num, pos, NUM_FONT_SIZE, 2, BLACK);
            }
            

		}

		DrawBoard(b);

		EndDrawing();
	}
    
    UnloadFont(lemonMilk);
    UnloadFont(orbitronSBFullSize); 
    UnloadFont(orbitronSBNumSize);
	CloseWindow();

	return 0;
}
