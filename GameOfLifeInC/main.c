#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#define ROUNDS 500
#define LENGTH 100
#define FULLLENGTH (LENGTH * LENGTH)

struct cell {
	int alive;
	int nextTurn;
	int* neighbours[8];
};

void setNeighbours(struct cell* cells, int i, int* nothing) {
	for (size_t y = 0; y < 8; y++)
	{
		cells[i].neighbours[y] = nothing;
	}

	// Top
	if (i - (LENGTH) > 0)
		cells[i].neighbours[1] = &cells[i - (LENGTH)].alive;

	// Bottom
	if (i + (LENGTH) < FULLLENGTH)
		cells[i].neighbours[6] = &cells[i + (LENGTH)].alive;

	// Left
	if (i % LENGTH != 0) {
		// Top Left
		if (i - (LENGTH + 1) >= 0)
			cells[i].neighbours[0] = &cells[i - (LENGTH + 1)].alive;

		// Left
		if (i - 1 >= 0)
			cells[i].neighbours[3] = &cells[i - 1].alive;

		// Bottom Left
		if (i + (LENGTH - 1) >= 0 && i + (LENGTH - 1) < FULLLENGTH)
			cells[i].neighbours[3] = &cells[i + (LENGTH - 1)].alive;
	}

	// Right
	if ((i + 1) % LENGTH != 0) {
		// Top Right
		if (i - (LENGTH - 1) >= 0)
			cells[i].neighbours[2] = &cells[i - (LENGTH - 1)].alive;

		// Right
		if (i + 1 < FULLLENGTH)
			cells[i].neighbours[4] = &cells[i + 1].alive;

		// Bottom Right
		if (i + (LENGTH + 1) < FULLLENGTH)
			cells[i].neighbours[7] = &cells[i + (LENGTH + 1)].alive;
	}
}

int aliveNeighbours(struct cell* cell) {
	int num = 0;
	for (size_t i = 0; i < 8; i++)
	{
		int x = *cell->neighbours[i];
		if (x != -1)
			num += x;
	}
	return num;
}

void calculateNext(struct cell* cell) {
	int neighCount = aliveNeighbours(cell);

	if (neighCount < 2) {
		cell->nextTurn = 0;
		return;
	}

	if ((cell->alive == 1 && neighCount == 2) || neighCount == 3) {
		cell->nextTurn = 1;
		return;
	}

	if (neighCount > 3) {
		cell->nextTurn = 0;
	}
}

void update(struct cell* cells) {
	for (size_t i = 0; i < FULLLENGTH; i++)
	{
		cells[i].alive = cells[i].nextTurn;
	}
}

int main() {

	struct cell* cells = (struct cell*)calloc(FULLLENGTH, sizeof(struct cell));

	if (cells == NULL)
		return -1;

	int nothing = -1;

	srand(time(NULL));

	for (size_t i = 0; i < FULLLENGTH; i++)
	{
		cells[i].alive = rand() % 2;

		//cells[i].alive = i;

		setNeighbours(cells, i, &nothing);
	}

	char screen[FULLLENGTH + LENGTH + 1];

	for (size_t k = 0; k < ROUNDS; k++) {
		int screenIndex = 0;
		
		for (size_t i = 0; i < FULLLENGTH; i++) {
			if (i % LENGTH == 0){
				screen[screenIndex] = '\n';
				screenIndex++;
			}
			screen[screenIndex] = cells[i].alive + '0';
			screenIndex++;

			calculateNext(&cells[i]);
		}
		screen[FULLLENGTH + LENGTH] = '\0';

		system("cls");
		//printf("-----");
		printf("%s\n", screen);

		//Sleep(50);

		update(cells);
	}

	free(cells);

	return 0;
}
