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

int h, w;
struct Location grid[10][10];

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

void drawTile(int y, int x, struct Tile t){
	if(t.type){
		drawHex(y,x);
	}
	mvaddstr(y+2,x+3,typeToCode(t.type);
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
			return "MOS"
		case 7:
			return "LDB";
		case 8:
			return "PLB";
		default:
			return "___";
	}
}


void drawRow(int r){
	for(int i = 0; i < 10; i++){
		Tile& t = Location[r][i]
	}
}

int main(){
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	getmaxyx(stdscr, h, w );

	
	getch();
	return 0;
}
