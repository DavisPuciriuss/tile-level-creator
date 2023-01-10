/*
 * MainObject.cpp
 *
 *  Created on: Sep 10, 2016
 *      Author: Bunjukz
 */

#include <Object/MainObject.h>

void ObjectC::Init(const SDL_Rect& box, int type){
	Location = box;
	mType = type;
}
int ObjectC::getType(){
	return mType;
}
void ObjectC::setLocation(SDL_Point point){
	Location.x 		= point.x;
	Location.y 		= point.y;
}
SDL_Rect ObjectC::getCollisionBox(){
	return Location;
}
void ObjectC::Clean(){
	mType		= 0;
}
