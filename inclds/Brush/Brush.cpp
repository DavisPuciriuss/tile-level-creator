/*
 * Brush.cpp
 *
 *  Created on: Feb 19, 2017
 *      Author: Bunjukz
 */

#include <Brush/Brush.h>

Brush::Brush(){
	brushSize = 1;
	mode = BRUSH_MODE::DRAW;
	UpdateBrushLines();
}

void Brush::getBrushLines 	(std::vector<Line>* line_In){
	if(lines.size() != 0)
		*line_In = lines;
}
void Brush::UpdateBrushLines(){
	lines.clear();
	std::vector<SDL_Point> _tiles = GetTilesCircle();
	for(SDL_Point pointA : _tiles){
		char outsideTile = 0;
		for(SDL_Point pointB : _tiles){
			if(pointA.x + 1 == pointB.x && pointA.y == pointB.y){
				outsideTile |= LEFT;
			}
			if(pointA.x - 1 == pointB.x && pointA.y == pointB.y){
				outsideTile |= RIGHT;
			}
			if(pointA.y + 1 == pointB.y && pointA.x == pointB.x){
				outsideTile |= UP;
			}
			if(pointA.y - 1 == pointB.y && pointA.x == pointB.x){
				outsideTile |= DOWN;
			}
		}
		if(!(outsideTile & LEFT)){
			Line _tmp;
			_tmp.setLine(pointA.x + 1, pointA.y, pointA.x + 1, pointA.y + 1);
			lines.push_back(_tmp);
		}
		if(!(outsideTile & RIGHT)){
			Line _tmp;
			_tmp.setLine(pointA.x, pointA.y, pointA.x, pointA.y + 1);
			lines.push_back(_tmp);
		}
		if(!(outsideTile & UP)){
			Line _tmp;
			_tmp.setLine(pointA.x, pointA.y + 1, pointA.x + 1, pointA.y + 1);
			lines.push_back(_tmp);
		}
		if(!(outsideTile & DOWN)){
			Line _tmp;
			_tmp.setLine(pointA.x, pointA.y, pointA.x + 1, pointA.y);
			lines.push_back(_tmp);
		}
	}
}
std::vector<SDL_Point> Brush::GetTilesCircle(){
	std::vector <SDL_Point> _tmp;
	int div = brushSize >> 1;
	int x;
	int y;
	int rx;
	int ry;
	int rxx;
	int ryy;
	int r 	= (brushSize >> 1) +1;
	int dd 	= (brushSize * brushSize) + brushSize;
	for(rx = -r, x = div + rx, rxx = rx * rx; rx <= r; rx++, x++, rxx = rx * rx){
		for (ry = -r ,y = div + ry ,ryy = ry * ry; ry <= r; ry++, y++, ryy = ry * ry){
			if ((rxx + ryy) << 2 < dd){
				SDL_Point ddd =  {x,y};
				_tmp.push_back(ddd);
			}
		}
	}
	return _tmp;
}
void Brush::FillTiles(int x_In, int y_In, int type_In, Level_Info* info_In){
	int x;
	int y;
	int rx;
	int ry;
	int rxx;
	int ryy;
	int r 	= (brushSize >> 1) + 1;
	int dd 	= (brushSize * brushSize) + brushSize;
	int blankTile = -1;
	for(rx = -r, x = x_In + rx, rxx = rx * rx; rx <= r; rx++, x++, rxx = rx * rx){
		for (ry = -r ,y = y_In + ry ,ryy = ry * ry; ry <= r; ry++, y++, ryy = ry * ry){
			if ((rxx + ryy) << 2 < dd){
				if ((x >= 0) && (x < info_In->width) && (y >= 0)&&(y < info_In->height)){
					if(mode == BRUSH_MODE::DRAW)
						(*info_In).SetTile(type_In,x,y);
					else if (mode == BRUSH_MODE::ERASE)
						(*info_In).SetTile(blankTile, x, y);
				}
			}
		}
	}
}
