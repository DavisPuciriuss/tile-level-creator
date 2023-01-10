/*
 * Inputs.h
 *
 *  Created on: Feb 3, 2017
 *      Author: Bunjukz
 */

#ifndef INCLDS_INPUTS_INPUTS_H_
#define INCLDS_INPUTS_INPUTS_H_


typedef enum MOUSE_STATES {
	NONE,
	CLICKED,
	HELD,
	RELEASED
}MOUSE_STATES;

struct str_Mouse {
	SDL_Point location;
	SDL_Point location_old;
	SDL_Point offset;
	float scroll_tot;
	MOUSE_STATES m1;
	MOUSE_STATES m2;
	MOUSE_STATES m3;
	int scroll;
	int moved;
};

struct str_Keyboard {
	int ctrl;
	int shift;
	int alt;
};

#endif /* INCLDS_INPUTS_INPUTS_H_ */
