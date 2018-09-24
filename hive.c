#include <curses.h>
#define HEXW 7
#define HEXH 2

//Codes for pieces
#define QUEEN 1
#define SPIDER 2
#define BEETLE 3
#define GRASSHOPPER 4
#define SOLDIER_ANT 5
#define MOSQUITO 6
#define LADYBUG 7
#define PILLBUG 8

struct Tile {
	int owner;
	char type;
};

struct Location {
	struct Tile tiles[10];
	int numTiles;
};

struct Coordinate {
	int y, x;
};

struct Player {
	int remaining[9];
}

int h, w;
struct Location grid[10][10];
struct Player players[2];

struct Location * coordinateToLocation(struct Coordinate c){
	return &(grid[c.y][c.x]);
}

void drawHex(int y, int x){
	mvaddstr(y++,x+2,"_____");
	mvaddch(y,x+1,'/');
	mvaddch(y++,x+7,'\\');
	mvaddch(y,x,'/');
	mvaddch(y++,x+8,'\\');
	mvaddch(y,x,'\\');
	mvaddch(y++,x+8,'/');
	mvaddstr(y,x+1,"\\_____/");
}	

const struct Coordinate * getAdjacent(y, x)
{
	static struct Coordinate adj[6];
	bool even = (x % 2) == 0;
	adj[0] = (struct Coordinate){y - 1, x};
	adj[3] = (struct Coordinate){y + 1, x};
	if(even){
		adj[1] = (struct Coordinate){y - 1, x + 1};
		adj[2] = (struct Coordinate){y, x + 1};
		adj[4] = (struct Coordinate){y, x - 1};
		adj[5] = (struct Coordinate){y - 1, x - 1};
	}
	else{
		adj[1] = (struct Coordinate){y, x + 1};
		adj[2] = (struct Coordinate){y + 1, x + 1};
		adj[4] = (struct Coordinate){y + 1, x - 1};
		adj[5] = (struct Coordinate){y, x - 1};
	}
	return adj;
}

const char * typeToCode(char type)
{
	switch(type) {
		case 1:
			return "QUN";
		case 2:
			return "SPI";
		case 3:
			return "BET";
		case 4:
			return "GRH";
		case 5:
			return "SDA";
		case 6:
			return "MOS";
		case 7:
			return "LDB";
		case 8:
			return "PLB";
		default:
			return " _ ";
	}
}

void drawTile(int y, int x, const struct Tile * t){
	if(t->type != 0){
		drawHex(y,x);
	}
	mvaddstr(y+2,x+3,typeToCode(t->type));
}


struct Tile * getTopTile(struct Location * loc){
	if(loc->numTiles == 0){
		return &(loc->tiles[0]);
	}
	int index = loc->numTiles - 1;
	return &((*loc).tiles[index]);
}

void drawRow(int y, int x, int r){
	for(int i = 0; i < 10; i++){
		struct Tile * tile = getTopTile(&grid[r][i]);
		if(i % 2)
			drawTile(y + HEXH,x,tile);
		else
			drawTile(y,x,tile);
		x+=HEXW;
	}
}

void drawGrid(){
	for(int i = 0; i < 6; i++)
		drawRow(i * 4, 0, i);
}

void addTile(struct Location * loc, int owner, char type){
	struct Tile * ct = &(loc->tiles[loc->numTiles]);
	ct->owner = owner;
	ct->type = type;
	loc->numTiles++;
}

void initPlayers(){
	int * arr = players[1].remaining;
	arr[QUEEN] = 1;
	arr[SPIDER] = 2;
	arr[BEETLE] = 2;
	arr[GRASSHOPPER] = 3;
	arr[SOLDIER_ANT] = 4;
	arr[MOSQUITO] = 1;
	arr[LADYBUG] = 1;
	arr[PILLBUG] = 1;
	players[2] = players[1];
}

void drawRemaining(int player){
	
}

int main(){
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	getmaxyx(stdscr, h, w );
	curs_set(0);
	int x = 0, y = 0;
	for(int row = 0; row < 4; row++){
		for(int num = 0; num < 6; num++){
			if(num % 2)
				drawHex(y+HEXH,x);
			else
				drawHex(y,x);
			x+=HEXW;
		}
		x = 0;
		y+= 2 * HEXH;
	}
	/*
	for(int i = 1; i < 7; i++){
		struct Location * loc = &grid[3][i+1];
		addTile(loc,0,i);
	}
	drawGrid();
	*/
	getch();
	endwin();
	return 0;
}
