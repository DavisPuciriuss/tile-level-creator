/*
 * IOStream_Wrappers.h
 *
 *  Created on: Feb 20, 2017
 *      Author: Bunjukz
 */

#ifndef INCLDS_IOSTREAM_IOSTREAM_WRAPPERS_H_
#define INCLDS_IOSTREAM_IOSTREAM_WRAPPERS_H_

#include <string>
#include <SDL.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <Object/Collision/Collision.h>

namespace IOStream {

	void 	Export(std::string in_directory, void* levelInfo_In);
	int 	Import(std::string in_directory, void* levelInfo_In, void* graphics_In);

	void 	ExportCollisionInfo(SDL_Rect* _boxIn, int _boxSize, int* _assetID, void* selection_In);
	void 	ImportCollisionInfo(int tileSize, void* selection_In, void* graphics_In, void* editors_In, int* editorSize_Out);
	void 	LoadCollisionInfo(int type_In, void* selection_In, Collision_Data** collision_data_Out);

	int 	FileExists(std::string);
	int	 	SkippableCharacter(char _c);
	int	 	getIntFromChar(char* _charIn);
	void 	getIntFromString(std::string** string_In, int stringSize, int** int_Out);

}



#endif /* INCLDS_IOSTREAM_IOSTREAM_WRAPPERS_H_ */
