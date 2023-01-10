/*
 * TextTexture.cpp
 *
 *  Created on: Jun 6, 2016
 *      Author: Bunjukz
 */

#include <Text/Texture/TextTexture.h>

TextC::TextC(){
	mWidth 		= 0;
	mHeight 	= 0;
	mTexture 	= NULL;
	currentText = "No Text Set!";
}

TextC::~TextC(){
	if(mTexture!=NULL){
		SDL_DestroyTexture(mTexture);
		mTexture 	= NULL;
		mWidth 		= 0;
		mHeight 	= 0;
		currentText = "";
	}
}
int TextC::LFT(std::string newText, SDL_Renderer* mRender, SDL_Color newTextColor, TTF_Font* newTextFont, int w){
	if(currentText.c_str() != newText.c_str()){
		Clean();
		SDL_Texture* newTextTexture;
		SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(newTextFont, newText.c_str(), newTextColor,w);
		if(textSurface == NULL){
			printf("Unable to create text surface! %s \n",TTF_GetError());
			return 1;
		}else{
			newTextTexture = SDL_CreateTextureFromSurface(mRender,textSurface);
			if(newTextTexture==NULL){
				printf("Unable to create text texture from surface! %s \n",TTF_GetError());
				return 1;
			}else{
				mWidth 	= textSurface->w;
				mHeight = textSurface->h;
			}
			SDL_FreeSurface(textSurface);
		}
		mTexture 	= newTextTexture;
		currentText = newText;
	}
	return 0;
}

void TextC::Clean(){
	SDL_DestroyTexture(mTexture);
	mWidth 	= 0;
	mHeight = 0;
}

void TextC::Render(int x, int y, SDL_Renderer* mRender){
	SDL_Rect renderQuad = {x,y,mWidth,mHeight};
	SDL_RenderCopy(mRender,mTexture,NULL,&renderQuad);
}

int TextC::GetHeight(){return mHeight;}
int TextC::GetWidth(){return mWidth;}
