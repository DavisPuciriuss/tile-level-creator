/*
 * Timer.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: Bunjukz
 */

#include <Timer/Timer.h>

TimerC::TimerC(){
	startTick 	= 0;
	pausedTick 	= 0;
	mStarted	= 0;
	mPaused		= 0;
}
Uint32 TimerC::GetTick(){
	Uint32 mTime;
	if(mStarted){
		if(mPaused){
			mTime = pausedTick;
		}else{
			mTime = SDL_GetTicks() - startTick;
		}
	}
	return mTime;
}
void TimerC::Pause(){
	if(mStarted && !mPaused){
		mPaused 	= 1;
		pausedTick	= SDL_GetTicks() - startTick;
		startTick	= 0;
	}
}
void TimerC::Start(){
	mStarted 	= 1;
	mPaused 	= 0;
	startTick 	= SDL_GetTicks();
	pausedTick 	= 0;
}
void TimerC::Stop(){
	mStarted	= 0;
	mPaused		= 0;
	startTick 	= 0;
	pausedTick	= 0;
}
void TimerC::Unpause(){
	if(mStarted && mPaused){
		mPaused 	= 0;
		startTick	= SDL_GetTicks() - pausedTick;
		pausedTick	= 0;
	}
}
int TimerC::isPaused(){
	return mPaused;
}
int TimerC::isStarted(){
	return mStarted;
}
