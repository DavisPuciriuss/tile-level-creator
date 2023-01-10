/*
 * DefaultButton.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: Bunjukz
 */
#include <Button/DefaultButton.h>

int DefaultButtonClass::Init(int x,int y, int w, int h,std::string text,SDL_Renderer* renderer,TextureC* newTexture,TTF_Font* font,SDL_Color color,int customBtn,SDL_Rect* _rect){
	int itmp;
	SetPos(x,y);
	customButton	= customBtn;
	mSprite 		= newTexture;
	if(customBtn == 2){
		mWidth	= mSprite->GetWidth();;
		mHeight	= mSprite->GetHeight();
	}else{
		mWidth 	= w;
		mHeight = h;
	}
	mCurSprite 		= OUTB;
	mType			= 1;

	SetSDLRect();
	if(_rect != NULL){
		mRectTile = *_rect;
	}

	if((itmp = mTextTexture.LFT(text,renderer,color,font,mWidth))){
		return itmp;
	}
	mTextRenderPos.x = mPosX + (mWidth*0.5) - (mTextTexture.GetWidth()*0.5);
	mTextRenderPos.y = mPosY + (mHeight*0.5) - (mTextTexture.GetHeight()*0.5);
	return 0;
}

void DefaultButtonClass::Init(SDL_Rect _dimensions, std::string _text, SDL_Renderer* _render, TextureC* _texture, TTF_Font* _font, SDL_Color _color){
	SetPos(_dimensions.x,_dimensions.y);
	mSprite = _texture;
	mWidth = _dimensions.w;
	mHeight = _dimensions.h;
	customButton = 0;
	mCurSprite 		= OUTB;
	SetSDLRect();
	mTextTexture.LFT(_text, _render, _color, _font, _dimensions.w);
	mTextRenderPos.x = mPosX + (mWidth*0.5) - (mTextTexture.GetWidth()*0.5);
	mTextRenderPos.y = mPosY + (mHeight*0.5) - (mTextTexture.GetHeight()*0.5);
}

void DefaultButtonClass::Init(int _x, int _y, int _size, TextureC* _texture){
	SetPos(_x,_y);
	mSprite = _texture;
	mWidth = _size;
	mHeight = _size;
	customButton = 2;
}

void DefaultButtonClass::Render(SDL_Renderer* renderer,int offSetY){
	if(!customButton){
		mSprite->Render(mPosX,mPosY,mWidth,mHeight,&mRect[mCurSprite],renderer);
		mTextTexture.Render(mTextRenderPos.x,mTextRenderPos.y,renderer);
	}else if (customButton == 1){
		mSprite->Render(mPosX,mPosY - offSetY,mWidth,mHeight,&mRectTile,renderer);
	}else{
		mSprite->Render(mPosX,mPosY - offSetY,mWidth,mHeight,NULL,renderer);
	}
}

void DefaultButtonClass::SetSDLRect(){
	int x;
	int y;
	if(!customButton){
		x = 10;
		y = 10;
		for(int i=0;i<MAXSPRITE;i++){
			mRect[i].h = y;
			mRect[i].w = x;
			mRect[i].x = x*i;
			mRect[i].y = 0;
		}
	}
}
void DefaultButtonClass::setType(int type){
	mType = type;
	SetSDLRect();
}
int DefaultButtonClass::Update(SDL_Event* e,int mouseX,int mouseY){
	bool inside = true;
	if(mouseX<mPosX){inside = false;}
	else if(mouseX>mPosX + mWidth){inside = false;}
	else if(mouseY<mPosY){inside = false;}
	else if(mouseY>mPosY + mHeight){inside = false;}
	if(!inside){
		mCurSprite = OUTB;
	}else{
		mCurSprite = INB;
	}
	if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP ){
		if(inside){
			switch(e->type){
			case SDL_MOUSEMOTION:
				mCurSprite = INB;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mCurSprite = CLICK;
				break;
			case SDL_MOUSEBUTTONUP:
				mCurSprite = RELEASE;
				return 1;
				break;
			}
		}
	}
	return 0;
}
int DefaultButtonClass::Update(str_Mouse* _mi){
	int inside = 1;
	if(_mi->location.x<mPosX){inside = 0;}
	else if(_mi->location.x>mPosX + mWidth){inside = 0;}
	else if(_mi->location.y<mPosY){inside = 0;}
	else if(_mi->location.y>mPosY + mHeight){inside = 0;}
	if(!inside){
		mCurSprite = OUTB;
		return 0;
	}else{
		mCurSprite = INB;
	}
	if(_mi->m1 == CLICKED || _mi->m1 == HELD || _mi->m2 == CLICKED || _mi->m2 == HELD)
		mCurSprite = CLICK;
	if(_mi->m1 == RELEASED){
		mCurSprite = RELEASE;
		return 1;
	}
	if(_mi->m2 == RELEASED){
		mCurSprite = RELEASE;
		return 2;
	}
	return 0;
}
void DefaultButtonClass::Clean(){
	SetPos(0,0);
	mWidth 		= 0;
	mHeight 	= 0;
	mCurSprite	= OUTB;
	mTextTexture.Clean();
}
int DefaultButtonClass::GetPosX(){
	return mPosX;
}
int DefaultButtonClass::GetPosY(){
	return mPosY;
}
void DefaultButtonClass::SetPos(int x,int y){
	mPosX = x;
	mPosY = y;
}
int DefaultButtonClass::getWidth(){
	return mWidth;
}
int DefaultButtonClass::getHeight(){
	return mHeight;
}
