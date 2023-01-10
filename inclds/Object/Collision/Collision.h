/*
 * Collision.h
 *
 *  Created on: Feb 20, 2017
 *      Author: Bunjukz
 */

#ifndef INCLDS_OBJECT_COLLISION_COLLISION_H_
#define INCLDS_OBJECT_COLLISION_COLLISION_H_


#include <SDL.h>

struct Collision_Data {
	Collision_Data() : boxes(0) {}
	int 		boxes;
	SDL_Rect 	box[5];

	void operator= (const Collision_Data& in){
		for(int i = 0; i < 5; i++){
			this->box[i].x = in.box[i].x;
			this->box[i].y = in.box[i].y;
			this->box[i].w = in.box[i].w;
			this->box[i].h = in.box[i].h;
		}
		this->boxes = in.boxes;
	}
};

extern int CheckCollisionWithObjectsCentered(int x, int y, void* levelInfo_In);


#endif /* INCLDS_OBJECT_COLLISION_COLLISION_H_ */
