/*
 * LevelInfo.cpp
 *
 *  Created on: Feb 19, 2017
 *      Author: Bunjukz
 */

#include <Tiling/LevelInfo.h>

/*
 * Initialization
 */
void Level_Info::Init(){
	tilemap 	= nullptr;
	objects 	= nullptr;
	tempobjects	= nullptr;
	temptilemap = nullptr;
	moveobject	= nullptr;
	width 			= 0;
	height 			= 0;
	tileSize		= 0;
	gridSize		= 0;
	objectsPlaced 	= 0;
	tilesLoaded 	= 0;
	objectsLoaded 	= 0;
	zoom			= 1.0f;
}
void Level_Info::InitObject		(int id_In, const SDL_Rect& box, int type_In){
	if(objects != nullptr && id_In >= 0 && id_In < objectsPlaced)
		objects[id_In].Init(box, type_In);
}
void Level_Info::InitObject		(int id_In, SDL_Point loc_In, int width_In, int height_In, int type_In){
	if(objects != nullptr && id_In >= 0 && id_In < objectsPlaced)
		objects[id_In].Init(SDL_Rect {loc_In.x, loc_In.y, width_In, height_In}, type_In);
}
/*
 * Destroyers
 */
void Level_Info::Clean(){
	ResetLevel();
	DeleteTempObjectArray();
	DeleteTempTilesArray();
	moveobject	= nullptr;

	width 			= 0;
	height 			= 0;
	tileSize		= 0;
	gridSize		= 0;
	objectsPlaced 	= 0;
	tilesLoaded 	= 0;
	objectsLoaded 	= 0;
	zoom			= 1.0f;
}
void Level_Info::DestroyObject	(int id_In){
	if(id_In >= 0 && id_In < objectsPlaced && objects != nullptr){
		if(objectsPlaced >= 2){
			SaveObjectArrayExcept(id_In);
			DeleteObjectArray();
			objectsPlaced--;
			AllocateObjectArray(objectsPlaced);
		}else{
			DeleteObjectArray();
			DeleteTempObjectArray();
			objectsPlaced = 0;
		}
	}
}
void Level_Info::ResetLevel		(){
	DeleteTilesArray();
	DeleteObjectArray();
	DeleteTempObjectArray();
	DeleteTempTilesArray();
	width 			= 0;
	height 			= 0;
	objectsPlaced	= 0;
	moveobject = nullptr;
}
/*
 * Memory allocation.
 */
void Level_Info::AllocateTilesArray(int new_width, int new_height){
	/*
	 * Allocates the tilemap with a new size.
	 * Fills the tile map with blank tiles (-1).
	 * Restores the tiles from temptilemap into tilemap.
	 * Delete the temptilemap.
	 * Set the width and height to the new sizes.
	 */
	tilemap = new int*[new_width];
	for(int i = 0; i < new_width; i++)
		tilemap[i] = new int[new_height];
	for(int x = 0; x < new_width; x++)
		for(int y = 0; y < new_height; y++)
			tilemap[x][y] = -1;
	if(temptilemap != nullptr)
		for(int x = 0, xx = 0, yy = 0; x < width; x++){
			if(x > new_width - 1){
				xx++;
				yy = 0;
			}
			for(int y = 0; y < height; y++){
				if(y > new_height - 1){
					yy++;
				}
				tilemap[x + xx][y + yy] = temptilemap[x][y];
			}
		}
	for(int i = 0; i < width; i++)
		delete [] temptilemap[i];
	delete [] temptilemap;
	temptilemap = nullptr;
	width = new_width;
	height = new_height;
}
void Level_Info::SaveTilesArray(){
	if(temptilemap != nullptr){
		for(int i = 0; i < width; i++)
			delete [] temptilemap[i];
		delete [] temptilemap;
	}
	temptilemap = new int*[width];
	for(int i = 0; i < width; i++)
		temptilemap[i] = new int[height];
	if(tilemap != nullptr)
		for(int x = 0; x < width; x++)
			for(int y = 0; y < height; y++)
				temptilemap[x][y] = tilemap[x][y];
}
void Level_Info::DeleteTilesArray(){
	/*
	 * Stores the current placed tiles into a temporary tile map.
	 * Deletes the old tile map.
	 */
	if(tilemap != nullptr){
		for(int i = 0; i < width; i++)
			delete [] tilemap[i];
		delete [] tilemap;
		tilemap = nullptr;
	}
}
void Level_Info::DeleteTempTilesArray	(){
	if(temptilemap != nullptr){
		for(int x = 0; x < width; x++)
			delete [] temptilemap[x];
		delete [] temptilemap;
	}
	temptilemap = nullptr;
}
void Level_Info::AllocateObjectArray(int new_obj){
	/*
	 * Creates the object array with the new size
	 * Restores the old placed objects.
	 * Deletes the old objects.
	 * Sets the object amount to the new size.
	 */
	objects = new ObjectC[new_obj];
	if(tempobjects != nullptr)
		for(int i = 0; i < objectsPlaced; i++){
			if(i > new_obj)
				i = new_obj;
			objects[i] = tempobjects[i];
		}
	delete [] tempobjects;
	tempobjects 	= nullptr;
	objectsPlaced 	= new_obj;
}
void Level_Info::SaveObjectArray(){
	/*
	 * Delete the old temporary object array if it exists
	 * Create the temporary object array.
	 * Store all of the objects in it.
	 */
	if(tempobjects != nullptr)
		delete [] tempobjects;
	tempobjects = new ObjectC[objectsPlaced];
	for(int i = 0; i < objectsPlaced; i++)
		tempobjects[i] = objects[i];
}
void Level_Info::SaveObjectArrayExcept(int id_In){
	/*
	 * Delete the old temporary object array if it exists
	 * Create the temporary object array.
	 * Store all of the objects except the one you are deleting in it.
	 */

	if(tempobjects != nullptr)
		delete [] tempobjects;
	if(objectsPlaced >= 2){
		tempobjects = new ObjectC[objectsPlaced - 1];
		for(int i = 0; i < objectsPlaced; i++){
			if(i < id_In)
				tempobjects[i] = objects[i];
			else if (i > id_In)
				tempobjects[i-1] = objects[i];
		}
	}
}
void Level_Info::DeleteObjectArray(){
	/*
	 * Delete the object array.
	 * Set the object array to nullptr.
	 */
	if(objects != nullptr){
		delete [] objects;
		objects = nullptr;
	}
}
void Level_Info::DeleteTempObjectArray	(){
	if(tempobjects != nullptr){
		delete [] tempobjects;
		tempobjects = nullptr;
	}
}
/*
 * Getters
 */
