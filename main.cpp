/*
 * main.cpp
 *
 *  Created on: Sep 10, 2016
 *      Author: Bunjukz
 */

#include <Engine/MainEngine.h>
#include <Graphics/MainGraphics.h>

int** allocateINTArray(int in_x, int in_y){
	int** _tmpArray;
	_tmpArray = new int*[in_x];
	for(int i = 0; i < in_x; i++){
		_tmpArray[i] = new int[in_y];
	}
	return _tmpArray;
}

int main(int argc, char* args[]){
	/*
	 * CheckEvent();
	 * if mouse down call onClick();
	 * check if mouse click on grid or on tile/object selection field;
	 * etc etc.
	 */
	EngineC 	mEngine;
	GraphicsCl	mGraphics;
	mEngine.Init((void*)&mGraphics,(void*)&mEngine);
	mEngine.Start();

	return 0;
}


