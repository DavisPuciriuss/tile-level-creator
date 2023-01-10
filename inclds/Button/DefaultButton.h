/*
 * DefaultButton.h
 *
 *  Created on: Aug 29, 2016
 *      Author: Bunjukz
 */

#ifndef INCLDS_BUTTON_DEFAULTBUTTON_H_
#define INCLDS_BUTTON_DEFAULTBUTTON_H_

#include <Graphics/Texture/Texture.h>
#include <Text/Texture/TextTexture.h>
#include <Button/DefaultButton_defines.h>
#include <Inputs/Inputs.h>

class DefaultButtonClass {
public:
	int Init(int,int,int,int,std::string,SDL_Renderer*,TextureC*,TTF_Font*,SDL_Color,int,SDL_Rect* _rect = NULL);
	void Init(SDL_Rect, std::string, SDL_Renderer*, TextureC*, TTF_Font*, SDL_Color);
	void Init(int, int, int, TextureC*);
	int Update(SDL_Event*,int,int);
	int Update(str_Mouse*);
	int GetPosX();
	int GetPosY();
	int getWidth();
	int getHeight();
	void Render(SDL_Renderer*,int);
	void SetPos(int,int);
	void Clean();
	void setType(int);
private:
	void SetSDLRect();
	int			mPosX,
				mPosY,
				mWidth,
				mHeight,
				mCurSprite,
				mType,
				customButton;
	SDL_Rect 	mRect[4];
	SDL_Rect	mRectTile;
	TextureC* 	mSprite;
	TextC		mTextTexture;
	SDL_Point	mTextRenderPos;
};



#endif /* INCLDS_BUTTON_DEFAULTBUTTON_H_ */
