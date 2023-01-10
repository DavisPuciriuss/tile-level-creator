/*
 * MainCamera.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: Bunjukz
 */

#include <Camera/MainCamera.h>

int MainCameraClass::Init(int w,int h){
	mW 				= w;
	mH 				= h;
	mW_scaled		= w;
	mH_scaled		= h;
	mLocation.x 	= 0;
	mLocation.y 	= 0;
	return 0;
}
int  MainCameraClass::Clean(){
	return 0;
}
void MainCameraClass::SetLocation(SDL_Point mNewLocation){
	mLocation.x = mNewLocation.x;
	mLocation.y = mNewLocation.y;
	if(mLocation.x < 0)
		mLocation.x = 0;
	if(mLocation.y < 0)
		mLocation.y = 0;
}
void MainCameraClass::SetDimensions(float _scale){
	mW_scaled = mW * _scale;
	mH_scaled = mH * _scale;
}
SDL_Point MainCameraClass::GetLocation(){
	return mLocation;
}
int	MainCameraClass::GetWidth(){
	return mW;
}
int	MainCameraClass::GetHeight(){
	return mH;
}
float		MainCameraClass::getWidthScaled(){
	return mW_scaled;
}
float		MainCameraClass::getHeightScaled(){
	return mH_scaled;
}