void Level_Info::GetObjects		(ObjectC* obj_Out, int id_In){
	if(objects != nullptr && id_In < objectsPlaced)
		*obj_Out = objects[id_In];
	else
		obj_Out = nullptr;
}
void Level_Info::GetTile		(int* tile_Out, int x_In, int y_In){
	if(tilemap != nullptr && x_In < width && y_In < height)
		*tile_Out = tilemap[x_In][y_In];
	else
		tile_Out = nullptr;
}
float Level_Info::GetTileScaled	(){
	return (tileSize * zoom);
}
int Level_Info::GetTileFloored	(){
	return (int)floor(tileSize * zoom);
}
float Level_Info::GetGridScaled	(){
	return (gridSize * zoom);
}
int Level_Info::GetGridFloored	(){
	return (int)floor(gridSize * zoom);
}
int	 Level_Info::GetTileEqual	(int tile_In, int x_In, int y_In){
	if(tilemap != nullptr && x_In >= 0 && y_In >= 0 && x_In < width && y_In < height)
		return tilemap[x_In][y_In] == tile_In;
	return 0;
}
/*
 * Setters
 */
void Level_Info::SetLevelSize	(int width_In, int height_In, int obj_In){
	if((width_In != -1 && height_In != -1) && (width_In != width || height_In != height)){
		if(width_In < width)
			width_In = width;
		if(height_In < height)
			height_In = height;
		SaveTilesArray();
		DeleteTilesArray();
		if(width_In > 0 && height_In > 0)
			AllocateTilesArray(width_In, height_In);
	}
	if((obj_In != -1) && (obj_In != objectsPlaced)){
		SaveObjectArray();
		DeleteObjectArray();
		if(obj_In > 0)
			AllocateObjectArray(obj_In);
	}
}
void Level_Info::SetObjects		(ObjectC obj_In, int id_In){
	if(id_In < objectsPlaced && id_In >= 0 && objects != nullptr)
		objects[id_In] = obj_In;
}
void Level_Info::SetTile		(int tile_In, int x_In, int y_In){
	if(tilemap != nullptr && x_In >= 0 && y_In >= 0 && x_In < width && y_In < height)
		tilemap[x_In][y_In] = tile_In;
}
void Level_Info::SetTileFast	(int tile_In, int x_In, int y_In){
	tilemap[x_In][y_In] = tile_In;
}
void Level_Info::FillArea		(int tile_In, int x_In, int y_In){
	int FillType;
	if(width > x_In && x_In >= 0 && height > y_In && y_In >= 0){
		GetTile(&FillType, x_In, y_In);
		std::vector <SDL_Point> nextLocation;
		if(FillType != tile_In){
			SDL_Point _tmp = {x_In, y_In};
			nextLocation.push_back(_tmp);
		}
		while (nextLocation.size() > 0){
			SDL_Point _tmp = nextLocation.back();
			nextLocation.pop_back();
			int x = _tmp.x;
			int y = _tmp.y - 1;
			if(GetTileEqual(FillType, x, y)){
				SDL_Point tmp = {x,y};
				nextLocation.push_back(tmp);
				SetTileFast(tile_In, x, y);
			}
			x = _tmp.x - 1;
			y = _tmp.y;
			if(GetTileEqual(FillType, x, y)){
				SDL_Point tmp = {x,y};
				nextLocation.push_back(tmp);
				SetTileFast(tile_In, x, y);
			}
			x = _tmp.x + 1;
			y = _tmp.y;
			if(GetTileEqual(FillType, x, y)){
				SDL_Point tmp = {x,y};
				nextLocation.push_back(tmp);
				SetTileFast(tile_In, x, y);
			}
			x = _tmp.x;
			y = _tmp.y + 1;
			if(GetTileEqual(FillType, x, y)){
				SDL_Point tmp = {x,y};
				nextLocation.push_back(tmp);
				SetTileFast(tile_In, x, y);
			}
		}
		nextLocation.clear();
	}
}
void Level_Info::SetMovingObject(int id_In){
	if(objects != nullptr && id_In >= 0 && id_In < objectsPlaced)
		moveobject = &objects[id_In];
	else
		moveobject = nullptr;
}
/*
 * Updates
 */
void Level_Info::MoveObject		(SDL_Point location_In){
	if(moveobject != nullptr){
		SDL_Point loc = {(int)floor(location_In.x / floor(gridSize * zoom)), (int)floor(location_In.y / floor(gridSize * zoom))};
		moveobject->setLocation(loc);
	}
}
