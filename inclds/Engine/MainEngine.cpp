/*
 * MainEngine.cpp
 *
 *  Created on: Sep 10, 2016
 *      Author: Bunjukz
 */



#include <Engine/MainEngine.h>
#include <Graphics/MainGraphics.h>

void EngineC::Init(void* pGraphics,void* pEngine){
	mRun 				= 1;

	mButtons 			= nullptr;
	mTilesCollision		= nullptr;
	mObjectsCollision	= nullptr;

	selection			= {};
	mMouse				= {};
	mKeyboard			= {};

	selection.what		= selection.NOTHING;
	selection.type		= 0;

	editorSize			= 0;

	mMouse.scroll_tot	= 1;
	iState				= INIT;
	mGraphics			= pGraphics;
	mEngine				= pEngine;
	levelInfo.Init();
}
int EngineC::Start(){
	/*
	 * Init SDL
	 * Init Graphics
	 * Load Media
	 * Go into main loop
	 */
	GraphicsCl* realGraphics = (GraphicsCl*)mGraphics;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL_INIT FAILED! %s \n",SDL_GetError());
		Clean();
	}
	if(!IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG){
		printf("IMG_INIT FAILED! %s \n",IMG_GetError());
		Clean();
	}
	if(TTF_Init() == -1){
		printf("TTF_INIT FAILED! %s \n",TTF_GetError());
		Clean();
	}

	/*
	 * Find all of the tile *.info files in "./Data/Tiles" location.
	 */
	LoadSprites("./Data/Tiles/", &mTilesCollision, &levelInfo.tilesLoaded);
	LoadCollisionData("./Data/Tiles/", &mTilesCollision, Selection::TILE);
	/*
	 * Find all of the object *.info files in "./Data/Objects" location.
	 */
	LoadSprites("./Data/Objects/", &mObjectsCollision, &levelInfo.objectsLoaded);
	LoadCollisionData("./Data/Objects/", &mObjectsCollision, Selection::OBJECT);

	realGraphics->Init(mEngine);
	realGraphics->frame.SDLCreateWindow(1280, 960, 60, "Map Editor v1.05", SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	realGraphics->frame.CreateRenderer(SDL_RENDERER_ACCELERATED);
	realGraphics->LoadMedia();
	int window_w, window_h;
	realGraphics->frame.GetDimensions(&window_w, &window_h);
	mCamera.Init(window_w,window_h);
	if(Main()){
		return 1;
	}
	return 0;
}

