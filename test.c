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
#define FONT_SIZE 48

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
	char* value;
	char* id;
	bool blocked;
    bool hovered;
    bool active;
    Rectangle bounds;
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
	for (int y = 0; y < CELL_COUNT; ++y)
	{
        int cy = b.startY + (y * CELL_DIM);
		for (int x = 0; x < CELL_COUNT; ++x)
		{
            int cx = b.startX + (x * CELL_DIM);
			Vector2 pos = { cx, cy };
            Rectangle r = { cx, cy, CELL_DIM, CELL_DIM };
			Cell c = 
            {
                .pos = pos, 
                .value = "A", 
                .id = "", 
                .blocked = false,
                .hovered = false,
                .bounds = r
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

    int activeCell = -1;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "heyo");

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
                    if (activeCell < 0)
                    {
                        activeCell = i;
                    }
                    else if (activeCell != (int)i)
                    {
                        cells.items[activeCell].active = false;
                        activeCell = i;
                    }
                }
            }
        }

        char theChar = (char)malloc(sizeof(char)+1);
        int charPressed = GetCharPressed();
        if (charPressed > 0)
        {
            theChar = (char)charPressed;
        }
        if (activeCell > -1)
        {
            if (charPressed > 0)
            {
                printf("%c\n", theChar);
                if (charPressed >= 32 && charPressed <= 125)
                    strcpy(cells.items[activeCell].value, &theChar);
                    //cells.items[activeCell].value = &theChar;
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
            DrawText(c.value, c.pos.x + CELL_DIM/2 - w/2, c.pos.y + CELL_DIM/2 - FONT_SIZE/2, FONT_SIZE, BLACK);

            if (!c.blocked)
            {
                DrawRectangleLines(c.pos.x, c.pos.y, CELL_DIM*0.25, CELL_DIM*0.25, BLACK);
            }

		}

		DrawBoard(b);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
