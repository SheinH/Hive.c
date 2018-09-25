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
#define GRID_MAX_Y 6
#define GRID_MAX_X 10
#define MAX_TILE_HEIGHT 10

struct Tile {
	int owner;
	char type;
};

struct Location {
	struct Tile tiles[MAX_TILE_HEIGHT];
	int numTiles;
};

struct Coordinate {
	int y, x;
};

struct Player {
	int remaining[9];
};

int h, w;
struct Location grid[GRID_MAX_Y][GRID_MAX_X];
struct Player players[2];
int startCx;
int startCy;
struct Coordinate hlLocation;
WINDOW * piecesUI;

bool validCoordinate(struct Coordinate c){
	return c.y >= 0 && c.y < GRID_MAX_Y && c.x >= 0 && c.x < GRID_MAX_X;
}
struct Location * coordinateToLocation(struct Coordinate c){
	return &(grid[c.y][c.x]);
}

void coordinateToScreenLocation(struct Coordinate c, int * ySC, int * xSC){
	int xOff = c.x - startCx;
	int yOff = c.y - startCy;
	*xSC = xOff * HEXW;
	*ySC = yOff * HEXH * 2;
	if(c.x % 2 == 1)
		*ySC += HEXH;
}


void drawHex(int y, int x){
	mvaddstr(y++,x+2,"_____");
	mvaddstr(y++,x+1,"/     \\");
	mvaddstr(y++,x, "/       \\");
	mvaddstr(y++,x,"\\       /");
	mvaddstr(y,x+1,"\\_____/");
}
void hlHex(int ySC,int xSC){
	int att = A_STANDOUT;
	mvchgat(ySC + 1,xSC + 1,7,att,0, NULL);
	mvchgat(ySC + 2,xSC,9,att,0, NULL);
	mvchgat(ySC + 3,xSC,9,att,0, NULL);
	mvchgat(ySC + 4,xSC + 1,7,att,0, NULL);
}
void chgatHex(int ySC, int xSC, int att){	
	mvchgat(ySC,xSC + 2,5,att,0, NULL);
	mvchgat(ySC + 1,xSC + 1,7,att,0, NULL);
	mvchgat(ySC + 2,xSC,9,att,0, NULL);
	mvchgat(ySC + 3,xSC,9,att,0, NULL);
	mvchgat(ySC + 4,xSC + 1,7,att,0, NULL);
}
void chgatCoor(struct Coordinate c, int att){
	int ySC, xSC;
	coordinateToScreenLocation(c,&ySC,&xSC);
	chgatHex(ySC,xSC,att);
}

const struct Coordinate * getAdjacent(struct Coordinate c)
{
	int x = c.x, y = c.y;
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

void addTileLoc(struct Coordinate c, int owner, char type){
	struct Location * loc = &grid[c.y][c.x];
	struct Tile * ct = &(loc->tiles[loc->numTiles]);
	ct->owner = owner;
	ct->type = type;
	loc->numTiles++;
}

void removeTile(struct Coordinate c){
	struct Location * loc = coordinateToLocation(c);
	loc->numTiles--;
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

struct Tile * getTopTile(struct Coordinate c){
	struct Location * loc = &grid[c.y][c.x];
	if(loc->numTiles == 0){
		return &(loc->tiles[0]);
	}
	int index = loc->numTiles - 1;
	return &((*loc).tiles[index]);
}

void mvTile(struct Coordinate c1, struct Coordinate c2){
	struct Location * loc1 = coordinateToLocation(c1);
	struct Location * loc2 = coordinateToLocation(c2);
	loc2->numTiles++;
	struct Tile * t1 = getTopTile(c1);
	struct Tile * t2 = getTopTile(c2);
	*t2 = *t1;
	loc1->numTiles--;
}
void drawTile(struct Coordinate c){
	struct Location * loc = coordinateToLocation(c);
	int ySC, xSC;
	coordinateToScreenLocation(c,&ySC,&xSC);
	struct Tile * t = getTopTile(c);
	if(loc->numTiles > 0){
		drawHex(ySC, xSC);
	}
	mvaddstr(ySC+2,xSC+3,typeToCode(t->type));
}


void drawRow(int r){
	for(int i = 0; i < GRID_MAX_X; i++){
		drawTile((struct Coordinate){r,i});
	}
}

void drawGrid(){
	for(int i = 0; i < GRID_MAX_Y; i++)
		drawRow(i);
	int xSC, ySC;
	coordinateToScreenLocation(hlLocation,&ySC,&xSC);
	hlHex(ySC,xSC);
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
	players[1] = players[0];
}

void drawRemaining(int player){
	
}

void queenMove(struct Coordinate c){
	struct Coordinate * adj = getAdjacent(c);
	for(int i = 0; i < 6; i++){
		if(validCoordinate(adj[i])){
			mvprintw(i + 5,60,"X: %d, Y: %d",adj[i].x,adj[i].y);
		}
	}
}

void testMove(){
	int ch = getch();
	while(ch != 'q'){
		drawGrid();
		switch (ch){
			case 'w':
				hlLocation.y--;
				break;
			case 'a':
				hlLocation.x--;
				break;
			case 's':
				hlLocation.y++;
				break;
			case 'd':
				hlLocation.x++;
				break;
			case 'm':
				queenMove(hlLocation);
				break;
		}
		ch = getch();
	}
}
int main(){
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	getmaxyx(stdscr, h, w );
	curs_set(0);
	initPlayers();
	for(int i = 1; i < 9; i++){
		struct Location * loc = &grid[0][i-1];
		addTile(loc,0,i);
	}
	drawGrid();
	testMove();
	endwin();
	return 0;
}