int EngineC::Main(){
	GraphicsCl* realGraphics = (GraphicsCl*)mGraphics;
	TimerC	_backupTimer;
	TimerC	_fpsTimer;
	while(mRun){
		switch(iState){
		case INIT:
			levelInfo.tileSize = 64;
			levelInfo.gridSize = 16;
			InitilizeButtons();
			InitilizeTexts();
			_backupTimer.Start();
			iState = LEVELEDITING;
			break;
		case LEVELEDITING:
			// @TODO
			_fpsTimer.Start();
			if(mMouse.moved){
				levelInfo.MoveObject(SDL_Point {mMouse.location.x - 192 + mCamera.GetLocation().x, mMouse.location.y + mCamera.GetLocation().y});
			}
			if(mMouse.m1 == RELEASED){
				levelInfo.SetMovingObject(-1);
			}
			if(mMouse.m3 == CLICKED){	// Mouse button 3 was pressed.
				mMouse.location_old = {mMouse.location.x, mMouse.location.y};
			}else if (mMouse.m3 == HELD && mMouse.moved){// Mouse button 3 is being held.
				MoveCamera();
			}
			if(mMouse.m1 == CLICKED || (mMouse.m1 == HELD && selection.what != Selection::OBJECT && mMouse.moved)){ // Mouse button 1 was pressed.
				OnClick(1);
			}
			if(mMouse.m2 == CLICKED){ // Mouse button 2 was pressed.
				OnClick(2);
			}
			UpdateScroll();
			if(selection.what == selection.TILE){
				for(int i = mTilesStartRender.x; i < mTilesStartRender.y; i++){
					if(mButtons[i].Update(&mMouse) == 1){
						selection.type = i - mTilesStartRender.x;
					}else if (mButtons[i].Update(&mMouse) == 2){
						selection.type 	= i - mTilesStartRender.x;
						iState			= COLLISIONEDITING;
					}
				}
			}else if (selection.what == selection.OBJECT){
				for(int i = mObjectsStartRender.x; i < mObjectsStartRender.y; i++){
					if(mButtons[i].Update(&mMouse) == 1){
						selection.type = i - mTilesStartRender.y;
					}else if (mButtons[i].Update(&mMouse) == 2){
						selection.type 	= i - mTilesStartRender.y;
						iState			= COLLISIONEDITING;
					}
				}
			}
			if(mButtons[0].Update(&mMouse) && selection.what != Selection::OBJECT){
				printf("Object Selected! \n");
				selection.what 		= Selection::OBJECT;
				selection.type 		= 0;
				mMouse.offset.x		= 0;
				mMouse.offset.y 	= objH - realGraphics->frame.GetH();
			}else if(mButtons[1].Update(&mMouse) && selection.what != Selection::TILE){
				printf("Tile Selected! \n");
				selection.what 		= Selection::TILE;
				selection.type 		= 0;
				mMouse.offset.x 	= 0;
				mMouse.offset.y 	= levelInfo.tileSize * floor(levelInfo.tilesLoaded / 3) - realGraphics->frame.GetH() + 40; // Tile size * (Tile amount Y) - Window_Height + Default_button_Height;
			}

			realGraphics->Update(mCamera.GetLocation(),mEngine); // Re - render.

			if(_backupTimer.GetTick() > 60000) {
				long _time;
				std::time(&_time);
				IOStream::Export("Data/Backups/backup_" + std::to_string(_time) + ".map", (void*)&levelInfo);
				_backupTimer.Start();
			}
			realGraphics->frame.LimitFPS(_fpsTimer.GetTick());
			if(CheckEvents()){
				return 1;
			}
			break;
		case COLLISIONEDITING:
			// @TODO
			// Start the FPS limiter counter.
			_fpsTimer.Start();
			// Draw the graphics.
			realGraphics->Update(mCamera.GetLocation() ,mEngine);

			/*
			 * Update all of the rectangle editors.
			 */
			if(editorSize != 0){
				for(int i = 0; i < editorSize; i++){
					mEditors[i].Update(&mMouse, &mEvent, realGraphics->frame.render);
					if(mEditors[i].getState() == 2){
						for(int z = 0; z < editorSize - i; z++){
							int _index = i + z + 1;
							if(_index < editorSize){
								mEditors[i + z] = mEditors[_index];
							}
						}
						editorSize--;
						i--;
						if(editorSize != 0)
							mEditors[0].setMain();
					}
				}
			}
			// Update the Export button
			if(mButtons[2].Update(&mMouse)){// Export
				SDL_Rect 	_tmpRect[editorSize];
				for(int i = 0; i < editorSize; i++){
					_tmpRect[i] = mEditors[i].getBox();
				}
				IOStream::ExportCollisionInfo(_tmpRect,editorSize,mEditors[0].getAssets(), (void*) &selection);
				if(selection.what == selection.TILE)
					IOStream::LoadCollisionInfo(selection.type, (void*) &selection.what, &mTilesCollision);
				else if (selection.what == selection.OBJECT)
					IOStream::LoadCollisionInfo(selection.type, (void*) &selection.what, &mObjectsCollision);
			}else if(mButtons[3].Update(&mMouse)){// Import
				IOStream::ImportCollisionInfo(levelInfo.tileSize, (void*) &selection, mGraphics, (void*)mEditors, &editorSize);
			}else if(mButtons[5].Update(&mMouse)){// GG
				editorSize = 0;
				iState = LEVELEDITING;
			}

			if(mButtons[4].Update(&mMouse)){// Add box
				if(editorSize <= 4){
					if(selection.what == Selection::TILE)
						mEditors[editorSize].Init(	1280 - 200, 175 * editorSize,200, 160, levelInfo.tileSize,
													realGraphics->getTexture(4,0),
													realGraphics->getTexture(0,0),
													realGraphics->getTexture(5,0) ,
													realGraphics->getFont("medium"),
													realGraphics->getColor("Black"),
													realGraphics->frame.render);
					else
						mEditors[editorSize].Init(SDL_Rect {1280 - 200, 175 * editorSize, 200, 160},
													realGraphics->getTexture(2,selection.type)->GetWidth(),
													realGraphics->getTexture(2,selection.type)->GetHeight(),
													realGraphics->getTexture(4,0),
													realGraphics->getTexture(0,0),
													realGraphics->getTexture(5,0),
													realGraphics->getFont("medium"),
													realGraphics->getColor("Black"),
													realGraphics->frame.render);
					if(!editorSize){
						mEditors[editorSize].setMain();
					}
					editorSize++;
				}
			}

			realGraphics->frame.LimitFPS(_fpsTimer.GetTick());
			if(CheckEvents()){
				return 1;
			}
			break;
		}
	}
	return 0;
}
void EngineC::Clean(){
	levelInfo.Clean();
	DeleteButtonArray();
	GraphicsCl* realGraphics = (GraphicsCl*)mGraphics;
	realGraphics->Clean();
	mGraphics			= nullptr;
	mEngine				= nullptr;
	mRun 				= 0;

	for(int i = 0; i < editorSize; i++){
		mEditors[i].Clean();
	}
	mTexts.Clean();

}

