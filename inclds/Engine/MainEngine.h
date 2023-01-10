/*
 * MainEngine.h
 *
 *  Created on: Sep 10, 2016
 *      Author: Bunjukz
 */

#ifndef INCLDS_ENGINE_MAINENGINE_H_
#define INCLDS_ENGINE_MAINENGINE_H_

#include <tgmath.h>
#include <tchar.h>
#include <ctime>
#include <sstream>
#include <vector>
#include <SDL_thread.h>
#include <SDL_image.h>
#include <SDL_TTF.h>
#include <Object/Collision/Collision.h>
#include <Timer/Timer.h>
#include <Graphics/Texture/Texture.h>
#include <Object/MainObject.h>
#include <Camera/MainCamera.h>
#include <Button/DefaultButton.h>
#include <Inputs/Inputs.h>
#include <RectEditor/RectEditor.h>
#include <Brush/Brush.h>
#include <Tiling/LevelInfo.h>
#include <Windows/WinAPI_Wrappers.h>
#include <IOStream/IOStream_Wrappers.h>



typedef enum ENGINE_STATES {
	INIT,
	LEVELEDITING,
	COLLISIONEDITING
}ENGINE_STATES;

class EngineC {
public:
	void 		Init(void*,void*);
	int 		Start();
	int 		Main();
	void 		Clean();
	/*
	 * Getters
	 */
	int					getTotButtons();
	Selection 			getSelection();
	float 				getZoom();
	DefaultButtonClass* getButton(int);
	Collision_Data		getCollisionData(std::string, int);
	Level_Info			GetLevelInfo();
	Brush				GetBrush();
	SDL_Point			renderTilesPoints();
	SDL_Point			renderObjectsPoints();
	SDL_Point			getCameraPosition();
	str_Mouse			getMouseStr();
	TextC*				getText();
	RectEditorCl* 		getEditor(int);
	ENGINE_STATES 		getState();
	int					getScrollOffset();
	int					getEditorCount();
private:
	/*
	 * Initialization.
	 */
	void InitilizeButtons();
	void InitilizeTexts();
	void LoadSprites(std::string dir_In, Collision_Data** collision_Out, int* loadedPngs_Out);
	void LoadCollisionData(std::string dir_In, Collision_Data** collision_Out, Selection::SELECTION_TYPES type_In);
	/*
	 *  Dynamic array allocation.
	 */

	void AllocateButtonArray();
	void DeleteButtonArray();
	/*
	 * Event checking and responses.
	 */
	int 	CheckEvents();
	void 	OnClick(int);
	void 	UpdateScroll();
	/*
	 * Tiles.
	 */
	void	FillTiles(SDL_Point);
	void 	PickTile(SDL_Point);
	void 	PlaceTile(SDL_Point);
	/*
	 * Camera
	 */
	void MoveCamera();
	/*
	 * Objects
	 */
	void PickObject(SDL_Point);
	void PlaceObject(SDL_Point);
	void DestroyObject(SDL_Point);
	void StartMovingObject(SDL_Point _mouseLocation);
	/*
	 * Variables.
	 */
	int mRun;				// Flags if the program is running.
	int mTotalButtons;		// Total buttons allocated.
	int objH;				// Total object size for the sprite holder box.
	int editorSize;			// How many collision boxes have been added.
	SDL_Point	mTilesStartRender;
	SDL_Point	mObjectsStartRender;
	void*		mGraphics;
	void*		mEngine;
	SDL_Event	mEvent;
	/*
	 * Classes.
	 */
	Selection			selection;
	ENGINE_STATES 		iState;
	str_Mouse			mMouse;
	str_Keyboard		mKeyboard;
	MainCameraClass		mCamera;
	DefaultButtonClass*	mButtons;
	Collision_Data*		mTilesCollision;
	Collision_Data*		mObjectsCollision;
	TextC				mTexts;
	RectEditorCl		mEditors[5];
	Level_Info			levelInfo;
	Brush				brush;
};



#endif /* INCLDS_ENGINE_MAINENGINE_H_ */
