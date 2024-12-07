#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

struct Coords {
	int x, y;

	Coords(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

struct Labyrinth {
	bool* walls;
	int height, width;

	Labyrinth(int width, int height) {
		this->width = width;
		this->height = height;

		walls = new bool[width * height];

		for (int i = 0; i < width * height; i++) {
			walls[i] = true;
		}
	}

	bool isWall(int x, int y) {
		return walls[y * width + x];
	}

	void set(int x, int y, bool state) {
		walls[y * width + x] = state;
	}

	bool canBeCarved(int x, int y) {
		// not implemented
		return true;
	}
};


Labyrinth generateLabyrinth(int width, int height) {

	Labyrinth labyrinth(width, height);

	vector<Coords> nextToCheck;

	int x = rand() % width;
	int y = rand() % height;

	labyrinth.set(x, y, false);

	if (x - 1 >= 0) nextToCheck.push_back(Coords(x - 1, y));
	if (y - 1 >= 0) nextToCheck.push_back(Coords(x, y - 1));
	if (x + 1 < width) nextToCheck.push_back(Coords(x + 1, y));
	if (y + 1 < height) nextToCheck.push_back(Coords(x, y + 1));


	return labyrinth;
}

void drawLabyrinth(Labyrinth labyrinth) {
	for (int y = 0; y < labyrinth.height; y++) {
		for (int x = 0; x < labyrinth.width; x++) {
			if (labyrinth.isWall(x, y)) {
				cout << '#';
			} else {
				cout << ' ';
			}
		}
		cout << endl;
	}
}


int main() {

	srand(time(0));

	Labyrinth labyrinth = generateLabyrinth(20, 10);

	drawLabyrinth(labyrinth);

	return 0;
}