/*
 * MainGraphics.cpp
 *
 *  Created on: Sep 10, 2016
 *      Author: Bunjukz
 */



#include <Graphics/MainGraphics.h>
#include <Engine/MainEngine.h>
#include <Camera/MainCamera.h>

void GraphicsCl::Clean(){
	frame.DeleteWindow();
	frame.DeleteRenderer();
	EngineC* _Engine = (EngineC*) mpEngine;
	for(int i=0;i<_Engine->GetLevelInfo().objectsLoaded;i++){
		mObjectTextures[i].Clean();
	}
	for(int i = 0 ; i < _Engine->GetLevelInfo().tilesLoaded; i++){
		mTileTextures[i].Clean();
	}
	delete [] mTileTextures;
	delete [] mObjectTextures;
	mSpriteHolder.Clean();
	mButtonSheet.Clean();
	TTF_CloseFont(smallFont);
	TTF_CloseFont(mediumFont);
	TTF_CloseFont(largeFont);
	mRun = 0;
}
void GraphicsCl::Init(void* pEngine){
	mRun 			= 1;
	mpEngine		= pEngine;
	mTileTextures 	= nullptr;
	mObjectTextures = nullptr;
}
void GraphicsCl::Update(SDL_Point camera, void* pEngine){
	EngineC* mEngine = (EngineC*)mpEngine;
	static SDL_Point tileRenderX;
	static SDL_Point tileRenderY;
	int		WINDOW_W;
	int		WINDOW_H;
	int 	spriteHolderSize 	= mSpriteHolder.GetWidth();
	float 	tileSize = mEngine->GetLevelInfo().GetTileScaled();
	float	gridSize = mEngine->GetLevelInfo().GetGridScaled();
	frame.GetDimensions(&WINDOW_W, &WINDOW_H);
	SDL_RenderClear(frame.render);
	switch(mEngine->getState()){
	case INIT:
		break;
	case LEVELEDITING:
		// Render tile grid (If it is enabled.)
		if(mEngine->getSelection().tileGridEnabled && mEngine->getSelection().what == Selection::TILE){
			for(int y = 0; y < mEngine->GetLevelInfo().height + 1; y++){
				SDL_SetRenderDrawColor(frame.render, 50,50,50,255);
				SDL_RenderDrawLine(frame.render, 	spriteHolderSize - camera.x,
												y * floor(tileSize) - camera.y,
												mEngine->GetLevelInfo().width * floor(tileSize) + spriteHolderSize - camera.x,
												y * floor(tileSize) - camera.y);
				SDL_SetRenderDrawColor(frame.render, 0,0,0,255);
			}
			for(int x = 0; x < mEngine->GetLevelInfo().width + 1; x++){
				SDL_SetRenderDrawColor(frame.render, 50,50,50,255);
				SDL_RenderDrawLine(frame.render, 	x * floor(tileSize) + spriteHolderSize - camera.x,
												0 - camera.y,
												x * floor(tileSize) + spriteHolderSize - camera.x,
												mEngine->GetLevelInfo().height * floor(tileSize) - camera.y);
				SDL_SetRenderDrawColor(frame.render, 0,0,0,255);
			}
		}

		// Get what tiles are on the screen.
		tileRenderX.x = floor((camera.x - spriteHolderSize) / tileSize) - 1;
		tileRenderX.y = ceil((camera.x + (int)WINDOW_W - spriteHolderSize) / tileSize) + 10;
		tileRenderY.x = floor(camera.y / tileSize) - 1;
		tileRenderY.y = ceil((camera.y + (int)WINDOW_H) / tileSize) + 10;
		if(tileRenderX.x < 0)
			tileRenderX.x = 0;
		if(tileRenderX.y > mEngine->GetLevelInfo().width)
			tileRenderX.y = mEngine->GetLevelInfo().width;
		if(tileRenderY.x < 0)
			tileRenderY.x = 0;
		if(tileRenderY.y > mEngine->GetLevelInfo().height)
			tileRenderY.y = mEngine->GetLevelInfo().height;
		// Render all of the tiles.
		int tile;
		for(int x = tileRenderX.x; x < tileRenderX.y; x++){
			for(int y = tileRenderY.x; y < tileRenderY.y; y++){
				mEngine->GetLevelInfo().GetTile(&tile, x, y);
				if(tile >= 0){
					mTileTextures[tile].RenderZoom(x * floor(tileSize) + spriteHolderSize - camera.x ,y * floor(tileSize) - camera.y,frame.render, mEngine->getZoom());
					if(mEngine->getCollisionData("tile", tile).boxes > 0 && mEngine->getSelection().tileCollisionEnabled){
						SDL_SetRenderDrawColor(frame.render, 0,255,0,255);
						for(int i = 0; i < mEngine->getCollisionData("tile", tile).boxes; i++){
							SDL_Rect _tmp = mEngine->getCollisionData("tile", tile).box[i];
							_tmp.x += x * floor(tileSize) + spriteHolderSize - camera.x;
							_tmp.y += y * floor(tileSize) - camera.y;
							_tmp.w *= mEngine->getZoom();
							_tmp.h *= mEngine->getZoom();
							SDL_RenderDrawRect(frame.render,&_tmp);
						}
						SDL_SetRenderDrawColor(frame.render, 0,0,0,255);
					}
				}
			}
		}
		// Render the object grid (if its enabled.)
		if (mEngine->getSelection().objGridEnabled && mEngine->getSelection().what == Selection::OBJECT){
			for(int y = 0; y < mEngine->GetLevelInfo().height * floor(tileSize / gridSize) + 1; y++){
				SDL_SetRenderDrawColor(frame.render, 50,50,50,255);
				SDL_RenderDrawLine(frame.render, 	spriteHolderSize - camera.x,
												y * floor(gridSize) - camera.y,
												(mEngine->GetLevelInfo().width * floor(tileSize / gridSize)) * floor(gridSize) + spriteHolderSize - camera.x,
												y * floor(gridSize) - camera.y);
				SDL_SetRenderDrawColor(frame.render, 0,0,0,255);
			}
			for(int x = 0; x < mEngine->GetLevelInfo().width * floor(tileSize / gridSize) + 1; x++){
				SDL_SetRenderDrawColor(frame.render, 50,50,50,255);
				SDL_RenderDrawLine(frame.render, 	x * floor(gridSize) + spriteHolderSize - camera.x,
												0 - camera.y,
												x * floor(gridSize) + spriteHolderSize - camera.x,
												(mEngine->GetLevelInfo().height * floor(tileSize / gridSize)) * floor(gridSize) - camera.y);
				SDL_SetRenderDrawColor(frame.render, 0,0,0,255);
			}
		}

		// Render all of the objects.
		for(int i=0;i<mEngine->GetLevelInfo().objectsPlaced;i++){
			ObjectC _obj;
			mEngine->GetLevelInfo().GetObjects(&_obj, i);
			mObjectTextures[_obj.getType()].RenderZoomCentered(		_obj.getCollisionBox().x * floor(gridSize) + spriteHolderSize - camera.x ,
																	_obj.getCollisionBox().y * floor(gridSize) - camera.y,
																	frame.render, mEngine->getZoom()	);
			if(mEngine->getCollisionData("object", _obj.getType()).boxes > 0 && mEngine->getSelection().objCollisionEnabled){
				SDL_SetRenderDrawColor(frame.render, 0,255,0,255);
				for(int z = 0; z < mEngine->getCollisionData("object", _obj.getType()).boxes; z++){
					SDL_Rect _tmp = mEngine->getCollisionData("object", _obj.getType()).box[z];
					_tmp.x += _obj.getCollisionBox().x * floor(gridSize) + spriteHolderSize - camera.x - floor(_tmp.w * mEngine->getZoom() * 0.5);
					_tmp.y += _obj.getCollisionBox().y * floor(gridSize) - camera.y - floor(_tmp.h * mEngine->getZoom() * 0.5);
					_tmp.w *= mEngine->getZoom();
					_tmp.h *= mEngine->getZoom();
					SDL_RenderDrawRect(frame.render,&_tmp);
				}
				SDL_SetRenderDrawColor(frame.render, 0,0,0,255);
			}
		}

		/*
		 * Render the brush outline.
		 */
		if(mEngine->getSelection().what == Selection::TILE && (mEngine->GetBrush().mode == DRAW || mEngine->GetBrush().mode == ERASE)){
			std::vector<Line> brushLines;
			mEngine->GetBrush().getBrushLines(&brushLines);
			SDL_SetRenderDrawColor(frame.render, 255,255,255,255);
			int brushSize = (mEngine->GetBrush().brushSize >> 1);
			for(unsigned int i = 0; i < brushLines.size(); i++){
				SDL_RenderDrawLine(frame.render, 	(floor((mEngine->getMouseStr().location.x + camera.x - spriteHolderSize) / floor(tileSize)) - brushLines[i].line[0].x + brushSize + 1) * floor(tileSize) - camera.x + spriteHolderSize,
												(floor((mEngine->getMouseStr().location.y + camera.y) / floor(tileSize)) - brushLines[i].line[0].y + brushSize + 1) * floor(tileSize) - camera.y,
												(floor((mEngine->getMouseStr().location.x + camera.x - spriteHolderSize) / floor(tileSize)) - brushLines[i].line[1].x + brushSize + 1) * floor(tileSize) - camera.x + spriteHolderSize,
												(floor((mEngine->getMouseStr().location.y + camera.y) / floor(tileSize)) - brushLines[i].line[1].y + brushSize + 1) * floor(tileSize) - camera.y);
			}
			SDL_SetRenderDrawColor(frame.render, 0,0,0,255);
		}


		mEngine->getText()->Render(spriteHolderSize, WINDOW_H - mEngine->getText()->GetHeight(), frame.render);

		mSpriteHolder.Render(0,0,spriteHolderSize,WINDOW_H,NULL,frame.render);

		if(mEngine->getSelection().what == Selection::TILE){
			for(int i=mEngine->renderTilesPoints().x;i<mEngine->renderTilesPoints().y;i++){
				mEngine->getButton(i)->Render(frame.render,mEngine->getScrollOffset());
			}
		}else if(mEngine->getSelection().what == Selection::OBJECT){
			for(int i=mEngine->renderObjectsPoints().x;i<mEngine->renderObjectsPoints().y;i++){
				mEngine->getButton(i)->Render(frame.render,mEngine->getScrollOffset());
			}
		}
		for(int i = 0; i < 2; i++){
			mEngine->getButton(i)->Render(frame.render,mEngine->getScrollOffset());
		}

		if (mEngine->getSelection().what == Selection::TILE && mEngine->GetBrush().mode == FILL){
			SDL_ShowCursor(SDL_DISABLE);
			mFillCursor.Render(mEngine->getMouseStr().location.x, mEngine->getMouseStr().location.y - 27, 32, 27, nullptr, frame.render);
		}else if (mEngine->getSelection().what == Selection::TILE && mEngine->GetBrush().mode == SELECTION){
			SDL_ShowCursor(SDL_DISABLE);
			mSelectionCursor.Render(mEngine->getMouseStr().location.x, mEngine->getMouseStr().location.y - 32, 32, 32, nullptr, frame.render);
		}else{
			if(!(SDL_ShowCursor(SDL_QUERY))){
				SDL_ShowCursor(SDL_ENABLE);
			}
		}

		break;
	case COLLISIONEDITING:
		//SDL_SetRenderDrawColor(frame.render, rand() %256,rand() %256,rand() %256,255);
		if(mEngine->getSelection().what == Selection::TILE){
			mTileTextures[mEngine->getSelection().type].RenderZoom(WINDOW_W * 0.5  - (floor(64 * 10) * 0.5), WINDOW_H * 0.5 - (floor(64 * 10) * 0.5) ,frame.render, 10);
		}else{
			mObjectTextures[mEngine->getSelection().type].RenderZoom(WINDOW_W * 0.5 - (floor(mObjectTextures[mEngine->getSelection().type].GetWidth() * 10) * 0.5), WINDOW_H * 0.5 - (floor(mObjectTextures[mEngine->getSelection().type].GetHeight() * 10) * 0.5) ,frame.render, 10);
		}
		SDL_SetRenderDrawColor(frame.render, 255,255,255,255);
		for(int i = 0 ; i < mEngine->getEditorCount(); i++){
			mEngine->getEditor(i)->Render(frame.render);
			SDL_Rect _tmp;
			_tmp = mEngine->getEditor(i)->getBox();
			if(mEngine->getSelection().what == Selection::TILE){
				_tmp.x = (_tmp.x * 10) + WINDOW_W * 0.5 - (floor(64 * 10) * 0.5);
				_tmp.y = (_tmp.y * 10) + (WINDOW_H * 0.5 - (floor(64 * 10) * 0.5));
			}else{
				_tmp.x = (_tmp.x * 10) + WINDOW_W * 0.5 - (floor(mObjectTextures[mEngine->getSelection().type].GetWidth() * 10) * 0.5);
				_tmp.y = (_tmp.y * 10) + (WINDOW_H * 0.5 - (floor(mObjectTextures[mEngine->getSelection().type].GetHeight() * 10) * 0.5));
			}
			_tmp.w = _tmp.w * 10;
			_tmp.h = _tmp.h * 10;
			SDL_RenderDrawRect(frame.render, &_tmp);
		}
		SDL_SetRenderDrawColor(frame.render, 0,0,0,255);
		for(int i = 2; i < mEngine->renderTilesPoints().x; i++){
			mEngine->getButton(i)->Render(frame.render,mEngine->getScrollOffset());
		}
		break;
	}
	SDL_RenderPresent(frame.render);
}
void GraphicsCl::LoadTiles(){
	EngineC* _Engine = (EngineC*) mpEngine;
	if(mTileTextures != nullptr)
		delete [] mTileTextures;
	mTileTextures 	= new TextureC[_Engine->GetLevelInfo().tilesLoaded];
	// Load tiles
	for(int i = 0; i < _Engine->GetLevelInfo().tilesLoaded; i++){
		std::stringstream streamTemp;
		streamTemp << "./Data/Tiles/" << i << ".png";
		mTileTextures[i].LFF(streamTemp.str().c_str(), frame.render);
	}
}
void GraphicsCl::LoadObjects(){
	EngineC* _Engine = (EngineC*) mpEngine;
	if(mObjectTextures != nullptr)
		delete [] mObjectTextures;
	mObjectTextures 	= new TextureC[_Engine->GetLevelInfo().objectsLoaded];
	// Load Objects
	for(int i = 0; i < _Engine->GetLevelInfo().objectsLoaded; i++){
		std::stringstream streamTemp;
		streamTemp << "./Data/Objects/Object_" << i << ".png";
		mObjectTextures[i].LFF(streamTemp.str().c_str(), frame.render);
	}
}
void GraphicsCl::LoadMedia(){
	mColors[0] = {0, 0, 0, 255};
	mColors[1] = {255, 255, 255, 255};
	LoadTiles();
	LoadObjects();
	mButtonSheet.LFF("./Data/Button/DefaultButton.png",frame.render);
	mSpriteHolder.LFF("Data/SpriteHolder.png",frame.render);
	mRectEditorBG.LFF("./Data/RectEditor.png", frame.render);
	mFillCursor.LFF("./Data/FillCursor.png", frame.render);
	mSelectionCursor.LFF("./Data/SelectionCursor.png", frame.render);
	textBG.LFF("./Data/TextBG.png", frame.render);
	smallFont 	= TTF_OpenFont("Data/Fonts/Roboto.ttf",12);
	mediumFont 	= TTF_OpenFont("Data/Fonts/Roboto.ttf",18);
	largeFont 	= TTF_OpenFont("Data/Fonts/Roboto.ttf",24);
}
TTF_Font* GraphicsCl::getFont(std::string _string){
	if(_string == "small"){
		return smallFont;
	}else if (_string == "medium")
		return mediumFont;
	else if (_string == "large")
		return largeFont;
	return nullptr;
}
TextureC* GraphicsCl::getTexture(int ID,int type){
	switch (ID){
	case 0:
		return &mButtonSheet;
		break;
	case 1:
		return &mTileTextures[type];
		break;
	case 2:
		return &mObjectTextures[type];
		break;
	case 3:
		return &mSpriteHolder;
		break;
	case 4:
		return &mRectEditorBG;
		break;
	case 5:
		return &textBG;
		break;
	}
	printf("Texture not found, returning nullptr!\n");
	return mObjectTextures;
}
SDL_Color GraphicsCl::getColor(std::string whatColor){
	if(whatColor == "Black"){
		return mColors[0];
	}else if(whatColor == "White"){
		return mColors[1];
	}else{
		printf("Couldn't find the color : %s , returning color Black!\n", whatColor.c_str());
		return mColors[0];
	}
}
