/*
 * MainCamera.h
 *
 *  Created on: Aug 30, 2016
 *      Author: Bunjukz
 */

#ifndef INCLDS_CAMERA_MAINCAMERA_H_
#define INCLDS_CAMERA_MAINCAMERA_H_

#include <SDL.h>
#include <stdio.h>

class MainCameraClass{
public:
	int 		Init(int,int);
	int 		Clean();
	int 		GetWidth();
	int 		GetHeight();
	float		getWidthScaled();
	float		getHeightScaled();
	void		SetLocation(SDL_Point);
	void		SetDimensions(float );
	SDL_Point 	GetLocation();
private:
	SDL_Point 	mLocation;
	int			mW;
	int			mH;
	float		mW_scaled;
	float		mH_scaled;
};



#endif /* INCLDS_CAMERA_MAINCAMERA_H_ */
