#include <iostream>
#include <vector>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>

using namespace std;


struct Coords {
	int x, y;

	Coords() {
		x = 0;
		y = 0;
	}

	Coords(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

enum Tile : char {
	UNDEFINED,
	TO_CHECK,
	WALL,
	PATH
};

struct Frame {

private:
	char* characters;

public:
	int width, height;

	Frame(int width, int height) {
		this->width = width;
		this->height = height;

		characters = new char[width * height];

		clear();
	}

	char get(int x, int y) {
		return characters[y * width + x];
	}

	void set(int x, int y, char c) {
		characters[y * width + x] = c;
	}

	void clear() {
		for (int i = 0; i < width * height; i++) {
			characters[i] = ' ';
		}
	}

	void show() {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				cout << characters[y * width + x];
			}
			cout << '\n';
		}
	}
};

struct Labyrinth {

private:
	Tile* walls;

public:
	int width, height;
	Coords start;
	Coords end;

	Labyrinth(int width, int height) {
		this->width = width;
		this->height = height;

		walls = new Tile[width * height];

		for (int i = 0; i < width * height; i++) {
			walls[i] = UNDEFINED;
		}
	}

	Tile get(int x, int y) {
		return walls[y * width + x];
	}

	void set(int x, int y, Tile tile) {
		walls[y * width + x] = tile;
	}

	int neighbouringPathTilesCount(int x, int y) {
		int count = 0;

		count += (x > 0 && get(x - 1, y) == PATH);
		count += (y > 0 && get(x, y - 1) == PATH);
		count += (x + 1 < width && get(x + 1, y) == PATH);
		count += (y + 1 < height && get(x, y + 1) == PATH);

		return count;
	}

	void draw(Frame canvas) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				char c;
				if (y == start.y && x == start.x) c = '$';
				else if (y == end.y && x == end.x) c = '@';
				else if (get(x, y) == PATH) c = ' ';
				else c = '#';

				canvas.set(x, y, c);
			}
		}
	}

};


Labyrinth generateLabyrinth(int seed, int width, int height) {

	Labyrinth labyrinth(width, height);
	vector<Coords> tilesToCheck;

	srand(seed);

	int x = rand() % width;
	int y = rand() % height;

	labyrinth.start = { x, y };

	tilesToCheck.push_back(Coords(x, y));

	do {
		int pos = rand() % tilesToCheck.size();
		Coords selectedTile = tilesToCheck[pos];
		tilesToCheck.erase(tilesToCheck.begin() + pos);

		x = selectedTile.x;
		y = selectedTile.y;

		if (labyrinth.neighbouringPathTilesCount(x, y) <= 1) {

			labyrinth.set(x, y, PATH);

			if (x > 0 && labyrinth.get(x - 1, y) == UNDEFINED) {
				tilesToCheck.push_back(Coords(x - 1, y));
				labyrinth.set(x - 1, y, TO_CHECK);
			}
			if (y > 0 && labyrinth.get(x, y - 1) == UNDEFINED) {
				tilesToCheck.push_back(Coords(x, y - 1));
				labyrinth.set(x, y - 1, TO_CHECK);
			}
			if (x + 1 < width && labyrinth.get(x + 1, y) == UNDEFINED) {
				tilesToCheck.push_back(Coords(x + 1, y));
				labyrinth.set(x + 1, y, TO_CHECK);
			}
			if (y + 1 < height && labyrinth.get(x, y + 1) == UNDEFINED) {
				tilesToCheck.push_back(Coords(x, y + 1));
				labyrinth.set(x, y + 1, TO_CHECK);
			}
		
		} else {
			labyrinth.set(x, y, WALL);
		}

	} while (tilesToCheck.size() != 0);

	labyrinth.set(x, y, PATH);
	labyrinth.end = { x, y };

	return labyrinth;
}

void writeCharacter(int x, int y, char c) {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
	cout << c;
}

void drawNextFrame(Frame previousFrame, Frame nextFrame) {
	for (int y = 0; y < previousFrame.height; y++) {
		for (int x = 0; x < previousFrame.width; x++) {

			char c = nextFrame.get(x, y);

			// Draw only differences between frames to prevent flickering
			if (c != previousFrame.get(x, y)) {
				writeCharacter(x, y, c);
			}
		}
	}
}


int main() {

	Frame previousFrame(20, 10);
	Frame currentFrame(20, 10);

	while (true) {

		Labyrinth labyrinth = generateLabyrinth(time(0), 20, 10);

		Coords player = labyrinth.start;

		do {

			// Prepare next frame
			labyrinth.draw(currentFrame);
			currentFrame.set(player.x, player.y, 'X');

			// Show
			drawNextFrame(previousFrame, currentFrame);

			// ====
			char key = tolower(_getch());
			

			if      ((key == 'w' || key == 'g') && player.y > 0 && labyrinth.get(player.x, player.y - 1) == PATH) player.y--;
			else if ((key == 'a' || key == 'l') && player.x > 0 && labyrinth.get(player.x - 1, player.y) == PATH) player.x--;
			else if ((key == 's' || key == 'd') && player.y + 1 < labyrinth.height && labyrinth.get(player.x, player.y + 1) == PATH) player.y++;
			else if ((key == 'd' || key == 'p') && player.x + 1 < labyrinth.width && labyrinth.get(player.x + 1, player.y) == PATH) player.x++;

			swap(previousFrame, currentFrame);

		} while (player.x != labyrinth.end.x || player.y != labyrinth.end.y);

	}

	return 0;
}