void EngineC::InitilizeTexts(){
	GraphicsCl* realGraphics = (GraphicsCl*)mGraphics;
	mTexts.LFT("F1 - Export, F2 - Import, F3 - Toggle tile grid, F4 - Toggle obj grid, F5 - Toggle tile hitboxes, F6 - Toggle obj hitboxes, F7 - Load backup, F9 - Reload",
				realGraphics->frame.render, realGraphics->getColor("White"), realGraphics->getFont("small") ,1280);
}
void EngineC::LoadSprites(std::string dir_In, Collision_Data** collision_Out, int* loadedPngs_Out){
	// @TODO
	int _dirSize;
	WinAPI::GetFileCountDirectory(dir_In + "*.png", &_dirSize);
	if((*collision_Out) != nullptr && (*loadedPngs_Out) > 0){
		Collision_Data* tempData = new Collision_Data[(*loadedPngs_Out)];
		for(int i = 0; i < (*loadedPngs_Out); i++)
			tempData[i] = (*collision_Out)[i];
		delete [] (*collision_Out);
		(*collision_Out) 		= new Collision_Data[_dirSize];
		if((*loadedPngs_Out) < _dirSize)
			for(int i = 0; i < (*loadedPngs_Out); i++)
				(*collision_Out)[i] = tempData[i];
		else
			for(int i = 0; i < _dirSize; i++)
				(*collision_Out)[i] = tempData[i];
	}else
		(*collision_Out) 		= new Collision_Data[_dirSize];
	*loadedPngs_Out = _dirSize;
}
void EngineC::LoadCollisionData(std::string dir_In, Collision_Data** collision_Out, Selection::SELECTION_TYPES type_In){
	int _dirSize;
	WinAPI::GetFileCountDirectory(dir_In + "*.info", &_dirSize);
	int* 			types 		= new int[_dirSize];
	std::string* 	filenames 	= new std::string[_dirSize];
	WinAPI::GetFileNamesDirectory(dir_In + "*.info", &filenames);
	IOStream::getIntFromString(&filenames, _dirSize, &types);
	for(int i = 0; i < _dirSize; i++)
		IOStream::LoadCollisionInfo(types[i], (void*)&type_In, collision_Out);
	delete [] types;
	delete [] filenames;
}

