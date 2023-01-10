/*
 * LevelInfo.h
 *
 *  Created on: Feb 19, 2017
 *      Author: Bunjukz
 */

#ifndef INCLDS_TILING_LEVELINFO_H_
#define INCLDS_TILING_LEVELINFO_H_

#include <Object/MainObject.h>
#include <SDL.h>
#include <stdio.h>
#include <tgmath.h>
#include <iostream>
#include <vector>

struct Level_Info {
private:
	/*
	 * Variables
	 */
	int** 		tilemap;
	int**		temptilemap;
	ObjectC*	objects;
	ObjectC*	tempobjects;
	ObjectC*	moveobject;
	/*
	 * Memory allocation.
	 */
	void AllocateTilesArray		(int new_width, int new_height);
	void SaveTilesArray			();
	void DeleteTilesArray		();
	void DeleteTempTilesArray	();
	void AllocateObjectArray	(int new_obj);
	void SaveObjectArray		();
	void SaveObjectArrayExcept	(int id_In);
	void DeleteObjectArray		();
	void DeleteTempObjectArray	();
public:
	/*
	 * Variables
	 */
	int 		width;
	int 		height;
	int			tileSize;
	int			gridSize;
	int			objectsPlaced;
	int			tilesLoaded;
	int			objectsLoaded;
	float		zoom;
	/*
	 * Initialization
	 */
	void Init			();
	void InitObject		(int id_In, const SDL_Rect& box, int type_In);
	void InitObject		(int id_In, SDL_Point loc_In, int width_In, int height_In, int type_In);
	/*
	 * Destroyers
	 */
	void Clean			();
	void DestroyObject	(int id_In);
	void ResetLevel		();
	/*
	 * Getters
	 */
	void 	GetObjects		(ObjectC* obj_Out, int id_In);
	void 	GetTile			(int* tile_Out, int x_In, int y_In);
	float 	GetTileScaled	();
	int	 	GetTileFloored	();
	float 	GetGridScaled	();
	int		GetGridFloored	();
	int	 	GetTileEqual	(int tile_In, int x_In, int y_In);
	/*
	 * Setters
	 */
	void SetLevelSize	(int width_In, int height_In, int obj_In);
	void SetObjects		(ObjectC obj_In, int id_In);
	void SetTile		(int tile_In, int x_In, int y_In);
	void SetTileFast	(int tile_In, int x_In, int y_In);
	void FillArea		(int tile_In, int x_In, int y_in);
	void SetMovingObject(int id_In);
	/*
	 * Updates
	 */
	void MoveObject		(SDL_Point location_In);
};



#endif /* INCLDS_TILING_LEVELINFO_H_ */
