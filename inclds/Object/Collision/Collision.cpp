/*
 * Collision.cpp
 *
 *  Created on: Feb 20, 2017
 *      Author: Bunjukz
 */

#include <Object/Collision/Collision.h>
#include <Tiling/LevelInfo.h>

int CheckCollisionWithObjectsCentered(int x_In, int y_In, void* levelInfo_In){
	ObjectC _obj;
	Level_Info* levelInfo = (Level_Info*) levelInfo_In;
	int _grid = levelInfo->GetGridFloored();
	int leftA, leftB;
	int rightB;
	int topA, topB;
	int bottomB;
	leftA = x_In;
	topA = y_In;
	float zoom = levelInfo->zoom;
	for(int i = levelInfo->objectsPlaced - 1; i > -1; i-- ){
		levelInfo->GetObjects(&_obj, i);
		leftB 	= (_obj.getCollisionBox().x * _grid) - floor(_obj.getCollisionBox().w * 0.5f * zoom);
		rightB 	= (_obj.getCollisionBox().x * _grid) + floor(_obj.getCollisionBox().w * 0.5f * zoom);
		topB 	= (_obj.getCollisionBox().y * _grid) - floor(_obj.getCollisionBox().h * 0.5f * zoom);
		bottomB = (_obj.getCollisionBox().y * _grid) + floor(_obj.getCollisionBox().h * 0.5f * zoom);
		if(((topA <= topB) || (topA >= bottomB) || (leftA <= leftB) || (leftA >= rightB)) == false)
			return i;
	}
	return -1;
}