void EngineC::InitilizeButtons(){
	GraphicsCl* realGraphics = (GraphicsCl*)mGraphics;
	int defaultBtns;
	int _xIndex;
	int _yIndex;
	int _startHeight;
	int _Obj;
	DeleteButtonArray();
	defaultBtns = 6;
	mTotalButtons = defaultBtns + levelInfo.tilesLoaded + levelInfo.objectsLoaded;
	AllocateButtonArray();
	mButtons[0].Init(0 ,0 ,96 ,40 ,"Objects",realGraphics->frame.render,realGraphics->getTexture(0,0),realGraphics->getFont("medium"),realGraphics->getColor("Black"),0,NULL);
	mButtons[1].Init(96 ,0 ,96 ,40 ,"Tiles",realGraphics->frame.render,realGraphics->getTexture(0,0),realGraphics->getFont("medium"),realGraphics->getColor("Black"),0,NULL);
	mButtons[2].Init(0 ,0 ,96 ,40 ,"Export",realGraphics->frame.render,realGraphics->getTexture(0,0),realGraphics->getFont("medium"),realGraphics->getColor("Black"),0,NULL);
	mButtons[3].Init(96 ,0 ,96 ,40 ,"Import",realGraphics->frame.render,realGraphics->getTexture(0,0),realGraphics->getFont("medium"),realGraphics->getColor("Black"),0,NULL);
	mButtons[4].Init(192 ,0 ,96 ,40 ,"Add Box",realGraphics->frame.render,realGraphics->getTexture(0,0),realGraphics->getFont("medium"),realGraphics->getColor("Black"),0,NULL);
	mButtons[5].Init(288 ,0 ,96 ,40 ,"GG",realGraphics->frame.render,realGraphics->getTexture(0,0),realGraphics->getFont("medium"),realGraphics->getColor("Black"),0,NULL);
	mTilesStartRender.x = defaultBtns;

	_xIndex 		= 0;
	_yIndex 		= 0;
	_startHeight 	= 40;

	for(int i = 0; i < levelInfo.tilesLoaded; i++){
		if(_xIndex >= 3){
			_xIndex = 0;
			_yIndex++;
		}
		mButtons[i + defaultBtns].Init(levelInfo.tileSize * _xIndex, _startHeight + levelInfo.tileSize * _yIndex, levelInfo.tileSize, realGraphics->getTexture(1,i));
		_xIndex++;
	}
	mTilesStartRender.y = defaultBtns + levelInfo.tilesLoaded;
	mObjectsStartRender.x = mTilesStartRender.y;
	mObjectsStartRender.y = defaultBtns + levelInfo.tilesLoaded + levelInfo.objectsLoaded;

	_xIndex			= 0;
	_yIndex			= 0;
	_Obj			= mObjectsStartRender.x;
	for(int i=mObjectsStartRender.x;i<mObjectsStartRender.y;i++){
		mButtons[i].Init(0,40,0,0,"Objects",realGraphics->frame.render,realGraphics->getTexture(2,i - mObjectsStartRender.x),realGraphics->getFont("medium"),realGraphics->getColor("Black"),2);
	}
	while(_Obj != mObjectsStartRender.y+1){
		if(_xIndex + mButtons[_Obj].getWidth() > realGraphics->getTexture(3,0)->GetWidth()){
			_startHeight += _yIndex;
			_yIndex		= 0;
			if(_yIndex<mButtons[_Obj].getHeight()){
				_yIndex = mButtons[_Obj].getHeight();
			}
			_xIndex = 0;
		}else{
			if(_yIndex<mButtons[_Obj].getHeight()){
				_yIndex = mButtons[_Obj].getHeight();
			}
		}
		mButtons[_Obj].SetPos(_xIndex,_startHeight);
		_xIndex += mButtons[_Obj].getWidth();
		_Obj++;
	}
	objH = _startHeight;
}
void EngineC::MoveCamera(){
	SDL_Point newLocation = {mCamera.GetLocation().x + mMouse.location_old.x - mMouse.location.x, mCamera.GetLocation().y + mMouse.location_old.y - mMouse.location.y};
	mMouse.location_old = {mMouse.location.x, mMouse.location.y};
	mCamera.SetLocation(newLocation);
}
void EngineC::UpdateScroll(){
	if(mMouse.scroll != 0){
		if(mMouse.location.x < 192){
			mMouse.offset.x += mMouse.scroll * -30;
			if(mMouse.offset.x < 0){
				mMouse.offset.x = 0;
			}else if (mMouse.offset.x > mMouse.offset.y){
				mMouse.offset.x = mMouse.offset.y;
			}
		}else{
			if(selection.what != selection.NOTHING){
				mMouse.scroll_tot += mMouse.scroll * 0.1;
				if(mMouse.scroll_tot > 2.0)
					mMouse.scroll_tot = 2.0;
				else if (mMouse.scroll_tot < 0.1)
					mMouse.scroll_tot = 0.1;
				else{
					/*
					 * Move camera to the mouse position.
					 */
				}
				levelInfo.zoom = mMouse.scroll_tot;
			}
		}
	}
}
int EngineC::CheckEvents(){
	/*
	 * Reset the mouse structures variables.
	 */
	mMouse.scroll 	= 0;
	mMouse.moved 	= 0;

	if(mMouse.m1 == CLICKED)
		mMouse.m1 = HELD;
	if(mMouse.m2 == CLICKED)
		mMouse.m2 = HELD;
	if(mMouse.m3 == CLICKED)
		mMouse.m3 = HELD;

	if(mMouse.m1 == RELEASED)
		mMouse.m1 = NONE;
	if(mMouse.m2 == RELEASED)
		mMouse.m2 = NONE;
	if(mMouse.m3 == RELEASED)
		mMouse.m3 = NONE;

	while(SDL_PollEvent(&mEvent)){
		if(mEvent.type==SDL_QUIT){
			Clean();
			return 1;
			break;
		}
		/*
		 * Update the mouse struct.
		 */
		SDL_GetMouseState(&mMouse.location.x, &mMouse.location.y);
		if(mEvent.type == SDL_MOUSEMOTION)
			mMouse.moved = 1;
		if(mEvent.type == SDL_MOUSEWHEEL)
			mMouse.scroll += mEvent.wheel.y;
		if(mEvent.type == SDL_MOUSEBUTTONDOWN){
			switch(mEvent.button.button){
			case SDL_BUTTON_LEFT:
				mMouse.m1 = CLICKED;
				break;
			case SDL_BUTTON_MIDDLE:
				mMouse.m3 = CLICKED;
				break;
			case SDL_BUTTON_RIGHT:
				mMouse.m2 = CLICKED;
				break;
			}
		}
		if(mEvent.type == SDL_MOUSEBUTTONUP){
			switch(mEvent.button.button){
			case SDL_BUTTON_LEFT:
				mMouse.m1 = RELEASED;
				break;
			case SDL_BUTTON_MIDDLE:
				mMouse.m3 = RELEASED;
				break;
			case SDL_BUTTON_RIGHT:
				mMouse.m2 = RELEASED;
				break;
			}
		}

		/*
		 * Check for keyboard input
		 */
		if(mEvent.type == SDL_KEYUP){
			switch(mEvent.key.keysym.sym){
			case SDLK_LCTRL:
				mKeyboard.ctrl = 0;
				break;
			case SDLK_LALT:
				mKeyboard.alt = 0;
				brush.mode = DRAW;
				break;
			case SDLK_LSHIFT:
				mKeyboard.shift = 0;
				break;
			case SDLK_b:
				brush.mode = DRAW;
				break;
			case SDLK_e:
				brush.mode = ERASE;
				break;
			case SDLK_f:
				brush.mode = FILL;
				break;
			case SDLK_s:
				brush.mode = SELECTION;
				break;
			case SDLK_LEFTBRACKET:
				//@TODO
				brush.brushSize--;
				if(brush.brushSize <= 0){
					brush.brushSize = 1;
				}else{
					brush.UpdateBrushLines();
					std::cout << "brushSize: " << brush.brushSize << std::endl;
				}
				break;
			case SDLK_RIGHTBRACKET:
				brush.brushSize++;
				if(brush.brushSize >= 13){
					brush.brushSize = 12;
				}else{
					brush.UpdateBrushLines();
					std::cout << "brushSize: " << brush.brushSize << std::endl;
				}
				break;
			case SDLK_F1:
				IOStream::Export("./Data/exported_level.map", (void*)&levelInfo);
				break;
			case SDLK_F2:
				IOStream::Import("./Data/imported_level.map", (void*)&levelInfo, mGraphics);
				break;
			case SDLK_F3:
				selection.tileGridEnabled = !selection.tileGridEnabled;
				if(selection.tileGridEnabled)
					std::cout << "Tile grid enabled." << std::endl;
				else
					std::cout << "Tile grid disabled."<< std::endl;
				break;
			case SDLK_F4:
				selection.objGridEnabled = !selection.objGridEnabled;
				if(selection.objGridEnabled)
					std::cout << "Object grid enabled." << std::endl;
				else
					std::cout << "Object grid disabled."<< std::endl;
				break;
			case SDLK_F5:
				selection.tileCollisionEnabled = !selection.tileCollisionEnabled;
				if(selection.tileCollisionEnabled)
					std::cout << "Tile collision box enabled." << std::endl;
				else
					std::cout << "Tile collision box disabled."<< std::endl;
				break;
			case SDLK_F6:
				selection.objCollisionEnabled = !selection.objCollisionEnabled;
				if(selection.objCollisionEnabled)
					std::cout << "Object collision box enabled." << std::endl;
				else
					std::cout << "Object collision box disabled."<< std::endl;
				break;
			case SDLK_F7:
				IOStream::Import(".//Data//Backups//" + WinAPI::GetLatestCreatedFile(".//Data//Backups//*.map"), (void*)&levelInfo, mGraphics);
				break;
			case SDLK_F9:
				/*
				 * Find all of the tile *.info files in "./Data/Tiles" location.
				 */
				LoadSprites("./Data/Tiles/", &mTilesCollision, &levelInfo.tilesLoaded);
				LoadCollisionData("./Data/Tiles/", &mTilesCollision, Selection::TILE);
				/*
				 * Find all of the object *.info files in "./Data/Objects" location.
				 */
				LoadSprites("./Data/Objects/", &mObjectsCollision, &levelInfo.objectsLoaded);
				LoadCollisionData("./Data/Objects/", &mObjectsCollision, Selection::OBJECT);
				GraphicsCl* _g = (GraphicsCl*) mGraphics;
				_g->LoadTiles();
				_g->LoadObjects();
				this->iState = ENGINE_STATES::INIT;
				break;
			}
		}
		if(mEvent.type == SDL_KEYDOWN){
			switch(mEvent.key.keysym.sym){
			case SDLK_LCTRL:
				mKeyboard.ctrl = 1;
				break;
			case SDLK_LALT:
				mKeyboard.alt = 1;
				break;
			case SDLK_LSHIFT:
				mKeyboard.shift = 1;
				break;
			}
		}
		if(iState == COLLISIONEDITING){
			for(int i = 0; i < editorSize; i++){
				mEditors[i].UpdateEvents(&mEvent, &mMouse);
			}
		}
	}

	if(mKeyboard.alt == 1)
		brush.mode = SELECTION;

	return 0;
}

