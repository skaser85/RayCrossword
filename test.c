#include "nob.h"
#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 960
#define BOARD_WIDTH 600
#define CELL_COUNT 5
#define CELL_DIM (BOARD_WIDTH / CELL_COUNT)

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
	Color color;
	char* value;
	char* id;
	bool blocked;
} Cell;

typedef struct
{
	Cell *items;
	size_t count;
	size_t capacity;
} Cells;

void DrawBoard(Board b)
{
	for (int y = 0; y < CELL_COUNT + 1; y++)
	{
		Vector2 p1 = { b.bounds.x, b.bounds.y + (y * CELL_DIM) };
		Vector2 p2 = { b.bounds.x + b.w, b.bounds.y + (y * CELL_DIM) };
		DrawLineEx(p1, p2, 3, b.lineColor);
		for (int x = 0; x < CELL_COUNT + 1; x++)
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

void InitCells(Cells *cells)
{
	int cellCount = 0;

	for (int y = 0; y < CELL_COUNT + 1; y++)
	{
		for (int x = 0; x < CELL_COUNT + 1; x++)
		{
			Vector2 pos = { x, y };
			Cell c = 
            {
                .pos = pos, 
                .color = WHITE, 
                .value = "", 
                .id = "", 
                .blocked = false
            };
			cells->items[cellCount] = c;
			cellCount++;
		}
	}
}

int main(void)
{
	Board b = InitBoard(BOARD_WIDTH, BOARD_WIDTH, 50, 20, BLACK, 3);

	Cells cells = {0};
    InitCells(&cells); 

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "heyo");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		
		ClearBackground(LIGHTGRAY);
		
		Vector2 m = GetMousePosition();

		if (CheckCollisionPointRec(m, b.bounds))
		{
			Rectangle r = { m.x, m.y, 100, 100 };
			DrawRectangleRec(r, YELLOW);
		}

		DrawBoard(b);

		for (size_t i = 0; i < cells.count; i++)
		{
			Cell c = cells.items[i];
			DrawRectangle(c.pos.x, c.pos.y, CELL_DIM, CELL_DIM, c.color);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
