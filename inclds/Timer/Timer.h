/*
 * Timer.h
 *
 *  Created on: Jun 10, 2016
 *      Author: Bunjukz
 */

#ifndef INCLDS_TIMER_TIMER_H_
#define INCLDS_TIMER_TIMER_H_

#include <SDL.h>
#include <stdio.h>

class TimerC {
public:
	TimerC();
	void Start();
	void Pause();
	void Unpause();
	void Stop();

	Uint32 GetTick();

	int isPaused();
	int isStarted();
private:
	Uint32 	startTick,pausedTick;
	int		mStarted,mPaused;
};

#endif /* INCLDS_TIMER_TIMER_H_ */
