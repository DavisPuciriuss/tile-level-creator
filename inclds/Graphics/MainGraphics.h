/*
 * MainGraphics.h
 *
 *  Created on: Sep 10, 2016
 *      Author: Bunjukz
 */

#ifndef INCLDS_GRAPHICS_MAINGRAPHICS_H_
#define INCLDS_GRAPHICS_MAINGRAPHICS_H_

#include <stdio.h>
#include <SDL_image.h>
#include <SDL_TTF.h>
#include <Graphics/Texture/Texture.h>
#include <Object/MainObject.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <Graphics/Window/SDL_WindowClass.h>


class GraphicsCl {
public:
	void Init(void*);
	void Clean();
	void Update(SDL_Point camera_In,void*);
	void LoadMedia();
	void LoadTiles();
	void LoadObjects();
	TTF_Font*		getFont(std::string);
	TextureC*		getTexture(int,int);
	SDL_Color		getColor(std::string);
	SDL_WindowClass frame;
private:
	int mRun;
	TextureC*		mObjectTextures;
	TextureC*		mTileTextures;
	TextureC		mSpriteHolder;
	TextureC		mRectEditorBG;
	TextureC		textBG;
	TextureC		mButtonSheet;
	TextureC		mFillCursor;
	TextureC		mSelectionCursor;
	TTF_Font*		largeFont;
	TTF_Font*		mediumFont;
	TTF_Font*		smallFont;
	SDL_Color		mColors[2];
	void*			mpEngine;
};



#endif /* INCLDS_GRAPHICS_MAINGRAPHICS_H_ */
