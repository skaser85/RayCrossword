#define NOB_IMPLEMENTATION
#include "nob.h"
#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 960
#define BOARD_WIDTH 600
#define CELL_COUNT 5
#define CELL_DIM (BOARD_WIDTH / CELL_COUNT)
#define CELL_COLOR_NORMAL RAYWHITE
#define CELL_COLOR_HOVERED PINK
#define CELL_COLOR_ACTIVE YELLOW
#define FONT_SIZE 80

const char* lemonMilkFP = "./fonts/LEMONMILK-Regular.otf";
const char* orbitronSBFP = "./fonts/Orbitron/Orbitron-SemiBold.ttf";

typedef struct
{
    int row;
    int col;
} RowCol;

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
	bool blocked;
    bool hovered;
    bool active;
    Rectangle bounds;
    RowCol rowCol;
} Cell;

typedef struct
{
	Cell *items;
	size_t count;
	size_t capacity;
} Cells;

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
                .num = "0",
                .blocked = false,
                .hovered = false,
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
    Font orbitronSB = LoadFontEx(orbitronSBFP, FONT_SIZE, NULL, 0);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		
		ClearBackground(LIGHTGRAY);
		
		Vector2 m = GetMousePosition();
        bool mouseReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);

        for (size_t i = 0; i < cells.count; i++)
        {
            cells.items[i].hovered = !cells.items[i].blocked && CheckCollisionPointRec(m, cells.items[i].bounds);
            if (mouseReleased && cells.items[i].hovered)
            {
                cells.items[i].active = !cells.items[i].active;
                if (cells.items[i].active)
                {
                    if (activeCell <= 0)
                    {
                        activeCell = &cells.items[i];
                    }
                    else if (activeCell != &cells.items[i])
                    {
                        activeCell->active = false;
                        activeCell = &cells.items[i];
                    }
                    if (activeCell > 0)
                        activeCell->active = true;
                }
            }
        }

        int charPressed = GetCharPressed();
        char theChar = (char)charPressed;
        if (activeCell > 0)
        {
            if (charPressed > 0)
            {
                if (charPressed >= 32 && charPressed <= 125)
                {
                    activeCell->value = (char*)malloc(sizeof(char*));
                    strcpy(activeCell->value, TextToUpper(&theChar));
                }
            }
            
            if(IsKeyPressed(KEY_UP))
            {
                if(activeCell->rowCol.row = 0)
                {
                    
                }
                else
                {
                    activeCell = &cells.items[activeCell->id + CELL_COUNT]; 
                }
            }
            else if(IsKeyPressed(KEY_DOWN))
            {

            }
            else if(IsKeyPressed(KEY_LEFT))
            {

            }
            else if(IsKeyPressed(KEY_RIGHT))
            {

            }
        }

		for (size_t i = 0; i < cells.count; i++)
		{
			Cell c = cells.items[i];
            Color color = c.hovered ? CELL_COLOR_HOVERED : CELL_COLOR_NORMAL;
			if (c.active)
                color = CELL_COLOR_ACTIVE;
            DrawRectangle(c.pos.x, c.pos.y, CELL_DIM, CELL_DIM, color);
            
            int w = MeasureText(c.value, FONT_SIZE);
            Vector2 pos = { .x = c.pos.x + CELL_DIM/2 - w/2, .y = c.pos.y + CELL_DIM/2 - FONT_SIZE/2 };
            DrawTextEx(orbitronSB, c.value, pos, FONT_SIZE, 2, BLACK);
            
            if (!c.blocked)
            {
                Rectangle r = { .x = c.pos.x, .y = c.pos.y, .width = CELL_DIM*0.25, .height = CELL_DIM*0.25 };
                DrawRectangleLinesEx(r, 1, BLACK);
                w = MeasureText(c.num, FONT_SIZE/2);
                pos = (Vector2){ .x = c.pos.x + (CELL_DIM*0.25)/2 - w/2, y: c.pos.y + (CELL_DIM*0.25)/2 - FONT_SIZE/4 };
                DrawTextEx(orbitronSB, c.num, pos, FONT_SIZE/2, 2, BLACK);
            }

		}

		DrawBoard(b);

		EndDrawing();
	}
    
    UnloadFont(lemonMilk);
    UnloadFont(orbitronSB);
	CloseWindow();

	return 0;
}
