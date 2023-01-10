/*
 * Brush.h
 *
 *  Created on: Feb 19, 2017
 *      Author: Bunjukz
 */

#ifndef INCLDS_BRUSH_BRUSH_H_
#define INCLDS_BRUSH_BRUSH_H_

#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <Tiling/LevelInfo.h>

struct Line {
	SDL_Point line[2];
	void setLine (int from_x, int from_y, int to_x, int to_y){
		line[0].x = from_x;
		line[0].y = from_y;
		line[1].x = to_x;
		line[1].y = to_y;
	}
};

struct Selection {
	enum SELECTION_TYPES{NOTHING, TILE, OBJECT} what;
	int type;
	int tileGridEnabled;
	int objGridEnabled;
	int tileCollisionEnabled;
	int objCollisionEnabled;
};

typedef enum BRUSH_MODE {
	DRAW, // B
	ERASE, // E
	FILL, // F
	SELECTION // S
}BRUSH_MODE;

struct Brush {
private:
	std::vector<SDL_Point> GetTilesCircle();
	enum COLLISION {
		LEFT = 0x1,
		RIGHT = 0x2,
		UP = 0x4,
		DOWN = 0x8
	};
	std::vector<Line> lines;
public:
	Brush();
	int brushSize;
	void getBrushLines 		(std::vector<Line>* line_In);
	void UpdateBrushLines	();
	void FillTiles			(int x_In, int y_In, int type_In, Level_Info* tiles_In);
	BRUSH_MODE mode;
};



#endif /* INCLDS_BRUSH_BRUSH_H_ */