void EngineC::OnClick(int whatButton){
	SDL_Point tempMouseLoc {mMouse.location.x - 192 + mCamera.GetLocation().x, mMouse.location.y + mCamera.GetLocation().y};
	if(mMouse.location.x > 192){
		switch(whatButton){
		case 1:
			if(selection.what == selection.TILE && brush.mode == SELECTION)
				PickTile(tempMouseLoc);
			else if (selection.what == selection.TILE && brush.mode == FILL)
				FillTiles(tempMouseLoc);
			else if(selection.what == selection.TILE) // If an Tile is selected;
				PlaceTile(tempMouseLoc);
			else if(selection.what == selection.OBJECT && brush.mode == SELECTION)
				PickObject(tempMouseLoc);
			else if (selection.what == selection.OBJECT) // If an Object is selected
				PlaceObject(tempMouseLoc);
			break;
		case 2:
			if (selection.what == selection.OBJECT) // If an Object is selected
				DestroyObject(tempMouseLoc);
			break;
		}
	}
}
void EngineC::StartMovingObject(SDL_Point _mouseLocation){
	levelInfo.SetMovingObject(CheckCollisionWithObjectsCentered(_mouseLocation.x,_mouseLocation.y, (void*)(&levelInfo)));
}
void	EngineC::FillTiles(SDL_Point _mouse_In){
	_mouse_In.x 	= floor(_mouse_In.x / levelInfo.GetTileScaled());
	_mouse_In.y 	= floor(_mouse_In.y / levelInfo.GetTileScaled());
	levelInfo.FillArea(selection.type, _mouse_In.x, _mouse_In.y);
}
void EngineC::PickTile(SDL_Point _mouseLocation){
	SDL_Point mNewALoc;
	float _tile = levelInfo.GetTileScaled();
	mNewALoc.x = floor(_mouseLocation.x / _tile);
	mNewALoc.y = floor(_mouseLocation.y / _tile);
	levelInfo.GetTile(&selection.type, mNewALoc.x, mNewALoc.y);
	brush.mode 		= DRAW;
}
void EngineC::PlaceTile(SDL_Point _mouseLocation){
	int _tile = levelInfo.GetTileFloored();
	SDL_Point mNewALoc = {(int)floor(_mouseLocation.x / _tile) + (brush.brushSize >> 1) + 1, (int)floor(_mouseLocation.y / _tile) + (brush.brushSize >> 1) + 1};
	levelInfo.SetLevelSize(mNewALoc.x, mNewALoc.y, -1);
	mNewALoc = {mNewALoc.x - ((brush.brushSize >> 1) + 1), mNewALoc.y - ((brush.brushSize >> 1) + 1)};
	brush.FillTiles(mNewALoc.x, mNewALoc.y, selection.type, &levelInfo);
}
void EngineC::PickObject(SDL_Point tempMouseLoc){
	ObjectC* newObject = nullptr;
	levelInfo.GetObjects(newObject, CheckCollisionWithObjectsCentered(tempMouseLoc.x ,tempMouseLoc.y, (void*)(&levelInfo)));
	if(newObject != nullptr){
		selection.type 	= newObject->getType();
		brush.mode 		= DRAW;
	}
}
void EngineC::PlaceObject(SDL_Point _mouseLocation){
	//@TODO Fix being able to place multiple objects of the same type, in a single place.
	GraphicsCl* _g = (GraphicsCl*) mGraphics;
	ObjectC		_obj;
	int itmp 	= -1;

	if((itmp = CheckCollisionWithObjectsCentered(_mouseLocation.x ,_mouseLocation.y, (void*)(&levelInfo))) != -1){
		levelInfo.GetObjects(&_obj, itmp);
		if(_obj.getType() == selection.type)
			levelInfo.SetMovingObject(itmp);
	}else{
		SDL_Rect box = {	(int)floor(_mouseLocation.x / levelInfo.GetGridFloored()),
							(int)floor(_mouseLocation.y / levelInfo.GetGridFloored()),
							_g->getTexture(2,selection.type)->GetWidth(),
							_g->getTexture(2,selection.type)->GetHeight()	};
		levelInfo.SetLevelSize(-1,-1,(1 + levelInfo.objectsPlaced));
		levelInfo.InitObject((levelInfo.objectsPlaced - 1), box, selection.type);
	}
}
void EngineC::DestroyObject(SDL_Point _mouseLocation){
	if(levelInfo.objectsPlaced)
		levelInfo.DestroyObject(CheckCollisionWithObjectsCentered(_mouseLocation.x,_mouseLocation.y, (void*)(&levelInfo)));
}
void EngineC::AllocateButtonArray(){
	if(mButtons == nullptr){
		mButtons = new DefaultButtonClass[mTotalButtons+1];
	}else{
		DeleteButtonArray();
		AllocateButtonArray();
	}
}
void EngineC::DeleteButtonArray(){
	if(mButtons != nullptr){
		for(int i=0;i<mTotalButtons;i++){
			mButtons[i].Clean();
		}
		delete [] mButtons;
		mButtons = nullptr;
	}
}

