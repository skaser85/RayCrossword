#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 960
#define BOARD_WIDTH 600
#define BOARD_START_X 50
#define BOARD_START_Y 20
#define LINE_COLOR BLACK
#define CELL_COUNT 5
#define CELL_DIM (BOARD_WIDTH / CELL_COUNT)

typedef struct
{
	Vector2 pos;
	Color color;
	char* value;
} Cell;

void DrawBoard(Rectangle b)
{
	for (int y = 0; y < CELL_COUNT + 1; y++)
	{
		Vector2 p1 = { b.x, b.y + (y * CELL_DIM) };
		Vector2 p2 = { b.x + b.width, b.y + (y * CELL_DIM) };
		DrawLineEx(p1, p2, 3, LINE_COLOR);
		for (int x = 0; x < CELL_COUNT + 1; x++)
		{
			Vector2 p1 = { b.x + (x * CELL_DIM), b.y };
			Vector2 p2 = { b.x + (x * CELL_DIM), b.y + b.height };
			DrawLineEx(p1, p2, 3, LINE_COLOR);
		}
	}
}

int main(void)
{

	Cell cells[CELL_COUNT * CELL_COUNT] = {0};

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "heyo");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		
		ClearBackground(RAYWHITE);
		
		Rectangle board = { BOARD_START_X, BOARD_START_Y, BOARD_WIDTH, BOARD_WIDTH };
		DrawBoard(board);

		Vector2 m = GetMousePosition();

		if (CheckCollisionPointRec(m, board))
		{
			Rectangle r = { m.x, m.y, 100, 100 };
			DrawRectangleRec(r, YELLOW);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
