/*
 * MainObject.h
 *
 *  Created on: Sep 10, 2016
 *      Author: Bunjukz
 */

#ifndef INCLDS_OBJECT_MAINOBJECT_H_
#define INCLDS_OBJECT_MAINOBJECT_H_

#include <SDL.h>

class ObjectC{
public:
	void		Init(const SDL_Rect& box, int type);
	void 		Clean();
	void		setLocation(SDL_Point);
	int			getType();
	SDL_Rect	getCollisionBox();
private:
	int mType;
	SDL_Rect	Location;
};



#endif /* INCLDS_OBJECT_MAINOBJECT_H_ */