/*
 *
 * GETTERS
 *
 */

int EngineC::getTotButtons(){
	return mTotalButtons;
}
DefaultButtonClass* EngineC::getButton(int x){
	return &mButtons[x];
}
SDL_Point EngineC::renderObjectsPoints(){
	return mObjectsStartRender;
}
SDL_Point	EngineC::getCameraPosition(){
	return	mCamera.GetLocation();
}
SDL_Point EngineC::renderTilesPoints(){
	return mTilesStartRender;
}
Selection EngineC::getSelection(){
	return selection;
}
str_Mouse EngineC::getMouseStr(){
	return mMouse;
}
int EngineC::getScrollOffset(){
	return mMouse.offset.x;
}
float EngineC::getZoom(){
	return levelInfo.zoom;
}
RectEditorCl* EngineC::getEditor(int id){
	if(id < 5)
		return &mEditors[id];
	else
		return nullptr;
}
ENGINE_STATES EngineC::getState(){
	return iState;
}
int EngineC::getEditorCount(){
	return editorSize;
}
TextC* EngineC::getText(){
	return &mTexts;
}
Collision_Data	EngineC::getCollisionData(std::string string_In, int type_In){
	if(string_In == "tile"){
		return mTilesCollision[type_In];
	}else if (string_In == "object"){
		return mObjectsCollision[type_In];
	}else{
		return Collision_Data {};
	}
}
Level_Info EngineC::GetLevelInfo(){
	return levelInfo;
}
Brush EngineC::GetBrush(){
	return brush;
}
